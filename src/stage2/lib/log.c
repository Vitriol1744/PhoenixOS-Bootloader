#include "log.h"

#include "lib/libc.h"
#include "lib/terminal.h"

void log(log_level_t log_level, const char* fmt, ...)
{
    switch (log_level)
    {
        case LOG_LEVEL_INFO: terminal_SetColor(0x0000ffff, 0x00000000); break;
        case LOG_LEVEL_TRACE: terminal_SetColor(0x0000ff00, 0x00000000); break;
        case LOG_LEVEL_WARN: terminal_SetColor(0x00ffff00, 0x00000000); break;
        case LOG_LEVEL_ERROR: terminal_SetColor(0x00ff0000, 0x00000000); break;
        case LOG_LEVEL_FATAL: terminal_SetColor(0x00000000, 0x00ff0000); break;
    }

    printf("[BOOT]: ", 0);
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
    va_end(args);
    printf("\n", 0);
    terminal_SetColor(0x00ffffff, 0x00000000);
}
