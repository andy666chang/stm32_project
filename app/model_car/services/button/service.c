/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:31:12 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-12-05 23:04:02
 */

#include "service.h"

#include "gpio.h"

#include "interfaces/interface.h"

#include "components/ring_buf/ring_buf.h"
#include "components/log/log.h"

#define TAG "BTN"
#define BTN_TIMEOUT 300

#define BTN_SWITCH      1
#define BTN_HIGH_BEAM   2
#define BTN_CALI        6
#define BTN_DIR         9

#define ON 1
#define OFF 0

static uint16_t btn_buf_data[10];
static struct ring_buf btn_buf;
static uint32_t time = 0; // Record time stamp for fast click

void btn_data_push(uint16_t data) {
    // count timeout
    time = log_timestamp();
    ring_buf_push(&btn_buf, (void *)&data);
}

uint8_t sw_state = 0;
static bool beam_state = 0;

static void btn_switch(void) {
    sw_state++;
    sw_state %= 4;
    LOGI(TAG, "%s state: %d", __func__, sw_state);

    switch (sw_state) {
    case 0:
        led_chasis_set(OFF);
        led_tail_set(0, OFF);
        break;

    case 1:
        led_chasis_set(ON);
        led_tail_set(0, ON);
        break;

    case 2:
        led_chasis_set(ON);
        led_tail_set(0, ON);
        led_head_set(0, ON);
        break;

    case 3:
        led_chasis_set(ON);
        led_tail_set(0, ON);
        led_head_set(0, OFF);
        break;
    
    default:
        break;
    }
}

static void btn_high_beam(void) {
    beam_state = !beam_state;
    LOGI(TAG, "%s beam: %d", __func__, beam_state);

    if (beam_state) {
        // turn on high beam
        led_head_set(1, ON);
        led_head_set(0, ON);
    } else {
        // turn off high beam
        led_head_set(1, OFF);

        // Keep btn switch status
        if (sw_state == 2)
            led_head_set(0, ON);
        else
            led_head_set(0, OFF);
    }
}

/**
 * @brief 
 * 
 */
void btn_service_process(void) {
    static uint16_t pre_btn = 0;
    static uint8_t cnt = 0;

    while (btn_buf.cnt) {
        uint16_t data = 0;
        ring_buf_pop(&btn_buf, (void *)&data);

        LOGI(TAG, "Button signal: %d", data);

        // TODO: Diff Button state
        if (data != pre_btn && data == 0)
            cnt++;
        
        // record Button state
        pre_btn = data;
    }

    // Count timeout
    if ((log_timestamp() - time) >= BTN_TIMEOUT &&
        cnt) {
        
        // TODO: Send event
        switch (cnt) {
        case BTN_SWITCH: // Switch on/off
            LOGI(TAG, "BTN_SWITCH");
            btn_switch();
            break;

        case BTN_HIGH_BEAM: // High beam
            LOGI(TAG, "BTN_HIGH_BEAM");
            btn_high_beam();
            break;

        case BTN_CALI: // Calibration
            LOGI(TAG, "BTN_CALI");
            break;

        case BTN_DIR: // Switch direction
            LOGI(TAG, "BTN_DIR");
            break;

        default:
            break;
        }

        cnt = 0;
    }
    
    return;
}

/**
 * @brief 
 * 
 * @return int 
 */
int btn_servic_init(void) {
    // Init Ring buffer
    RING_BUF_INIT(btn_buf, btn_buf_data);

    return 0;
}
