/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 15:01:03 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2026-01-02 13:12:32
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "components/shell/shell.h"
#include "components/log/log.h"

#include "interfaces/interface.h"

#define TAG "SHELL-LED_BAR"

static int shell_led_bar(int argc, char *argv[]) {

    if (argc < 3) {
        LOGE(TAG, "Invalid args, need 3 args for r,g,b");
        return -EINVAL;
    }

    uint8_t r = 0, g = 0, b = 0;
    r = strtol(argv[0], NULL, 0);
    g = strtol(argv[1], NULL, 0);
    b = strtol(argv[2], NULL, 0);

    LOGI(TAG, "Set led bar color: r = %d, g = %d, b = %d", r, g, b);
    thro_led_set(r, g, b);

    return 0;
}

SHELL_CMD_DEFINE(led_bar) = {
    .name = "led_bar",
    .info = "led_bar <r> <g> <b>",
    .func = shell_led_bar,
    .sub = NULL,
};
