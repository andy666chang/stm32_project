/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:31:12 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-08-02 01:54:16
 */

#include <string.h>

#include "osal.h"
#include "osal_tasks.h"
#include "osal_timers.h"
#include "osal_msg.h"

#include "ring_buf.h"

#include "usart.h"

#include "components/log/log.h"

#include "services/shell/service.h"

#define TAG "UART_SERIVCE"

#define UART_TASK_ID 2
#define UART_DATA_EVENT 0x01

static uint8_t tmp;

static uint8_t uart_buf_data[256];
static struct ring_buf uart_buf;

void uart_data_put(uint8_t data)
{
    ring_buf_push(&uart_buf, &data);
    osal_start_timerEx(UART_TASK_ID, UART_DATA_EVENT, 10);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        uart_data_put(tmp);
        HAL_UART_Receive_IT(huart, &tmp, 1);
    }
}

/**
 * @brief 
 * 
 * @param id 
 * @param events 
 * @return uint16_t 
 */
static uint16_t uart_task(uint8_t id, uint16_t events)
{
    (void)id;

    if (events & UART_DATA_EVENT)
    {
        // check buf cnt
        if (uart_buf.cnt) {
            // Create message
            uint8_t *msg = osal_msg_allocate(uart_buf.cnt+1);

            // Message len
            msg[0] = uart_buf.cnt;

            // put message data
            for (size_t i = 1; uart_buf.cnt; i++)
            {
                ring_buf_pop(&uart_buf, &msg[i]);
            }

            // send message
            osal_msg_send(SHELL_TASK_ID ,msg);
        }
    }

    return 0;
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t uart_service_init(void)
{
    RING_BUF_INIT(uart_buf, uart_buf_data);
    osal_task_create(UART_TASK_ID, uart_task);

    // initial uart
    MX_USART1_UART_Init();
    HAL_UART_Receive_IT(&huart1, &tmp, 1);

    return 0;
}
