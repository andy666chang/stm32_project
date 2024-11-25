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

/**
 * @brief 
 * 
 * @param id 
 * @param en 
 */
void led_head_set(uint8_t id, bool en) {
    (void) id;
    (void) en;
}

/**
 * @brief 
 * 
 * @param id 
 * @return true 
 * @return false 
 */
bool led_head_get(uint8_t id) {
    (void) id;

    return 0;
}

/**
 * @brief 
 * 
 * @param id 
 * @param en 
 */
void led_tail_set(uint8_t id, bool en) {
    (void) id;
    (void) en;
}

/**
 * @brief 
 * 
 * @param en 
 * @return true 
 * @return false 
 */
bool led_tail_get(uint8_t id) {
    (void) id;

    return 0;
}

/**
 * @brief 
 * 
 * @param en 
 */
void led_chasis_set(bool en) {
    (void) en;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool led_chasis_get(void) {
    return 0;
}

/**
 * @brief 
 * 
 * @param en 
 */
void led_fire_set(bool en) {
    (void) en;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool led_fire_get(void) {
    return 0;
}

/**
 * @brief 
 * 
 * @return int 
 */
int led_init(void) {
    
    return 0;
}
