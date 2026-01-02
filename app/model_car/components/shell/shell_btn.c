/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 15:01:03 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2026-01-02 13:08:58
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "components/shell/shell.h"
#include "components/log/log.h"

#include "services/button/service.h"

#define TAG "SHELL-BTN"

static int shell_btn(int argc, char *argv[]) {
    int p = 0;

    for (int i = 0; i < argc; i++) {
        if ( !strcmp(argv[i], "-p") && (i+1) < argc) {
            p = strtol(argv[i+1], NULL, 10);
        }
    }

    LOGI(TAG,"press: %d",p );

    for (int i = 0; i < p; i++){
        static uint16_t btn = 2000;
        
        btn_data_push(btn);

        btn = (btn == 2000)? 1000:2000;
    }

    return 0;
}

SHELL_CMD_DEFINE(btn) = {
    .name = "btn",
    .info = "btn -p <1...9>",
    .func = shell_btn,
    .sub = NULL,
};
