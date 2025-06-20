/*
 * @Author: andy.chang 
 * @Date: 2025-04-22 00:48:49 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2025-04-24 01:16:37
 */

#pragma once

#ifndef CONTAINER_OF
#ifndef offsetof
#define offsetof(s, m) ((size_t) & (((s *)0)->m))
#endif
#define CONTAINER_OF(ptr, type, field)                                         \
    ((type *)(((char *)(ptr)) - offsetof(type, field)))
#endif

typedef struct event_type {
    const char *name;
} event_type_t;

typedef struct event_header {
    const event_type_t *type;
} event_header_t;

typedef struct event_listener {
    const char *name;
    bool (*notification)(const event_header_t *aeh);
} event_listener_t;

struct event_subscriber {
    const event_listener_t *listener;
    const event_type_t *type;
};

#define APP_EVENT_TYPE_DECLARE(ename)                                          \
    extern const event_type_t ename##_type;                                    \
    static inline void ename##_init(struct ename *evt) {                       \
        evt->header.type = &ename##_type;                                      \
    }                                                                          \
    static inline bool is_##ename(const event_header_t *aeh) {                 \
        return (aeh->type == &ename##_type);                                   \
    }                                                                          \
    static inline struct ename *cast_##ename(const event_header_t *aeh) {      \
        return CONTAINER_OF(aeh, struct ename, header);                        \
    }

#define APP_EVENT_TYPE_DEFINE(ename)                                           \
    const event_type_t ename##_type = {                                        \
        .name = #ename,                                                        \
    }

#define APP_EVENT_SUBMIT(evt)                                                  \
    do {                                                                       \
        struct event_subscriber _event_subscriber_start[];                     \
        struct event_subscriber _event_subscriber_end[];                       \
        for (struct event_subscriber *p = _event_subscriber_start;             \
             p < _event_subscriber_end; p++) {                                 \
            if (p->type == evt.header.type && p->listener) {                   \
                p->listener(&evt.header);                                      \
            }                                                                  \
        }                                                                      \
    } while (0)

#define APP_EVENT_LISTENER(mname, handler_fn)                                  \
    static const event_listener_t _##mname##_event_listener = {                \
        .name = #mname,                                                        \
        .notification = handler_fn,                                            \
    }

#define APP_EVENT_SUBSCRIBE(mname, ename)                                      \
    __attribute__((                                                            \
        used,                                                                  \
        section(".event_subscriber"))) static const struct event_subscriber    \
        _##mname##_event_subscriber = {                                        \
            .listener = &_##mname##_event_listener,                            \
            .type = &ename##_type,                                             \
    }

///////////////////////////////////////////////////
#if 0
struct sample_event {
    event_header_t header;

    int data1;
    int data2;
};

APP_EVENT_TYPE_DECLARE(sample_event);
APP_EVENT_TYPE_DEFINE(sample_event);

// --------------------------------------------------

static bool handler(const event_header_t *aeh) {
    if (is_sample_event(aeh)) {
        struct sample_event *evt = cast_sample_event(aeh);
    };
    return 0;
}

APP_EVENT_LISTENER(module123, handler);
APP_EVENT_SUBSCRIBE(module123, sample_event);
#endif
