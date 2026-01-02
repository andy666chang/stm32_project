/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 15:01:03 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2026-01-02 13:18:14
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "components/shell/shell.h"
#include "components/log/log.h"

#include "services/throttle/service.h"

#define TAG "SHELL-THRO"

static int shell_thro(int argc, char *argv[]) {
    int thro = 0;

    for (int i = 0; i < argc; i++) {
        if ( !strcmp(argv[i], "-v") && (i+1) < argc) {
            thro = strtol(argv[i+1], NULL, 10);
        }
    }

    LOGI(TAG,"thro: %d", thro);
    thro_data_push(thro);

    return 0;
}

SHELL_CMD_DEFINE(thro) = {
    .name = "thro",
    .info = "thro -v <1000...2000>",
    .func = shell_thro,
    .sub = NULL,
};
