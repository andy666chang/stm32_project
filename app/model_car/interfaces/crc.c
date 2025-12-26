/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 10:40:40 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2025-12-26 16:54:29
 */


#include "crc.h"

uint32_t crc_calculate(uint8_t *data, uint32_t len) {
    return HAL_CRC_Calculate(&hcrc, (uint32_t *)data, len);
}
