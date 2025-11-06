/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 10:40:40 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2025-01-02 16:36:56
 */

#include "config.h"

#include "interfaces/interface.h"

#include "components/log/log.h"

#define TAG "CFG"

#define CFG_ADDR 0x0800F000
#define MAGIC 0xA1234567

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
    .center = 1500,
    .margin = 10,
    .mode = 0,
};

static cfg_pack_t cfg_pack = {.check = 0x55};
prj_cfg_t *prj_cfg = NULL;

void config_init(void) {
    // LOGI(TAG, "size of prj_cfg: %d", sizeof(prj_cfg_t)); // 6
    // LOGI(TAG, "size of cfg_pack: %d", sizeof(cfg_pack_t)); // 256

    // Read config pack from flash
    load_config();
    LOGI(TAG, "cfg_pack.check: 0x%08X", cfg_pack.check);

    // TODO: verify data valid with CRC32
    if (cfg_pack.check != MAGIC) {
        // Replace by default config
        LOGW(TAG, "Invalid data in flash, reset to default");
        memset(&cfg_pack, 0, sizeof(cfg_pack_t));
        memcpy(&cfg_pack.pack.cfg, &default_cfg, sizeof(prj_cfg_t));
        cfg_pack.check = MAGIC;

        // Erase and write to flash
        save_config();
    }
    
    // Load to global config
    prj_cfg = &cfg_pack.pack.cfg;

    // Dump config
    LOGI(TAG, "prj_cfg:");
    LOGI(TAG, "  version = %d", prj_cfg->version);
    LOGI(TAG, "  dir = %d", prj_cfg->dir);
    LOGI(TAG, "  center = %d", prj_cfg->center);
    LOGI(TAG, "  margin = %d", prj_cfg->margin);
    LOGI(TAG, "  mode = %d\n", prj_cfg->mode);
}

void load_config(void) {
    flash_read(CFG_ADDR, (uint8_t *)&cfg_pack, sizeof(cfg_pack_t));
}

void save_config(void) {
    // TODO: Caculate CRC32
    cfg_pack.check = MAGIC;

    // Erase and write to flash
    flash_erase(CFG_ADDR, sizeof(cfg_pack_t));
    flash_write(CFG_ADDR, (uint8_t *)&cfg_pack, sizeof(cfg_pack_t));
}
