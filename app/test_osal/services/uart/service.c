/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:31:12 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-08-02 01:03:09
 */

#include <string.h>

#include "osal.h"
#include "osal_tasks.h"
#include "osal_timers.h"

#include "ring_buf.h"

#include "usart.h"

#include "components/log/log.h"

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

    static uint8_t buff[1024] = {0};
    static uint16_t len = 0;

    if (events & UART_DATA_EVENT)
    {
        while (uart_buf.cnt)
        {
            uint8_t data;
            ring_buf_pop(&uart_buf, &data);

            buff[len++] = data;

            if (data == '\r')
            {
                buff[--len] = '\0';
                LOGI(TAG, "%s", buff);
                memset(buff, 0, sizeof(buff));
                len = 0;
            }
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
