/*
 * @Author: andy.chang 
 * @Date: 2024-11-25 22:32:13 
 * @Last Modified by:   andy.chang 
 * @Last Modified time: 2024-11-25 22:32:13 
 */

#include "interface.h"
#include "../services/throttle/config.h"
#include "gpio.h"

#include "components/log/log.h"
#define TAG "LED"

#define ON 1
#define OFF 0

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// #define SK6812
#define WS2812B

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
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP();
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
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();

    // T1L: 0.45 +- 0.15 us
    LL_GPIO_ResetOutputPin(CHASIS_GPIO_Port, CHASIS_Pin);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
#endif
}

/**
 * @brief 
 * 
 */
static void led_reset(void) {
    // Trst: Low level > 80us
    HAL_GPIO_WritePin(CHASIS_GPIO_Port, CHASIS_Pin, GPIO_PIN_RESET);
    for (size_t i = 0; i < 600; i++) {
        __NOP();
    }
}

/**
 * @brief 
 * 
 * @param data 
 */
static void led_data_set(uint32_t data) {
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
    // const uint8_t led_map_ref[12][3] = {
    //     {50, 0, 0}, {0, 50, 0}, {0, 0, 100}, {50, 50, 50}, {50, 0, 0}, {0, 50, 0},
    //     {0, 0, 100}, {50, 50, 50}, {50, 0, 0}, {0, 50, 0}, {0, 0, 100}, {50, 50, 50},
    // };
    // const uint8_t led_map_ref[12][3] = {
    //     {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {85, 85, 85}, {255, 0, 0}, {0, 255, 0}, 
    //     {0, 0, 255}, {85, 85, 85}, {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {85, 85, 85}, 
    // };
    const uint8_t led_map_ref[12][3] = {
        {0, 255, 0},  {22, 233, 0},  {45, 210, 0},  {68, 187, 0},
        {90, 165, 0}, {113, 142, 0}, {136, 119, 0}, {158, 97, 0},
        {181, 74, 0}, {204, 51, 0},  {227, 28, 0},  {255, 0, 0},
    };

    uint8_t led_map[3];
    thro = MAX(0, thro);

    uint8_t on = MIN((thro / 40), 12); // (500/12) = 41.66
    uint8_t off = 12 - on;

    // on = 12;
    // off = 0;
    LOGD(TAG, "thro: %d, on: %d, off: %d", thro, on, off);

    for (size_t i = 0; i < on; i++) {
        memcpy(led_map, led_map_ref[i], sizeof(led_map));
        led_color_set(led_map[0], led_map[1], led_map[2]);
    }

    memset(led_map, 0, sizeof(led_map));
    for (size_t i = 0; i < off; i++) {
        led_color_set(led_map[0], led_map[1], led_map[2]);
    }

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
