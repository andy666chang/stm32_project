/*
 * @Author: andy.chang 
 * @Date: 2024-11-25 22:32:13 
 * @Last Modified by:   andy.chang 
 * @Last Modified time: 2024-11-25 22:32:13 
 */

#include "interface.h"

#include "gpio.h"

#include "components/log/log.h"
#define TAG "RC"

#define ON 1
#define OFF 0

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
    en = !en;
    HAL_GPIO_WritePin(CHASIS_GPIO_Port, CHASIS_Pin, en);
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
 * @return int 
 */
int led_init(void) {

    led_chasis_set(OFF);
    led_head_set(0, OFF);
    led_head_set(1, OFF);
    led_tail_set(0, OFF);
    led_tail_set(1, OFF);
    led_fire_set(OFF);
    
    return 0;
}
