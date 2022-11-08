#ifndef PHOENIXOS_BOOTLOADER__LOG_H
#define PHOENIXOS_BOOTLOADER__LOG_H

#include <stdarg.h>

typedef enum
{
    LOG_LEVEL_INFO,
    LOG_LEVEL_TRACE,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
} log_level_t;

void log(log_level_t log_level, const char* fmt, ...);

#ifdef PH_DEBUG
    #define LogInfo(...)   log(LOG_LEVEL_INFO , __VA_ARGS__)
    #define LogTrace(...)  log(LOG_LEVEL_TRACE, __VA_ARGS__)
    #define LogWarn(...)   log(LOG_LEVEL_WARN,  __VA_ARGS__)
    #define LogError(...)  log(LOG_LEVEL_ERROR, __VA_ARGS__)
    #define LogFatal(...)  log(LOG_LEVEL_FATAL, __VA_ARGS__)
#else
    #define LogInfo(...)   
    #define LogTrace(...)  
    #define LogWarn(...)   
    #define LogError(...)  
    #define LogFatal(...)  
#endif

#endif // PHOENIXOS_BOOTLOADER__LOG_H
