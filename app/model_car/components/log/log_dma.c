/*
 * @Author: andy.chang 
 * @Date: 2025-01-02 02:05:07 
 * @Last Modified by:   andy.chang 
 * @Last Modified time: 2025-01-02 02:05:07 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"

#include "components/ring_buf/ring_buf.h"

extern UART_HandleTypeDef huart1;

static uint8_t tx_buf_data[512];
static struct ring_buf tx_buf = {
    .head = tx_buf_data,
    .tail = tx_buf_data,
    .data = tx_buf_data,
    .len = ARRAY_SIZE(tx_buf_data),
    .cnt = 0,
    .size = sizeof(tx_buf_data[0]),
};

static uint8_t dma_buf[64] = {0};
static volatile bool dma_tx_done = true;

static void tx_dma_tranfer(void);

void tx_data_push(uint8_t *data, uint32_t len) {
    // Push data to ring buffer
    for (uint32_t i = 0; i < len; i++) {
        while (tx_buf.cnt >= tx_buf.len){
            HAL_Delay(5);
        }
        
        ring_buf_push(&tx_buf, (void *)data);
        data++;
    }

    if (dma_tx_done) {
        tx_dma_tranfer();
    }
}

/**
  * @brief  Tx Transfer completed callback
  * @param  huart: UART handle.
  * @note   This example shows a simple way to report end of DMA Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    (void) huart;

    dma_tx_done = true;

    // Check Tx buffer is empty
    if(tx_buf.cnt) {
        tx_dma_tranfer();
    }
}

static void tx_dma_tranfer(void) {
    uint16_t cnt = 0;
    memset(dma_buf, 0, sizeof(dma_buf));

    // Pop data from ring buffer
    for (cnt = 0; cnt < sizeof(dma_buf) && tx_buf.cnt; cnt++){
        ring_buf_pop(&tx_buf, (void *)&dma_buf[cnt]);
    }

    // Transmit data
    HAL_UART_Transmit_DMA(&huart1, dma_buf, cnt);
    dma_tx_done = false;
}
