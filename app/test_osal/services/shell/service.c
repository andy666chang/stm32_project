/*
 * @Author: andy.chang 
 * @Date: 2024-08-02 01:05:45 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-08-02 03:17:34
 */

#include <string.h>

#include "osal.h"
#include "osal_tasks.h"
#include "osal_msg.h"

#include "service.h"

#include "components/log/log.h"

#define TAG "SHELL_SERIVCE"


static void shell_process(uint8_t *data, uint16_t len)
{
    static uint8_t shell_buf[1024] = {0};
    static uint16_t idx = 0;

    for (size_t i = 0; i < len; i++)
    {
        if (data[i] != '\r')
        {
            shell_buf[idx++] = data[i];

        } else {
            // clear line
            printf("\33[2K\r");
            printf("%s%s\r\n", "mcu-cli > ", shell_buf);
            memset(shell_buf, 0, sizeof(shell_buf));
            idx = 0;
            return;
        }
    }

    // clear line
    printf("\33[2K\r");
    printf("%s%s", "mcu-cli > ", shell_buf);
    fflush(stdout);
}

/**
 * @brief 
 * 
 * @param id 
 * @param events 
 * @return uint16_t 
 */
static uint16_t shell_task(uint8_t id, uint16_t events)
{
    if (events & SYS_EVENT_MSG)
    {
        uint8_t *data = osal_msg_receive(id);
#if 0
        uint8_t len = data[0];
        memmove(&data[0], &data[1], len);
        data[len] = '\0';
        
        LOGI(TAG, "message : %s", data);
#else
        shell_process(&data[1], data[0]);
#endif

        osal_msg_deallocate(data);
    }

    return 0;
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t shell_service_init(void)
{
    osal_task_create(SHELL_TASK_ID, shell_task);
    return 0;
}
