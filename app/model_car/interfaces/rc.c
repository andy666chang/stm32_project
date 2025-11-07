/*
 * @Author: andy.chang 
 * @Date: 2024-11-24 02:24:28 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-12-05 22:48:46
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"
#include "tim.h"

#include "services/button/service.h"
#include "services/throttle/service.h"
#include "components/log/log.h"
#define TAG "RC"

static uint16_t time[2] = {0};

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){
    // Record Rising time
    switch (GPIO_Pin)
    {
    case GPIO_PIN_3:
        time[0] = __HAL_TIM_GET_COUNTER(&htim1);
        break;
    
    case GPIO_PIN_12:
        time[1] = __HAL_TIM_GET_COUNTER(&htim1);
        break;
    
    default:
        LOGE(TAG, "Unknow GPIO EXTI: %d", GPIO_Pin);
        break;
    }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    int32_t diff = __HAL_TIM_GET_COUNTER(&htim1);
    uint16_t *time_ptr = NULL;
    void (*func)(uint16_t) = NULL;

    LOGD(TAG, "diff = %d", diff);

    // Measure the pulse time
    switch (GPIO_Pin) {
    case GPIO_PIN_3:
        time_ptr = &time[0];
        func = btn_data_push;
        break;

    case GPIO_PIN_12:
        time_ptr = &time[1];
        func = thro_data_push;
        break;

    default:
        LOGE(TAG, "Unknow GPIO EXTI: %d", GPIO_Pin);
        break;
    }

    if (time_ptr && func) {
        diff -= *time_ptr;
        if (diff < 0)
            diff += 0xffff;
        func(diff);
    }
}

/**
 * @brief 
 * 
 * @return int 
 */
int rc_init(void) {
    // Start & Reset timer
    MX_TIM1_Init();
    HAL_TIM_Base_Start(&htim1);

    // Init GPIO IRQ
    HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
    
    return 0;
}
