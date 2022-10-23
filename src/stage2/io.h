#ifndef PHOENIXOS_BOOT_IO_H
#define PHOENIXOS_BOOT_IO_H

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t value)
{
    asm volatile("out dx, al" : : "a"(value), "d"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret = 0;
    asm volatile("in al, dx" : "=a"(ret) : "d"(port));
    
    return ret;
}

#endif // PHOENIXOS_BOOT_IO_H