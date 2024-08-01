/*
 * @Author: andy.chang 
 * @Date: 2024-07-31 02:49:33 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-08-02 01:26:39
 */

#ifndef SHELL_SERVICE_H
#define SHELL_SERVICE_H

#include <stdio.h>
#include <stdint.h>

#define SHELL_TASK_ID (0x03)

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t shell_service_init(void);

#endif // SHELL_SERVICE_H
