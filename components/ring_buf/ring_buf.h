/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:39:06 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-08-01 01:47:41
 */

#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#ifndef ARRAY_SIZE
  #define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))
#endif

#include <stdio.h>
#include <stdint.h>

/**
 * @brief 
 * 
 */
struct ring_buf {
    void *head;
    void *tail;
    void *data;
    uint16_t len;
    uint16_t cnt;
    uint8_t size;
};

#define RING_BUF_INIT(name, buf)     \
    do                               \
    {                                \
        name.head = name.tail = buf; \
        name.data = buf;             \
        name.len = ARRAY_SIZE(buf);  \
        name.cnt = 0;                \
        name.size = sizeof(buf[0]);  \
    } while (0)

/**
 * @brief 
 * 
 * @param buf 
 * @param pdata 
 * @return int 
 */
int ring_buf_push(struct ring_buf *buf, void *pdata);

/**
 * @brief 
 * 
 * @param buf 
 * @param pdata 
 * @return int 
 */
int ring_buf_pop(struct ring_buf *buf, void *pdata);




#endif // _RING_BUF_H_
