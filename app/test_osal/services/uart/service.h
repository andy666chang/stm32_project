/*
 * @Author: andy.chang 
 * @Date: 2024-07-31 02:49:33 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-08-02 00:59:43
 */

#ifndef UART_SERVICE_H
#define UART_SERVICE_H

#include <stdio.h>
#include <stdint.h>

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t uart_service_init(void);

/**
 * @brief 
 * 
 * @param data 
 */
void uart_data_put(uint8_t data);

#endif // UART_SERVICE_H
