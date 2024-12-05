/*
 * @Author: andy.chang 
 * @Date: 2024-12-05 20:26:05 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-12-05 22:40:19
 */

#include "service.h"


#include "usart.h"

#include "components/ring_buf/ring_buf.h"
#include "components/shell/shell.h"
#include "components/log/log.h"

#define TAG "SH"

#define SHELL_TIMEOUT 20

static uint8_t tmp;

static uint16_t shell_buf_data[10];
static struct ring_buf shell_buf;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        ring_buf_push(&shell_buf, (void *)&tmp);
        HAL_UART_Receive_IT(huart, &tmp, 1);
    }
}

static int shell_led(int argc, char *argv[]) {
    // for (size_t i = 0; i < argc; i++) {
    //     LOGI(TAG, " led sub: %s", argv[i]);
    // }

    int n = 0, l = 0;

    for (size_t i = 0; i < argc; i++) {
        if ( !strcmp(argv[i], "-n") && (i+1) < argc) {
            n = strtol(argv[i+1], NULL, 10);
        }

        if ( !strcmp(argv[i], "-l") && (i+1) < argc) {
            l = strtol(argv[i+1], NULL, 10);
        } 
    }

    LOGI(TAG,"n: %d, l:%d",n ,l);

    for (size_t i = 0; i < l; i++){
        btn_data_push(100);
        btn_data_push(0);
    }


    return 0;
}

int parse_shell(uint8_t *shell, uint32_t len) {
    (void) len;

    int argc = 0;
    char *argv[20] = {NULL};

    // parse shell
    argv[argc] = strtok(shell, " ");
    
    while(argv[argc] != NULL) {
        argc++;
        argv[argc] = strtok(NULL, " ");
    }


    // check shell keyword
    if ( !strcasecmp(argv[0], "led")) {
        shell_led(argc-1, argv+1);
    }
    
    return 0;
}


/**
 * @brief 
 * 
 */
void shell_service_process(void) {
    static uint32_t sh_time = 0;

    if ((log_timestamp() - sh_time) >= SHELL_TIMEOUT) {
        if (shell_buf.cnt) {
            uint16_t len = 0;
            uint8_t buf[10] = {0};
            uint8_t *p_buf = buf;
            while (shell_buf.cnt && len < 10) {
                ring_buf_pop(&shell_buf, p_buf);
                p_buf++;
                len++;
            }

            shell_process(buf, len);
        }
        sh_time = log_timestamp();
    }
    
}


/**
 * @brief 
 * 
 * @return int 
 */
int shell_servic_init(void) {
    // Init Ring buffer
    RING_BUF_INIT(shell_buf, shell_buf_data);

    // initial uart
    MX_USART1_UART_Init();
    HAL_UART_Receive_IT(&huart1, &tmp, 1);

    return 0;
}
