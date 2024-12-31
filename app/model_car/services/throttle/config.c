/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 10:40:40 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-12-31 11:28:47
 */

#pragma once

#include "config.h"

#include "components/log/log.h"

#define TAG "CFG"

typedef struct cfg_pack_t {
    union pack_t {
        prj_cfg_t cfg;
        uint8_t dummy[252];
    };

    union pack_t pack;
    uint32_t check; // CRC32
} cfg_pack_t;

static const prj_cfg_t default_cfg = {
    .version = 0,
    .dir = 1,
    .centor = 1500,
    .margin = 10,
};

static cfg_pack_t cfg_pack = {.check = 0x55};
prj_cfg_t *prj_cfg = NULL;

void config_init(void) {
    // LOGI(TAG, "size of prj_cfg: %d", sizeof(prj_cfg_t)); // 6
    // LOGI(TAG, "size of cfg_pack: %d", sizeof(cfg_pack_t)); // 256

    // TODO: read config pack from flash

    // TODO: verify data valid with CRC32
    if (cfg_pack.check == 0x55) {
        // Replace by default config
        LOGW(TAG, "Invalid data in flash, reset to default");
        memcpy(&cfg_pack.pack.cfg, &default_cfg, sizeof(prj_cfg_t));
        // TODO: write to flash
    }
    
    // Load to global config
    prj_cfg = &cfg_pack.pack.cfg;

    // Dump config
    LOGI(TAG, "prj_cfg:");
    LOGI(TAG, "  version = %d", prj_cfg->version);
    LOGI(TAG, "  dir = %d", prj_cfg->dir);
    LOGI(TAG, "  centor = %d", prj_cfg->centor);
    LOGI(TAG, "  margin = %d\n", prj_cfg->margin);
}
