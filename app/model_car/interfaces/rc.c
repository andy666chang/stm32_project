/*
 * @Author: andy.chang 
 * @Date: 2024-11-24 02:24:28 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-11-24 03:14:28
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"
#include "tim.h"

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
        break;
    }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    int32_t diff = __HAL_TIM_GET_COUNTER(&htim1);

    // Measure the pulse time
    // TODO: send signal
    switch (GPIO_Pin)
    {
    case GPIO_PIN_3:
        diff -= time[0];
        if (diff < 0)
            diff += 0xffff;
        btn_data_push(diff);
        break;

    case GPIO_PIN_12:
        diff -= time[1];
        if (diff < 0)
            diff += 0xffff;
        
        break;

    default:
        break;
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

    // Init GPIO IRQ
    HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
    
    return 0;
}
