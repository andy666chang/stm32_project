/*
 * @Author: andy.chang 
 * @Date: 2024-07-31 02:49:33 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-08-01 22:10:30
 */

#ifndef SHELL_SERVICE_H
#define SHELL_SERVICE_H

#include <stdio.h>
#include <stdint.h>

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t shell_service_init(void);

/**
 * @brief 
 * 
 * @param data 
 */
void shell_data_put(uint8_t data);

#endif // SHELL_SERVICE_H
