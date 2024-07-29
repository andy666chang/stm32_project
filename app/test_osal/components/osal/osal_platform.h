#ifndef _OSAL_PLATFORM_H_
#define _OSAL_PLATFORM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "stm32f1xx_hal.h"

#define MAXMEMHEAP  (8*1024)

#define halDataAlign_t uint32_t

#define halIntState_t bool
#define HAL_ENTER_CRITICAL_SECTION(x) do { (void)x; __disable_irq(); } while(0)
#define HAL_EXIT_CRITICAL_SECTION(x) do { (void)x; __enable_irq(); } while(0)

#define HAL_ENABLE_INTERRUPTS __enable_irq
#define HAL_DISABLE_INTERRUPTS __disable_irq

#define HAL_ASSERT assert_param

#define platform_rand rand

#endif // _OSAL_PLATFORM_H_
