/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 10:40:40 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-12-31 16:07:33
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct prj_cfg_t {
    uint8_t version;
    
    int8_t dir;
    uint16_t centor;
    uint16_t margin;
} prj_cfg_t;

extern prj_cfg_t *prj_cfg;

void config_init(void);
void load_config(void);
void save_config(void);
