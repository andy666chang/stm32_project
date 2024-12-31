/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 15:01:03 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-12-31 16:17:02
 */

#include "shell_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "services/throttle/config.h"

#include "components/log/log.h"

#define TAG "SHELL-CFG"

static int shell_dump(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    // Dump config
    LOGI(TAG, "prj_cfg:");
    LOGI(TAG, "  version = %d", prj_cfg->version);
    LOGI(TAG, "  dir = %d", prj_cfg->dir);
    LOGI(TAG, "  centor = %d", prj_cfg->centor);
    LOGI(TAG, "  margin = %d\n", prj_cfg->margin);
    return 0;
}

static int shell_load(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    load_config();
    return 0;
}

static int shell_save(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    save_config();
    return 0;
}

static int shell_dir(int argc, char *argv[]) {
    (void) argc;

    int8_t dir = strtol(argv[0], NULL, 10);
    prj_cfg->centor = dir;
    LOGI(TAG, "dir = %d", prj_cfg->dir);
    return 0;
}

static int shell_center(int argc, char *argv[]) {
    (void) argc;

    uint16_t num = strtol(argv[0], NULL, 10);
    prj_cfg->centor = num;
    LOGI(TAG, "centor = %d", prj_cfg->centor);
    return 0;
}

static int shell_margin(int argc, char *argv[]) {
    (void) argc;

    uint16_t num = strtol(argv[0], NULL, 10);
    prj_cfg->margin = num;
    LOGI(TAG, "margin = %d", prj_cfg->margin);
    return 0;
}

struct shell_t _cfg_list[] = {
    { "dump", "dump config", shell_dump, NULL},
    { "load", "load config", shell_load, NULL},
    { "save", "save config", shell_save, NULL},
    { "dir" , "dir -1/1", shell_dir, NULL},
    { "center", "center <1500>", shell_center, NULL},
    { "margin", "margin <10>", shell_margin, NULL},
    SHELL_END,
};
