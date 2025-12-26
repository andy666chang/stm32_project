/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 10:40:40 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2025-01-02 16:36:56
 */

#include "config.h"

#include "interfaces/interface.h"

#include "components/log/log.h"
#include "crc.h"
#define TAG "CFG"

#define CFG_ADDR 0x0800F000
#define MAGIC 0xA1234567

typedef struct cfg_pack_t {
    uint32_t header;

    union pack_t {
        prj_cfg_t cfg;
        uint8_t dummy[248];
    } pack;

    uint32_t crc32;
} cfg_pack_t;

static const prj_cfg_t default_cfg = {
    .version = 1,
    .dir = 1,
    .center = 1500,
    .margin = 10,
    .max = 2000,
    
    .mode = 0,
    .bar_idx = 0,
};

static cfg_pack_t cfg_pack;
prj_cfg_t *prj_cfg = &cfg_pack.pack.cfg;

void config_init(void) {
    LOGD(TAG, "size of prj_cfg: %d", sizeof(prj_cfg_t)); // 10
    LOGD(TAG, "size of cfg_pack: %d", sizeof(cfg_pack_t)); // 256

    // Read config pack from flash
    load_config();
    LOGI(TAG, "cfg_pack.header: 0x%08X", cfg_pack.header);
    LOGI(TAG, "cfg_pack.crc32: 0x%08X", cfg_pack.crc32);

    // Verify data valid with CRC32
    uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&cfg_pack.pack,
                                         sizeof(cfg_pack.pack));
    if (cfg_pack.crc32 != crc || cfg_pack.header != MAGIC) {
        // Replace by default config
        LOGW(TAG, "Invalid data in flash, reset to default");
        memset(&cfg_pack, 0, sizeof(cfg_pack_t));
        memcpy(&cfg_pack.pack.cfg, &default_cfg, sizeof(prj_cfg_t));
        cfg_pack.crc32 = MAGIC;

        // Erase and write to flash
        save_config();
    }

    // Dump config
    dump_config();
}

void load_config(void) {
    flash_read(CFG_ADDR, (uint8_t *)&cfg_pack, sizeof(cfg_pack_t));
}

void save_config(void) {
    cfg_pack.header = MAGIC;

    // Calculate CRC32
    cfg_pack.crc32 = HAL_CRC_Calculate(&hcrc, (uint32_t *)&cfg_pack.pack,
                                         sizeof(cfg_pack.pack));

    // Erase and write to flash
    flash_erase(CFG_ADDR, sizeof(cfg_pack_t));
    flash_write(CFG_ADDR, (uint8_t *)&cfg_pack, sizeof(cfg_pack_t));
}

void dump_config(void) {
    LOGI(TAG, "prj_cfg:");
    LOGI(TAG, "  version = %d", prj_cfg->version);
    LOGI(TAG, "  dir = %d", prj_cfg->dir);
    LOGI(TAG, "  center = %d", prj_cfg->center);
    LOGI(TAG, "  margin = %d", prj_cfg->margin);
    LOGI(TAG, "  max = %d", prj_cfg->max);
    LOGI(TAG, "  mode = %d", prj_cfg->mode);
    LOGI(TAG, "  bar_idx = %d", prj_cfg->bar_idx);
}
