/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:31:12 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-11-24 03:48:14
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

#include "components/ring_buf/ring_buf.h"
#include "components/log/log.h"

#define TAG "BTN"
#define BTN_TIMEOUT 300

static uint16_t btn_buf_data[10];
static struct ring_buf btn_buf;
static uint32_t time = 0; // Record time stamp for fast click

void btn_data_push(uint16_t data) {
    // TODO: count timeout
    time = log_timestamp();
    ring_buf_push(&btn_buf, (void *)&data);
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
        if (data != pre_btn)
            cnt++;
        
        // TODO: record Button state
        pre_btn = data;
    }

    // Count timeout
    if ((log_timestamp() - time) >= BTN_TIMEOUT &&
        cnt) {
        
        // TODO: Send event
        switch (cnt) {
        case 1: // Switch on/off
            break;

        case 2: // High beam
            break;

        case 6: // Calibration
            break;

        case 9: // Switch direction
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
