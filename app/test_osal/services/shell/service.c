/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:31:12 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-08-01 23:48:58
 */

#include "osal.h"
#include "osal_tasks.h"
#include "osal_timers.h"

#include "ring_buf.h"

#include "usart.h"

#define SHELL_TASK_ID 2
#define SHELL_DATA_EVENT 0x01

static uint8_t uart_buf;

static uint8_t shell_buf_data[256];
static struct ring_buf shell_buf;

void shell_data_put(uint8_t data)
{
    ring_buf_push(&shell_buf, &data);
    osal_start_timerEx(SHELL_TASK_ID, SHELL_DATA_EVENT, 10);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        shell_data_put(uart_buf);
        HAL_UART_Receive_IT(huart, &uart_buf, 1);
    }
}

/**
 * @brief 
 * 
 * @param id 
 * @param events 
 * @return uint16_t 
 */
static uint16_t shell_task(uint8_t id, uint16_t events)
{
    (void)id;

    if (events & SHELL_DATA_EVENT)
    {
        while (shell_buf.cnt)
        {
            uint8_t data;
            ring_buf_pop(&shell_buf, &data);
            HAL_UART_Transmit(&huart1, &data, sizeof(data), 50);

            if (data == '\r')
            {
                HAL_UART_Transmit(&huart1, "\n", sizeof("\n"), 50);
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
uint8_t shell_service_init(void)
{
    RING_BUF_INIT(shell_buf, shell_buf_data);
    osal_task_create(SHELL_TASK_ID, shell_task);

    // initial uart
    MX_USART1_UART_Init();
    HAL_UART_Receive_IT(&huart1, &uart_buf, 1);

    return 0;
}
