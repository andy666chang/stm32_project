/*
 * @Author: andy.chang 
 * @Date: 2025-04-20 01:14:50 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2025-04-21 02:59:46
 */
#pragma once

#define SERVICE_SECTION_FOREACH(_iterator)                                     \
    for (service_t *_iterator = (void *)_start_service;                        \
         (void *)_iterator < (void *)_stop_service; _iterator++)

typedef struct service_t {
    int (*init)(void);
    void (*process)(void);
} service_t;

extern service_t _start_service[];
extern service_t _stop_service[];
