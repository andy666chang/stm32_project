/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:31:12 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2025-01-02 23:50:16
 */

#include "service.h"

#include "gpio.h"

#include "interfaces/interface.h"

#include "services/throttle/config.h"

#include "components/ring_buf/ring_buf.h"
#include "components/log/log.h"
#include "system.h"

#define TAG "BTN"
#define BTN_TIMEOUT 750
#define FLASH_TIMEOUT 500
#define CALI_TIMEOUT 1500


#define BTN_SWITCH      1
#define BTN_HIGH_BEAM   2
#define BTN_FALSH       3
#define BTN_LED_SEL     4
#define BTN_CALI        6
#define BTN_MODE        7
#define BTN_DIR         9

#define ON 1
#define OFF 0
#define RC_MAX 2000
#define RC_MIN 1000
#define RC_CENTER 1500

static uint16_t btn_buf_data[10];
static struct ring_buf btn_buf;
static uint32_t time = 0; // Record time stamp for fast click

void btn_data_push(uint16_t data) {
    ring_buf_push(&btn_buf, (void *)&data);
}

uint8_t sw_state = 0;
static bool beam_state = 0;

static void (*sub_process)(void) = NULL;

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

static void btn_led_flash(void) {
    static uint32_t flash_time = 0;
    static bool led_state = 0;
    if ((log_timestamp() - flash_time) >= FLASH_TIMEOUT) {
        led_chasis_set(led_state);
        led_state = !led_state;
        flash_time = log_timestamp();
    }
}

/**
 * @brief 
 * 
 */
void btn_service_process(void) {
    static uint16_t pre_btn = RC_MAX;
    static uint8_t cnt = 0;

    while (btn_buf.cnt) {
        uint16_t data = 0;
        ring_buf_pop(&btn_buf, (void *)&data);

        LOGD(TAG, "Button signal: %d", data); // 978 or 2045 in each 15ms
        if (data >= RC_CENTER) {
            data = RC_MAX;
        } else {
            data = RC_MIN;
        }

        // Diff Button state
        if (data != pre_btn) {
            // count timeout
            time = log_timestamp();
            cnt++;
        }
        
        // record Button state
        pre_btn = data;
    }

    // Remove initial noise
    if (log_timestamp() <= 2000) {
        cnt = 0;
    }

    // Count timeout
    if ((log_timestamp() - time) >= BTN_TIMEOUT &&
        cnt) {
        LOGI(TAG, "BTN cnt: %d", cnt);
        
        // Send event
        switch (cnt) {
        case BTN_SWITCH: // Switch on/off
            if (system_get_state() == SYSTEM_LED_SELECT) {
                prj_cfg->bar_idx++;
                if (prj_cfg->bar_idx >= led_idx_max()) {
                    prj_cfg->bar_idx = 0;
                }
                LOGI(TAG, "Select bar idx: %d", prj_cfg->bar_idx);
            } else {
                LOGI(TAG, "BTN_SWITCH");
                btn_switch();
            }
            break;

        case BTN_HIGH_BEAM: // High beam
            LOGI(TAG, "BTN_HIGH_BEAM");
            btn_high_beam();
            break;

        case BTN_FALSH: // LED Flash
            LOGI(TAG, "BTN_FLASH");
            if (sub_process == btn_led_flash) {
                if (sw_state == 0)
                    led_chasis_set(OFF);
                else 
                    led_chasis_set(ON);

                sub_process = NULL;
            } else if (sub_process == NULL) {
                sub_process = btn_led_flash;
            }
            break;
        
        case BTN_LED_SEL:
            LOGI(TAG, "BTN_LED_SEL");
            if (system_get_state() == SYSTEM_NORMAL) {
                system_set_state(SYSTEM_LED_SELECT);
            } else if (system_get_state() == SYSTEM_LED_SELECT) {
                save_config();
                system_set_state(SYSTEM_NORMAL);
            }
            break;

        case BTN_CALI: // Calibration
            LOGI(TAG, "BTN_CALI");
            if (system_get_state() == SYSTEM_NORMAL) {
                system_set_state(SYSTEM_CALIBRATION);
            }
            break;

        case BTN_DIR: // Switch direction
            LOGI(TAG, "BTN_DIR");
            prj_cfg->dir = -prj_cfg->dir;
            LOGI(TAG, "dir = %d", prj_cfg->dir);
            save_config();

            // Restart
            HAL_Delay(100);
            HAL_NVIC_SystemReset();
            break;

        case BTN_MODE: // Switch mode
            LOGI(TAG, "BTN_MODE");
            prj_cfg->mode = !prj_cfg->mode;
            LOGI(TAG, "mode = %d", prj_cfg->mode);
            save_config();

            if (prj_cfg->mode == 1) {
                led_string_set(ON);
            } else {
                led_string_set(OFF);
            }
            break;

        default:
            break;
        }

        cnt = 0;
    }

    if (sub_process) {
        sub_process();
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
