/*
 * @Author: andy.chang 
 * @Date: 2024-11-25 22:32:13 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2025-12-26 16:48:41
 */

#include <math.h>
#include "interface.h"
#include "gpio.h"

#include "components/log/log.h"
#define TAG "LED_BAR"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#ifndef ARRAY_SIZE
  #define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))
#endif

// #define SK6812
#define WS2812B

union led_pack_t {
    struct led_data_t {
        // Little endian
        uint8_t b;
        uint8_t r;
        uint8_t g;
        uint8_t rev;
    } color;

    uint32_t data;
};

enum {
    BAR_THRO = 0,
    BAR_WHITE,
    BAR_RED,
    BAR_BRAKE = BAR_RED,
    BAR_ORANGE,
    BAR_YELLOW,
    BAR_GREEN,
    BAR_CYAN,
    BAR_BLUE,
    BAR_PURPLE,
    BAR_PINK,

    BAR_MAX,
};

static const union led_pack_t led_ref[BAR_MAX][12] =
    {
        [BAR_THRO] =
            {
                {.color = {.r =  0, .g = 60, .b = 0}},
                {.color = {.r = 10, .g = 50, .b = 0}},
                {.color = {.r = 20, .g = 40, .b = 0}},
                {.color = {.r = 30, .g = 30, .b = 0}},
                {.color = {.r = 40, .g = 25, .b = 0}},
                {.color = {.r = 50, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 15, .b = 0}},
                {.color = {.r = 70, .g = 15, .b = 0}},
                {.color = {.r = 80, .g = 10, .b = 0}},
                {.color = {.r = 90, .g =  5, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
            },
        [BAR_WHITE] =
            {
                {.color = {.r = 20, .g = 15, .b = 20}},
                {.color = {.r = 20, .g = 15, .b = 20}},
                {.color = {.r = 20, .g = 15, .b = 20}},
                {.color = {.r = 20, .g = 15, .b = 20}},
                {.color = {.r = 20, .g = 15, .b = 20}},
                {.color = {.r = 20, .g = 15, .b = 20}},
                {.color = {.r = 20, .g = 15, .b = 20}},
                {.color = {.r = 20, .g = 15, .b = 20}},
                {.color = {.r = 20, .g = 15, .b = 20}},
                {.color = {.r = 20, .g = 15, .b = 20}},
                {.color = {.r = 20, .g = 15, .b = 20}},
                {.color = {.r = 20, .g = 15, .b = 20}},
            },
        [BAR_RED] =
            {
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
                {.color = {.r = 100, .g = 0, .b = 0}},
            },
        [BAR_ORANGE] =
            {
                {.color = {.r = 60, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 20, .b = 0}},
                {.color = {.r = 60, .g = 20, .b = 0}},
            },
        [BAR_YELLOW] =
            {
                {.color = {.r = 60, .g = 40, .b = 0}},
                {.color = {.r = 60, .g = 40, .b = 0}},
                {.color = {.r = 60, .g = 40, .b = 0}},
                {.color = {.r = 60, .g = 40, .b = 0}},
                {.color = {.r = 60, .g = 40, .b = 0}},
                {.color = {.r = 60, .g = 40, .b = 0}},
                {.color = {.r = 60, .g = 40, .b = 0}},
                {.color = {.r = 60, .g = 40, .b = 0}},
                {.color = {.r = 60, .g = 40, .b = 0}},
                {.color = {.r = 60, .g = 40, .b = 0}},
                {.color = {.r = 60, .g = 40, .b = 0}},
                {.color = {.r = 60, .g = 40, .b = 0}},
            },
        [BAR_GREEN] =
            {
                {.color = {.r = 0, .g = 60, .b = 0}},
                {.color = {.r = 0, .g = 60, .b = 0}},
                {.color = {.r = 0, .g = 60, .b = 0}},
                {.color = {.r = 0, .g = 60, .b = 0}},
                {.color = {.r = 0, .g = 60, .b = 0}},
                {.color = {.r = 0, .g = 60, .b = 0}},
                {.color = {.r = 0, .g = 60, .b = 0}},
                {.color = {.r = 0, .g = 60, .b = 0}},
                {.color = {.r = 0, .g = 60, .b = 0}},
                {.color = {.r = 0, .g = 60, .b = 0}},
                {.color = {.r = 0, .g = 60, .b = 0}},
                {.color = {.r = 0, .g = 60, .b = 0}},
            },
        [BAR_CYAN] =
            {
                {.color = {.r = 0, .g = 25, .b =  25}},
                {.color = {.r = 0, .g = 25, .b =  25}},
                {.color = {.r = 0, .g = 25, .b =  25}},
                {.color = {.r = 0, .g = 25, .b =  25}},
                {.color = {.r = 0, .g = 25, .b =  25}},
                {.color = {.r = 0, .g = 25, .b =  25}},
                {.color = {.r = 0, .g = 25, .b =  25}},
                {.color = {.r = 0, .g = 25, .b =  25}},
                {.color = {.r = 0, .g = 25, .b =  25}},
                {.color = {.r = 0, .g = 25, .b =  25}},
                {.color = {.r = 0, .g = 25, .b =  25}},
                {.color = {.r = 0, .g = 25, .b =  25}},
            },
        [BAR_BLUE] =
            {
                {.color = {.r = 0, .g = 0, .b = 100}},
                {.color = {.r = 0, .g = 0, .b = 100}},
                {.color = {.r = 0, .g = 0, .b = 100}},
                {.color = {.r = 0, .g = 0, .b = 100}},
                {.color = {.r = 0, .g = 0, .b = 100}},
                {.color = {.r = 0, .g = 0, .b = 100}},
                {.color = {.r = 0, .g = 0, .b = 100}},
                {.color = {.r = 0, .g = 0, .b = 100}},
                {.color = {.r = 0, .g = 0, .b = 100}},
                {.color = {.r = 0, .g = 0, .b = 100}},
                {.color = {.r = 0, .g = 0, .b = 100}},
                {.color = {.r = 0, .g = 0, .b = 100}},
            },
        [BAR_PURPLE] =
            {
                {.color = {.r = 10, .g = 0, .b = 25}},
                {.color = {.r = 10, .g = 0, .b = 25}},
                {.color = {.r = 10, .g = 0, .b = 25}},
                {.color = {.r = 10, .g = 0, .b = 25}},
                {.color = {.r = 10, .g = 0, .b = 25}},
                {.color = {.r = 10, .g = 0, .b = 25}},
                {.color = {.r = 10, .g = 0, .b = 25}},
                {.color = {.r = 10, .g = 0, .b = 25}},
                {.color = {.r = 10, .g = 0, .b = 25}},
                {.color = {.r = 10, .g = 0, .b = 25}},
                {.color = {.r = 10, .g = 0, .b = 25}},
                {.color = {.r = 10, .g = 0, .b = 25}},
            },
        [BAR_PINK] =
            {
                {.color = {.r = 30, .g = 0, .b = 15}},
                {.color = {.r = 30, .g = 0, .b = 15}},
                {.color = {.r = 30, .g = 0, .b = 15}},
                {.color = {.r = 30, .g = 0, .b = 15}},
                {.color = {.r = 30, .g = 0, .b = 15}},
                {.color = {.r = 30, .g = 0, .b = 15}},
                {.color = {.r = 30, .g = 0, .b = 15}},
                {.color = {.r = 30, .g = 0, .b = 15}},
                {.color = {.r = 30, .g = 0, .b = 15}},
                {.color = {.r = 30, .g = 0, .b = 15}},
                {.color = {.r = 30, .g = 0, .b = 15}},
                {.color = {.r = 30, .g = 0, .b = 15}},
            },
};

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t led_idx_max(void) {
    return ARRAY_SIZE(led_ref);
}

/**
 * @brief 
 * 
 * @param en 
 */
void led_bar_set(bool en) {
    HAL_GPIO_WritePin(SW_GPIO_Port, SW_Pin, en);
}

/**
 * @brief 
 * 
 */
static void led_reset(void) {
    // Trst: Low level > 80us
    HAL_GPIO_WritePin(CHASIS_GPIO_Port, CHASIS_Pin, GPIO_PIN_RESET);
    for (size_t i = 0; i < 800; i++) {
        __NOP();
    }
}

//  64MHz -> _NOP = 15.625ns
/**
 * @brief 
 * 
 */
static inline void led_set_low(void) {
#ifdef SK6812
    // T0H: 0.3 +- 0.15 us
    LL_GPIO_SetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();

    // T0L: 0.9 +- 0.15 us
    LL_GPIO_ResetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
#else
    // T0H: 0.4 +- 0.15 us
    LL_GPIO_SetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();

    // T0L: 0.85 +- 0.15 us
    LL_GPIO_ResetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
#endif
}

/**
 * @brief 
 * 
 */
static inline void led_set_high(void) {
#ifdef SK6812
    // T1H: 0.9 +- 0.15 us
    LL_GPIO_SetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();

    // T1L: 0.3 +- 0.15 us
    LL_GPIO_ResetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP();
#else
    // T1H: 0.8 +- 0.15 us
    LL_GPIO_SetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP();

    // T1L: 0.45 +- 0.15 us
    LL_GPIO_ResetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP();
#endif
}

/**
 * @brief 
 * 
 * @param data 
 */
static inline void led_data_set(uint32_t data) {
    LOGD(TAG, "led_data: 0x%06X", data);

    for (int8_t i = 0; i < 24; i++) {
        if (data & (0x800000)) {
            led_set_high();
        } else {
            led_set_low();
        }
        data <<= 1;
    }
}

/**
 * @brief 
 * 
 * @param r 
 * @param g 
 * @param b 
 */
static void led_color_set(uint8_t r, uint8_t g, uint8_t b) {
    led_data_set(((uint32_t)g << 16) | ((uint32_t)r << 8) | ((uint32_t)b));
}

/**
 * @brief 
 * 
 * 
 * @param thro 
 */
void thro_led_update(int16_t thro) {
    uint8_t idx = prj_cfg->bar_idx;
    uint8_t step = floorf(abs(prj_cfg->max - prj_cfg->center) / 12.0);
    uint8_t on, off;

    if (idx >= ARRAY_SIZE(led_ref)) {
        LOGE(TAG, "Invalid bar idx: %d", idx);
        idx = 0;
    }
    thro = MAX(0, thro);

    on = MIN((thro / step), 12);
    on = MAX(on, 1);
    off = 12 - on;

    LOGD(TAG, "thro: %d, on: %d, off: %d", thro, on, off);

    __disable_irq();
    for (size_t i = 0; i < on; i++) {
        led_data_set(led_ref[idx][i].data);
    }

    for (size_t i = 0; i < off; i++) {
        led_data_set(0x000000);
    }
    __enable_irq();

    led_reset();
}

/**
 * @brief 
 * 
 * 
 * @param thro 
 */
void thro_led_brake(void) {

    __disable_irq();
    for (size_t i = 0; i < 12; i++) {
        led_data_set(led_ref[BAR_BRAKE][i].data);
    }
    __enable_irq();

    led_reset();
}

/**
 * @brief 
 * 
 * @param r 
 * @param g 
 * @param b 
 */
void thro_led_set(uint8_t r, uint8_t g, uint8_t b) {

    __disable_irq();
    for (size_t i = 0; i < 12; i++) {
        led_color_set(r, g, b);
    }
    __enable_irq();

    led_reset();
}
