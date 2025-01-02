/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:31:12 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2025-01-02 16:36:27
 */

#include "service.h"
#include "config.h"

#include "gpio.h"

#include "interfaces/interface.h"

#include "components/ring_buf/ring_buf.h"
#include "components/log/log.h"

#define TAG "THRO"
#define THRO_SHORT_TIMEOUT  500
#define THRO_LONG_TIMEOUT  1000
#define THRO_FIRE_TIMEOUT   500

#define THRO_NONE   0
#define THRO_SHORT  1
#define THRO_LONG   2
#define THRO_FIRE   3

#define BIT(n) (UINT32_C(1) << (n))

#define ON 1
#define OFF 0

// int16_t centor = 1500; // 1000 ~ 2000 us
// static const uint16_t margin = 20; // 20us
static uint16_t thro_buf_data[10];
static struct ring_buf thro_buf;

extern uint8_t sw_state;

void thro_data_push(uint16_t data) {
    ring_buf_push(&thro_buf, (void *)&data);
}

/**
 * @brief 
 * 
 */
void thro_service_process(void) {
    static int16_t pre_thro = 0;
    static uint8_t event_cap = 0;
    static uint32_t short_timeout, long_timeout, fire_timeout;
    int16_t thro = 0;

    while (thro_buf.cnt) {
        uint16_t data = 0;
        ring_buf_pop(&thro_buf, (void *)&data);

        thro = data - prj_cfg->center;
        thro *= prj_cfg->dir;

        LOGD(TAG, "Throttle signal: %d", thro); // 1500 +- 544 in each 15ms

        // Check short 
        if ((abs(thro) < abs(pre_thro)) &&
            (abs(thro-pre_thro) > prj_cfg->margin) &&
            (abs(thro) > prj_cfg->margin)) {
            LOGI(TAG, "THRO_SHORT");
            event_cap |= BIT(THRO_SHORT);
            short_timeout = log_timestamp();
        }

        // Check long 
        if ((abs(thro) < abs(pre_thro)) &&
            (abs(thro-pre_thro) > prj_cfg->margin) &&
            (abs(thro) < prj_cfg->margin)) {
            LOGI(TAG, "THRO_LONG");
            event_cap |= BIT(THRO_LONG);
            long_timeout = log_timestamp();
        }

        // Check fire 
        if ((pre_thro > prj_cfg->margin) &&
            (abs(thro) < prj_cfg->margin)) {
            LOGI(TAG, "THRO_FIRE");
            event_cap |= BIT(THRO_FIRE);
            fire_timeout = log_timestamp();
        }

        
        // Record thro state
        pre_thro = thro;
    }

    /* Check throttle event */
    // short 
    if (event_cap & BIT(THRO_SHORT)) {
        // turn on tail led
        led_tail_set(0, ON);
        led_tail_set(1, ON);

        if ( (log_timestamp() - short_timeout) > THRO_SHORT_TIMEOUT ) {
            // turn off tail led
            if (sw_state == 0)
                led_tail_set(0, OFF);
            led_tail_set(1, OFF);
            
            // Clear event
            event_cap &= ~(BIT(THRO_SHORT));
        }
    }

    // long
    if (event_cap & BIT(THRO_LONG)) {
        // turn on tail led
        led_tail_set(0, ON);
        led_tail_set(1, ON);

        if ( (log_timestamp() - long_timeout) > THRO_LONG_TIMEOUT ) {
            // turn off tail led
            if (sw_state == 0)
                led_tail_set(0, OFF);
            led_tail_set(1, OFF);
            
            // Clear event
            event_cap &= ~(BIT(THRO_LONG));
        }
    }

    // fire
    if (event_cap & BIT(THRO_FIRE)) {
        // turn on fire led
        led_fire_set(ON);

        if ( (log_timestamp() - fire_timeout) > THRO_FIRE_TIMEOUT ) {
            // turn off fire led
            led_fire_set(OFF);
            
            // Clear event
            event_cap &= ~(BIT(THRO_FIRE));
        }
    }

    return;
}

/**
 * @brief 
 * 
 * @return int 
 */
int thro_servic_init(void) {
    // Init Ring buffer
    RING_BUF_INIT(thro_buf, thro_buf_data);

    config_init();

    return 0;
}
