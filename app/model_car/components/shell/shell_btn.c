/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 15:01:03 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-12-31 15:19:55
 */

#include "shell_btn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "services/button/service.h"

#include "components/log/log.h"

#define TAG "SHELL-BTN"

int shell_btn(int argc, char *argv[]) {
    // for (size_t i = 0; i < argc; i++) {
    //     LOGI(TAG, " led sub: %s", argv[i]);
    // }

    int p = 0;

    for (int i = 0; i < argc; i++) {
        if ( !strcmp(argv[i], "-p") && (i+1) < argc) {
            p = strtol(argv[i+1], NULL, 10);
        }
    }

    LOGI(TAG,"press: %d",p );

    for (int i = 0; i < p; i++){
        btn_data_push(100);
        btn_data_push(0);
    }

    return 0;
}
