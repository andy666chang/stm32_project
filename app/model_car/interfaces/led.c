/*
 * @Author: andy.chang 
 * @Date: 2024-11-25 22:32:13 
 * @Last Modified by:   andy.chang 
 * @Last Modified time: 2024-11-25 22:32:13 
 */

#include <math.h>
#include "interface.h"
#include "../services/throttle/config.h"
#include "gpio.h"

#include "components/log/log.h"
#define TAG "LED"

#define ON 1
#define OFF 0

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

/**
 * @brief 
 * 
 * @param id 
 * @param en 
 */
void led_head_set(uint8_t id, bool en) {
    en = !en;
    switch (id) {
    case 0:
        HAL_GPIO_WritePin(HEAD_0_GPIO_Port, HEAD_0_Pin, en);
        break;
    
    case 1:
        HAL_GPIO_WritePin(HEAD_1_GPIO_Port, HEAD_1_Pin, en);
        break;
    
    default:
        LOGE(TAG,"Unknow id: %d", id);
        break;
    }
}

/**
 * @brief 
 * 
 * @param id 
 * @return true 
 * @return false 
 */
bool led_head_get(uint8_t id) {
    bool val = 0;

    switch (id) {
    case 0:
        val = HAL_GPIO_ReadPin(HEAD_0_GPIO_Port, HEAD_0_Pin);
        break;
    
    case 1:
        val = HAL_GPIO_ReadPin(HEAD_1_GPIO_Port, HEAD_1_Pin);
        break;
    
    default:
        LOGE(TAG,"Unknow id: %d", id);
        break;
    }

    return !val;
}

/**
 * @brief 
 * 
 * @param id 
 * @param en 
 */
void led_tail_set(uint8_t id, bool en) {
    en = !en;

    switch (id) {
    case 0:
        HAL_GPIO_WritePin(TAIL_0_GPIO_Port, TAIL_0_Pin, en);
        break;
    
    case 1:
        HAL_GPIO_WritePin(TAIL_1_GPIO_Port, TAIL_1_Pin, en);
        break;
    
    default:
        LOGE(TAG,"Unknow id: %d", id);
        break;
    }
}

/**
 * @brief 
 * 
 * @param en 
 * @return true 
 * @return false 
 */
bool led_tail_get(uint8_t id) {
    bool val = 0;

    switch (id) {
    case 0:
        val = HAL_GPIO_ReadPin(TAIL_0_GPIO_Port, TAIL_0_Pin);
        break;
    
    case 1:
        val = HAL_GPIO_ReadPin(TAIL_1_GPIO_Port, TAIL_1_Pin);
        break;
    
    default:
        LOGE(TAG,"Unknow id: %d", id);
        break;
    }

    return !val;
}

/**
 * @brief 
 * 
 * @param en 
 */
void led_chasis_set(bool en) {
    if (prj_cfg->mode == 0) {
        en = !en;
        HAL_GPIO_WritePin(CHASIS_GPIO_Port, CHASIS_Pin, en);
    }
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool led_chasis_get(void) {
    return !HAL_GPIO_ReadPin(CHASIS_GPIO_Port, CHASIS_Pin);
}

/**
 * @brief 
 * 
 * @param en 
 */
void led_fire_set(bool en) {
    en = !en;
    HAL_GPIO_WritePin(FIRE_GPIO_Port, FIRE_Pin, en);
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool led_fire_get(void) {
    return !HAL_GPIO_ReadPin(FIRE_GPIO_Port, FIRE_Pin);
}

/**
 * @brief 
 * 
 * @param en 
 */
void led_string_set(bool en) {
    HAL_GPIO_WritePin(SW_GPIO_Port, SW_Pin, en);
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
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP();

    // T0L: 0.9 +- 0.15 us
    LL_GPIO_ResetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
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
    // T1H: 0.6 +- 0.15 us
    LL_GPIO_SetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();

    // T1L: 0.6 +- 0.15 us
    LL_GPIO_ResetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
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
 */
static void led_reset(void) {
    // Trst: Low level > 80us
    HAL_GPIO_WritePin(CHASIS_GPIO_Port, CHASIS_Pin, GPIO_PIN_RESET);
    for (size_t i = 0; i < 800; i++) {
        __NOP();
    }
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
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 33, .g = 222, .b = 0}},
                {.color = {.r = 66, .g = 189, .b = 0}},
                {.color = {.r = 99, .g = 156, .b = 0}},
                {.color = {.r = 132, .g = 123, .b = 0}},
                {.color = {.r = 165, .g = 100, .b = 0}},
                {.color = {.r = 198, .g = 75, .b = 0}},
                {.color = {.r = 231, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 25, .b = 0}},
                {.color = {.r = 255, .g = 12, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
            },
        [BAR_WHITE] =
            {
                {.color = {.r = 80, .g = 80, .b = 80}},
                {.color = {.r = 80, .g = 80, .b = 80}},
                {.color = {.r = 80, .g = 80, .b = 80}},
                {.color = {.r = 80, .g = 80, .b = 80}},
                {.color = {.r = 80, .g = 80, .b = 80}},
                {.color = {.r = 80, .g = 80, .b = 80}},
                {.color = {.r = 80, .g = 80, .b = 80}},
                {.color = {.r = 80, .g = 80, .b = 80}},
                {.color = {.r = 80, .g = 80, .b = 80}},
                {.color = {.r = 80, .g = 80, .b = 80}},
                {.color = {.r = 80, .g = 80, .b = 80}},
                {.color = {.r = 80, .g = 80, .b = 80}},
            },
        [BAR_RED] =
            {
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
                {.color = {.r = 255, .g = 0, .b = 0}},
            },
        [BAR_ORANGE] =
            {
                {.color = {.r = 255, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 50, .b = 0}},
                {.color = {.r = 255, .g = 50, .b = 0}},
            },
        [BAR_YELLOW] =
            {
                {.color = {.r = 200, .g = 80, .b = 0}},
                {.color = {.r = 200, .g = 80, .b = 0}},
                {.color = {.r = 200, .g = 80, .b = 0}},
                {.color = {.r = 200, .g = 80, .b = 0}},
                {.color = {.r = 200, .g = 80, .b = 0}},
                {.color = {.r = 200, .g = 80, .b = 0}},
                {.color = {.r = 200, .g = 80, .b = 0}},
                {.color = {.r = 200, .g = 80, .b = 0}},
                {.color = {.r = 200, .g = 80, .b = 0}},
                {.color = {.r = 200, .g = 80, .b = 0}},
                {.color = {.r = 200, .g = 80, .b = 0}},
                {.color = {.r = 200, .g = 80, .b = 0}},
            },
        [BAR_GREEN] =
            {
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 0, .g = 255, .b = 0}},
                {.color = {.r = 0, .g = 255, .b = 0}},
            },
        [BAR_CYAN] =
            {
                {.color = {.r = 0, .g = 40, .b =  40}},
                {.color = {.r = 0, .g = 40, .b =  40}},
                {.color = {.r = 0, .g = 40, .b =  40}},
                {.color = {.r = 0, .g = 40, .b =  40}},
                {.color = {.r = 0, .g = 40, .b =  40}},
                {.color = {.r = 0, .g = 40, .b =  40}},
                {.color = {.r = 0, .g = 40, .b =  40}},
                {.color = {.r = 0, .g = 40, .b =  40}},
                {.color = {.r = 0, .g = 40, .b =  40}},
                {.color = {.r = 0, .g = 40, .b =  40}},
                {.color = {.r = 0, .g = 40, .b =  40}},
                {.color = {.r = 0, .g = 40, .b =  40}},
            },
        [BAR_BLUE] =
            {
                {.color = {.r = 0, .g = 0, .b = 255}},
                {.color = {.r = 0, .g = 0, .b = 255}},
                {.color = {.r = 0, .g = 0, .b = 255}},
                {.color = {.r = 0, .g = 0, .b = 255}},
                {.color = {.r = 0, .g = 0, .b = 255}},
                {.color = {.r = 0, .g = 0, .b = 255}},
                {.color = {.r = 0, .g = 0, .b = 255}},
                {.color = {.r = 0, .g = 0, .b = 255}},
                {.color = {.r = 0, .g = 0, .b = 255}},
                {.color = {.r = 0, .g = 0, .b = 255}},
                {.color = {.r = 0, .g = 0, .b = 255}},
                {.color = {.r = 0, .g = 0, .b = 255}},
            },
        [BAR_PURPLE] =
            {
                {.color = {.r = 128, .g = 0, .b = 150}},
                {.color = {.r = 128, .g = 0, .b = 150}},
                {.color = {.r = 128, .g = 0, .b = 150}},
                {.color = {.r = 128, .g = 0, .b = 150}},
                {.color = {.r = 128, .g = 0, .b = 150}},
                {.color = {.r = 128, .g = 0, .b = 150}},
                {.color = {.r = 128, .g = 0, .b = 150}},
                {.color = {.r = 128, .g = 0, .b = 150}},
                {.color = {.r = 128, .g = 0, .b = 150}},
                {.color = {.r = 128, .g = 0, .b = 150}},
                {.color = {.r = 128, .g = 0, .b = 150}},
                {.color = {.r = 128, .g = 0, .b = 150}},
            },
        [BAR_PINK] =
            {
                {.color = {.r = 200, .g = 0, .b = 30}},
                {.color = {.r = 200, .g = 0, .b = 30}},
                {.color = {.r = 200, .g = 0, .b = 30}},
                {.color = {.r = 200, .g = 0, .b = 30}},
                {.color = {.r = 200, .g = 0, .b = 30}},
                {.color = {.r = 200, .g = 0, .b = 30}},
                {.color = {.r = 200, .g = 0, .b = 30}},
                {.color = {.r = 200, .g = 0, .b = 30}},
                {.color = {.r = 200, .g = 0, .b = 30}},
                {.color = {.r = 200, .g = 0, .b = 30}},
                {.color = {.r = 200, .g = 0, .b = 30}},
                {.color = {.r = 200, .g = 0, .b = 30}},
            },
};

uint8_t led_idx_max(void) {
    return ARRAY_SIZE(led_ref);
}

/**
 * @brief 
 * 
 * 
 * @param thro 
 */
void thro_led_update(int16_t thro) {
    uint8_t idx = prj_cfg->bar_idx;
    uint8_t step = floorf((prj_cfg->max - prj_cfg->center) / 12.0);
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

/**
 * @brief 
 * 
 * @return int 
 */
int led_init(void) {

    led_chasis_set(OFF);
    led_head_set(0, OFF);
    led_head_set(1, OFF);
    led_tail_set(0, OFF);
    led_tail_set(1, OFF);
    led_fire_set(OFF);

    if (prj_cfg->mode == 1) {
        led_string_set(ON);
    } else {
        led_string_set(OFF);
    }

    return 0;
}
