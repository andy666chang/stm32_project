#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "components/log/log.h"

static log_level_t g_log_level = LOG_INFO;

void log_write(log_level_t level, const char *tag, const char *format,
                  ...) {
    (void) tag;
    if (g_log_level >= level) {
        uint32_t len = 0;
        uint8_t buf[128] = {0};

        va_list arg;
        va_start(arg, format);
        len = vsprintf((char *)buf, format, arg);
        va_end(arg);

        // Send to DMA buffer
        extern void tx_data_push(uint8_t *data, uint32_t len);
        tx_data_push(buf, len);
    }
}
