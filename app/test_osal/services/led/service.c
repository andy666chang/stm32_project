/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:31:12 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-08-02 01:27:51
 */

#include "osal.h"
#include "osal_tasks.h"
#include "osal_timers.h"

#include "ring_buf.h"

#include "main.h"
#include "gpio.h"

#define LED_TASK_ID 1
#define LED_TIME_EVENT 0x01
#define LED_TIMEOUT_MS 500

/**
 * @brief 
 * 
 * @param id 
 * @param events 
 * @return uint16_t 
 */
static uint16_t led_task(uint8_t id, uint16_t events){
  (void) id;
  (void) events;

  switch(events) {
    case LED_TIME_EVENT:
      // LED blink 
      HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
      break;
    default:
      break;
  }

  return 0;
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t led_service_init(void){

  osal_task_create(LED_TASK_ID, led_task);
  osal_start_reload_timer(LED_TASK_ID, LED_TIME_EVENT, LED_TIMEOUT_MS);

  return 0;
}
