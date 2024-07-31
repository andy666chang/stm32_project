/*
 * @Author: andy.chang 
 * @Date: 2024-08-01 00:39:08 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-08-01 01:58:29
 */

#include <string.h>
#include "ring_buf.h"

// uint8_t sys_buf[100];
// struct ring_buf sys;
// RING_BUF_INIT(sys, sys_buf);

int ring_buf_push(struct ring_buf *buf, void *pdata)
{
    uint8_t *tail = buf->tail;
    uint8_t *data = buf->data;
    uint16_t addr_len = buf->size * buf->len;

    memcpy(tail, pdata, buf->size);

    tail += buf->size;

    if (tail >= (data + addr_len))
    {
        tail = data;
    }

    buf->tail = tail;
    buf->cnt++;

    return 0;
}

int ring_buf_pop(struct ring_buf *buf, void *pdata)
{
    uint8_t *head = buf->head;
    uint8_t *data = buf->data;
    uint16_t addr_len = buf->size * buf->len;
    
    memcpy(pdata, head, buf->size);

    head += buf->size;

    if (head >= (data + addr_len))
    {
        head = data;
    }

    buf->head = head;
    buf->cnt--;

    return 0;
}
