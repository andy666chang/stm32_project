/*
 * @Author: andy.chang 
 * @Date: 2024-08-03 01:02:21 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-08-03 01:21:56
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <errno.h>

#include "components/log/log.h"

#define TAG "SHELL"

#define SHELL_VT100_ASCII_ESC		(0x1b)
#define SHELL_VT100_ASCII_DEL		(0x7F)
#define SHELL_VT100_ASCII_BSPACE	(0x08)
#define SHELL_VT100_ASCII_CTRL_A	(0x01)
#define SHELL_VT100_ASCII_CTRL_B	(0x02)
#define SHELL_VT100_ASCII_CTRL_C	(0x03)
#define SHELL_VT100_ASCII_CTRL_D	(0x04)
#define SHELL_VT100_ASCII_CTRL_E	(0x05)
#define SHELL_VT100_ASCII_CTRL_F	(0x06)
#define SHELL_VT100_ASCII_CTRL_K	(0x0B)
#define SHELL_VT100_ASCII_CTRL_L	(0x0C)
#define SHELL_VT100_ASCII_CTRL_N	(0x0E)
#define SHELL_VT100_ASCII_CTRL_P	(0x10)
#define SHELL_VT100_ASCII_CTRL_U	(0x15)
#define SHELL_VT100_ASCII_CTRL_W	(0x17)
#define SHELL_VT100_ASCII_ALT_B		(0x62)
#define SHELL_VT100_ASCII_ALT_F		(0x66)
#define SHELL_VT100_ASCII_ALT_R		(0x72)

#define SHELL_ASCII_MAX_CHAR (127u)

enum
{
    SHELL_RECEIVE_DEFAULT,
    SHELL_RECEIVE_ESC,
    SHELL_RECEIVE_ESC_SEQ,
    SHELL_RECEIVE_TILDE_EXP,
};

/**
 * @brief 
 * 
 * @param data 
 * @return int 
 */
static int ascii_filter(const char data)
{
    return (uint8_t) data > SHELL_ASCII_MAX_CHAR ? -EINVAL : 0;
}

/**
 * @brief 
 * 
 * @param data 
 * @param len 
 */
void shell_process(uint8_t *data, uint16_t len)
{
    static char shell_buf[2][512] = {0};
    static uint16_t idx = 0;
    static uint8_t state = SHELL_RECEIVE_DEFAULT;

    if (data == NULL)
        return;
    
    for (size_t i = 0; i < len; i++)
    {
        // filter ascii
        if (ascii_filter(data[i]) != 0) {
			continue;
		}

        switch (state)
        {
        case SHELL_RECEIVE_DEFAULT:
            if (data[i] == '\r')
            {
                // TODO: history
                // TODO: execute cmd
                char shell_cmd[1024] = {0};

                printf("\33[2K\r");

                sprintf(shell_cmd, "%s%s", shell_buf[0], shell_buf[1]);

                printf("%s%s\r\n", "mcu-cli > ", shell_cmd);
                LOGI(TAG, "shell: %s", shell_cmd);

                memset(shell_buf, 0, sizeof(shell_buf));
                idx = 0;

                continue;
            }

            switch (data[i])
            {
            case SHELL_VT100_ASCII_ESC: /* ESCAPE */
                state = SHELL_RECEIVE_ESC;
                break;

            case '\0':
            case '\t': /* TAB */
                break;

            case SHELL_VT100_ASCII_BSPACE: /* BACKSPACE */
                if(idx==0)
                    continue;

                shell_buf[0][--idx] = 0;
                break;

            case SHELL_VT100_ASCII_DEL: /* DELETE */
                if(!shell_buf[1][0])
                    continue;
                
                memmove(&shell_buf[1][0], &shell_buf[1][1], strlen(&shell_buf[1][0]));
                break;

            default:
                shell_buf[0][idx++] = data[i];
                break;
            }
            break;

        case SHELL_RECEIVE_ESC:
            if (data[i] == '[')
            {
                state = SHELL_RECEIVE_ESC_SEQ;
                break;
            }
            state = SHELL_RECEIVE_DEFAULT;
            break;

        case SHELL_RECEIVE_ESC_SEQ:
            state = SHELL_RECEIVE_DEFAULT;

            switch (data[i])
            {
            case 'A': /* UP arrow */
                LOGI(TAG, "UP arrow");
                break;
            case 'B': /* DOWN arrow */
                LOGI(TAG, "DOWN arrow");
                break;

            case 'C': /* RIGHT arrow */
                if (!strlen(&shell_buf[1][0]))
                    continue;
                
                shell_buf[0][idx++] = shell_buf[1][0];
                memmove(&shell_buf[1][0], &shell_buf[1][1], strlen(&shell_buf[1][0]));
                break;

            case 'D': /* LEFT arrow */
                if (idx == 0)
                    continue;
                
                idx--;
                memmove(&shell_buf[1][1], &shell_buf[1][0], strlen(&shell_buf[1][0]));
                shell_buf[1][0] = shell_buf[0][idx];
                shell_buf[0][idx] = 0;
                break;

            case '3':/* DELETE Button in ESC[n~ mode */
                state = SHELL_RECEIVE_TILDE_EXP;
				if(!shell_buf[1][0])
                    continue;
                
                memmove(&shell_buf[1][0], &shell_buf[1][1], strlen(&shell_buf[1][0]));
				break;
            default:
                break;
            }
            break;

        case SHELL_RECEIVE_TILDE_EXP:
        default:
			state = SHELL_RECEIVE_DEFAULT;
            break;
        }
    }  

    // clear line
    printf("\33[2K\r");
    printf("%s%s%s", "mcu-cli > ", shell_buf[0], shell_buf[1]);
    fflush(stdout);
    for (size_t i = 0; i < strlen(shell_buf[1]); i++)
        printf("\b");
    fflush(stdout);
}


