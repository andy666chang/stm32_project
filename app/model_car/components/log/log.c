#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "components/log/log.h"

static const char *log_level_name[] = {
    "none", "error", "warn", "info", "debug", "verbose",
};
static log_system_ms log_system_cb;
static log_level_t g_log_level = LOG_INFO;

void log_route(log_system_ms system_ms) { log_system_cb = system_ms; }

uint32_t log_timestamp() {
    if (log_system_cb != NULL)
        return log_system_cb();
    else
        return 0;
}

void log_write(log_level_t level, const char *tag, const char *format,
                  ...) {
    (void) tag;
    if (g_log_level >= level) {
        uint32_t len = 0;
        uint8_t buf[128] = {0};

        va_list arg;
        va_start(arg, format);
        len = vsprintf(buf, format, arg);
        va_end(arg);

        // Send to DMA buffer
        extern void tx_data_push(uint8_t *data, uint32_t len);
        tx_data_push(buf, len);
    }
}

void log_set_level(log_level_t level) { g_log_level = level; }

log_level_t log_get_level(void) { return g_log_level; }

const char *log_get_log_level_name(log_level_t level) {
    return log_level_name[level];
}
