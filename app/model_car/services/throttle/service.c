/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:31:12 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-11-24 13:15:41
 */

#include "service.h"

#include "gpio.h"

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

int16_t centor = 1500; // 1000 ~ 2000 us
static const uint16_t margin = 10; // 10us
static uint16_t thro_buf_data[10];
static struct ring_buf thro_buf;
static uint32_t time = 0; // Record time stamp

void thro_data_push(uint16_t data) {
    // count timeout
    time = log_timestamp();
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

        thro = data - centor;

        LOGI(TAG, "Throttle signal: %d", data);

        // TODO: check short 
        if ((abs(thro) < abs(pre_thro)) &&
            (abs(thro) > margin)) {
            event_cap |= BIT(THRO_SHORT);
            short_timeout = log_timestamp();
        }

        // TODO: check long 
        if ((abs(thro) < abs(pre_thro)) &&
            (abs(thro) < margin)) {
            event_cap |= BIT(THRO_LONG);
            long_timeout = log_timestamp();
        }

        // TODO: check fire 
        if ((pre_thro > margin) &&
            (abs(thro) < margin)) {
            event_cap |= BIT(THRO_FIRE);
            fire_timeout = log_timestamp();
        }

        
        // Record thro state
        pre_thro = thro;
    }

    /* Check throttle event */
    // short 
    if (event_cap & BIT(THRO_SHORT)) {
        // TODO: turn on tail led

        if ( (log_timestamp() - short_timeout) > THRO_SHORT_TIMEOUT ) {
            // TODO: turn off tail led
            
            // Clear event
            event_cap &= ~(BIT(THRO_SHORT));
        }
    }

    // long
    if (event_cap & BIT(THRO_LONG)) {
        // TODO: turn on tail led

        if ( (log_timestamp() - long_timeout) > THRO_LONG_TIMEOUT ) {
            // TODO: turn off tail led
            
            // Clear event
            event_cap &= ~(BIT(THRO_LONG));
        }
    }

    // fire
    if (event_cap & BIT(THRO_FIRE)) {
        // TODO: turn on fire led

        if ( (log_timestamp() - fire_timeout) > THRO_FIRE_TIMEOUT ) {
            // TODO: turn off fire led
            
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

    return 0;
}
