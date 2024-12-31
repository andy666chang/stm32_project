/*
 * @Author: andy.chang 
 * @Date: 2024-12-31 14:45:02 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-12-31 16:52:24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "shell.h"
#include "shell_sys.h"
#include "shell_led.h"
#include "shell_btn.h"
#include "shell_thro.h"
#include "shell_config.h"

#include "components/log/log.h"

#define TAG "SHELL"

static int shell_help(int argc, char *argv[]);

struct shell_t shell_func_list[] = {
    {"help", "list shell", shell_help, NULL},
    SHELL_SYS,
    SHELL_LED,
    SHELL_BTN,
    SHELL_THRO,
    SHELL_CFG,
    SHELL_END,
};


static void shell_dump(struct shell_t shell_list[]) {
    for (size_t i = 0; shell_list[i].func_name != NULL; i++) {
        if (shell_list[i].sub_shell) {
            LOGI(TAG, "%s: %s", shell_list[i].func_name, shell_list[i].func_info);
            shell_dump(shell_list[i].sub_shell);
            LOGI(TAG, "");
        } else {
            LOGI(TAG, "%s - %s", shell_list[i].func_name, shell_list[i].func_info);
        }
    }
}

static int shell_help(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    shell_dump(shell_func_list);
    return 0;
}

static int execute_shell(int argc, char *argv[], struct shell_t shell_list[]) {
    for (size_t i = 0; shell_list[i].func_name != NULL; i++) {
        if ( !strcasecmp(argv[0], shell_list[i].func_name)) {
            if (shell_list[i].sub_shell && argc >= 1) {
                return execute_shell(argc-1, argv+1, shell_list[i].sub_shell);
            }

            if (shell_list[i].func) {
                return shell_list[i].func(argc-1, argv+1);
            }
        }
    }
    
    return -EINVAL;
}

int parse_shell(uint8_t *shell, uint32_t len) {
    (void) len;
    int ret = 0;

    int argc = 0;
    char *argv[20] = {NULL};

    // parse shell
    argv[argc] = strtok((char *)shell, " ");
    
    while(argv[argc] != NULL) {
        argc++;
        argv[argc] = strtok(NULL, " ");
    }

    // check shell keyword
    ret = execute_shell(argc, argv, shell_func_list);
    switch (ret) {
        case 0:
            break;
        case -EINVAL:
            LOGE(TAG, "Invalid argument");
            break;
        
        default:
            LOGE(TAG, "Unknow: %d", ret);
            break;
    }
    
    return 0;
}
