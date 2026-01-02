/*
 * @Author: andy.chang 
 * @Date: 2024-12-05 20:26:05 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2026-01-02 11:49:48
 */

#include "service.h"

#include "usart.h"
#include <errno.h>

#include "interfaces/interface.h"

#include "components/ring_buf/ring_buf.h"
#include "components/shell/shell.h"
#include "components/log/log.h"

#define TAG "SHELL-S"

#define SHELL_TIMEOUT 20

static uint8_t tmp;

static uint16_t shell_buf_data[32];
static struct ring_buf shell_buf;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        ring_buf_push(&shell_buf, (void *)&tmp);
        HAL_UART_Receive_IT(huart, &tmp, 1);
    }
}

/**
 * @brief 
 * 
 */
void shell_service_process(void) {
    static uint32_t sh_time = 0;

    if (WAIT_TIMEOUT(sh_time, SHELL_TIMEOUT)) {
        uint16_t len = 0;
        uint8_t buf[10] = {0};

        for (len = 0; shell_buf.cnt && len < sizeof(buf); len++) {
            ring_buf_pop(&shell_buf, &buf[len]);
        }

        if (len) {
            shell_process(buf, len);
        }

        sh_time = GET_SYS_TIME();
    }
}

/**
 * @brief 
 * 
 * @return int 
 */
int shell_servic_init(void) {
    // Init Ring buffer
    RING_BUF_INIT(shell_buf, shell_buf_data);

    // initial uart irq
    HAL_UART_Receive_IT(&huart1, &tmp, 1);

    return 0;
}
