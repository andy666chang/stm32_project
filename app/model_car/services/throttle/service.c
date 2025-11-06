/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:31:12 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2025-01-02 16:51:04
 */

#include "service.h"
#include "config.h"

#include "gpio.h"

#include "interfaces/interface.h"

#include "components/ring_buf/ring_buf.h"
#include "components/log/log.h"
#include "system.h"

#define TAG "THRO"
#define THRO_SHORT_TIMEOUT  300
#define THRO_LONG_TIMEOUT  1000
#define THRO_FIRE_TIMEOUT    50
#define THRO_CALI_TIMEOUT  2000

enum {
    THRO_INPUT = 0,
    THRO_SHORT,
    THRO_LONG,
    THRO_FIRE,
    THRO_BRAKE,
    THRO_UPDATE,
    THRO_CALI,
};

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
    static uint16_t event_cap = 0;
    static uint32_t short_timeout;
    // static uint32_t long_timeout;
    static uint32_t fire_timeout;
    static uint32_t cali_time; // Record time stamp for calibration
    int16_t thro = 0;
    static uint16_t data = 0;

    while (thro_buf.cnt) {
        ring_buf_pop(&thro_buf, (void *)&data);
        event_cap |= BIT(THRO_INPUT);

        // Check calibration
        if (system_get_state() == SYSTEM_CALIBRATION) {
            if ((event_cap & BIT(THRO_CALI)) == 0) {
                event_cap |= BIT(THRO_CALI);
                cali_time = log_timestamp();
            }
            break;
        }

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

        // Check brake
        if ((abs(pre_thro) < prj_cfg->margin) &&
            (abs(thro) > prj_cfg->margin) &&
            (thro < 0)) {
            LOGI(TAG, "THRO_BRAKE");
            event_cap |= BIT(THRO_BRAKE);
        }

        // Check long 
        // if ((abs(thro) < abs(pre_thro)) &&
        //     (abs(thro-pre_thro) > prj_cfg->margin) &&
        //     (abs(thro) < prj_cfg->margin)) {
        //     LOGI(TAG, "THRO_LONG");
        //     event_cap |= BIT(THRO_LONG);
        //     long_timeout = log_timestamp();
        // }

        // Check fire 
        if ((pre_thro > prj_cfg->margin) &&
            (abs(thro) < prj_cfg->margin)) {
            LOGI(TAG, "THRO_FIRE");
            event_cap |= BIT(THRO_FIRE);
            fire_timeout = log_timestamp();
        }

        // Check thro update
        if (prj_cfg->mode == 1) {
            event_cap |= BIT(THRO_UPDATE);
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
    // if (event_cap & BIT(THRO_LONG)) {
    //     // turn on tail led
    //     led_tail_set(0, ON);
    //     led_tail_set(1, ON);

    //     if ( (log_timestamp() - long_timeout) > THRO_LONG_TIMEOUT ) {
    //         // turn off tail led
    //         if (sw_state == 0)
    //             led_tail_set(0, OFF);
    //         led_tail_set(1, OFF);
            
    //         // Clear event
    //         event_cap &= ~(BIT(THRO_LONG));
    //     }
    // }

    // fire
    if (event_cap & BIT(THRO_FIRE)) {
        uint32_t duration = log_timestamp() - fire_timeout;

        if (duration > 3 * THRO_FIRE_TIMEOUT) {
            // turn off fire led
            led_fire_set(OFF);

            // Clear event
            event_cap &= ~(BIT(THRO_FIRE));
        } else if (duration > 2 * THRO_FIRE_TIMEOUT) {
            // turn off fire led
            led_fire_set(ON);
        } else if (duration > 1 * THRO_FIRE_TIMEOUT) {
            // turn off fire led
            led_fire_set(OFF);
        } else if (duration < THRO_FIRE_TIMEOUT) {
            // turn on fire led
            led_fire_set(ON);
        }
    }

    // Brake
    if ((event_cap & BIT(THRO_BRAKE)) && (event_cap & BIT(THRO_INPUT))) {
        // turn on tail led
        led_tail_set(0, ON);
        led_tail_set(1, ON);

        if ((abs(thro) < prj_cfg->margin) || (thro > prj_cfg->margin)) {
            // turn off tail led
            if (sw_state == 0)
                led_tail_set(0, OFF);
            led_tail_set(1, OFF);

            // Clear event
            event_cap &= ~(BIT(THRO_BRAKE));
        }
    }

    // calibration
    if (event_cap & BIT(THRO_CALI)) {
        //  Capture center value
        if ((log_timestamp() - cali_time) < THRO_CALI_TIMEOUT) {
            //  Capture center value
            prj_cfg->center = data;

            led_tail_set(0, ON);
            led_tail_set(1, ON);
        } else if ((log_timestamp() - cali_time) < 2 * THRO_CALI_TIMEOUT) {
            //  Capture max value
            prj_cfg->max = data;
            led_fire_set(ON);
        } else {
            LOGI(TAG, "center = %d", prj_cfg->center);
            LOGI(TAG, "max = %d", prj_cfg->max);
            save_config();

            // Exit calibration mode
            system_set_state(SYSTEM_NORMAL);
            LOGI(TAG, "Exit calibration mode");

            // Clear event
            event_cap &= ~(BIT(THRO_CALI));

            // Restart
            HAL_Delay(100);
            HAL_NVIC_SystemReset();
        }
    }

    if ((event_cap & BIT(THRO_UPDATE))) {
        thro_led_update(thro);
        thro_led_update(thro); // update twice to avoid miss
        event_cap &= ~(BIT(THRO_UPDATE));
    }

    event_cap &= ~(BIT(THRO_INPUT));
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
