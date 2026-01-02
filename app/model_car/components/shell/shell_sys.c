/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 15:01:03 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2026-01-02 13:13:46
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "components/shell/shell.h"
#include "components/log/log.h"

#include "main.h"

#define TAG "SHELL-SYS"

static int shell_reboot(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    HAL_NVIC_SystemReset();
    return 0;
}

static const struct shell_t sys_list[] = {
    { "reboot", "reboot system", shell_reboot, NULL},
    SHELL_END,
};

SHELL_CMD_DEFINE(sys) = {
    .name = "sys",
    .info = "sys func",
    .func = NULL,
    .sub = sys_list,
};
