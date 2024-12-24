/*
 * @Author: andy.chang 
 * @Date: 2024-12-05 20:26:05 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-12-05 23:26:26
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

struct _shell_func_ {
    char *func_name;
    char *func_info;
    int (*func)(int argc, char *argv[]);
    struct _shell_func_ *sub_cli ;
};

static int shell_led(int argc, char *argv[]);
static int shell_btn(int argc, char *argv[]);
static int shell_thro(int argc, char *argv[]);


struct _shell_func_ shell_func_list[] = {
    { "led", NULL, shell_led, NULL},
    { "btn", NULL, shell_btn, NULL},
    { "thro", NULL, shell_thro, NULL},
};


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        ring_buf_push(&shell_buf, (void *)&tmp);
        HAL_UART_Receive_IT(huart, &tmp, 1);
    }
}

static int shell_btn(int argc, char *argv[]) {
    // for (size_t i = 0; i < argc; i++) {
    //     LOGI(TAG, " led sub: %s", argv[i]);
    // }

    int p = 0;

    for (size_t i = 0; i < argc; i++) {
        if ( !strcmp(argv[i], "-p") && (i+1) < argc) {
            p = strtol(argv[i+1], NULL, 10);
        }
    }

    LOGI(TAG,"press: %d",p );

    for (size_t i = 0; i < p; i++){
        btn_data_push(100);
        btn_data_push(0);
    }

    return 0;
}

static int shell_thro(int argc, char *argv[]) {
    // for (size_t i = 0; i < argc; i++) {
    //     LOGI(TAG, " led sub: %s", argv[i]);
    // }

    int thro = 0;

    for (size_t i = 0; i < argc; i++) {
        if ( !strcmp(argv[i], "-v") && (i+1) < argc) {
            thro = strtol(argv[i+1], NULL, 10);
        }
    }

    LOGI(TAG,"thro: %d", thro);
    thro_data_push(thro);

    return 0;
}

static int shell_led(int argc, char *argv[]) {
    // for (size_t i = 0; i < argc; i++) {
    //     LOGI(TAG, " led sub: %s", argv[i]);
    // }

    int idx = 99, level = 99;

    for (size_t i = 0; i < argc; i++) {
        if ( !strcmp(argv[i], "-i") && (i+1) < argc) {
            idx = strtol(argv[i+1], NULL, 10);
            continue;
        }

        if ( !strcmp(argv[i], "-l") && (i+1) < argc) {
            level = (bool)strtol(argv[i+1], NULL, 10);
            continue;
        }
    }

    LOGI(TAG,"idx: %d, level: %d", idx, level);

    switch (idx) {
        case 0:
            LOGI(TAG, "HEAD_0_Pin level: %d", level);
            led_head_set(0, level);
            break;
        
        case 1:
            LOGI(TAG, "HEAD_1_Pin level: %d", level);
            led_head_set(1, level);
            break;

        case 2:
            LOGI(TAG, "CHASIS_Pin level: %d", level);
            led_chasis_set(level);
            break;

        case 3:
            LOGI(TAG, "TAIL_0_Pin level: %d", level);
            led_tail_set(0, level);
            break;

        case 4:
            LOGI(TAG, "TAIL_1_Pin level: %d", level);
            led_tail_set(1, level);
            break;

        case 5:
            LOGI(TAG, "FIRE_Pin level: %d", level);
            led_fire_set(level);
            break;
        
        default:
            LOGE(TAG,"Unknow LED: %d", idx);
            break;
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
    for (size_t i = 0; i < ARRAY_SIZE(shell_func_list); i++){
        if ( !strcasecmp(argv[0], shell_func_list[i].func_name)) {
            if (shell_func_list[i].func) {
                shell_func_list[i].func(argc-1, argv+1);
            }
            break;
        }
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
