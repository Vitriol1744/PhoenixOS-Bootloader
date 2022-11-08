#ifndef PHOENIXOS_BOOTLOADER__IO_H
#define PHOENIXOS_BOOTLOADER__IO_H

#include <stdint.h>

/**
 * @brief Outputs byte through specified port
 * @param port Port to send data through
 * @param value Byte to be sent
 */
static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile("out dx, al" : : "a"(value), "d"(port));
}
static inline void outw(uint16_t port, uint16_t value)
{
    __asm__ volatile("out dx, al" : : "a"(value), "d"(port));
}
/**
 * @brief Returns byte received through specified port
 * @param port Port to get data from
 * 
 * @return Byte received
 */
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret = 0;
    __asm__ volatile("in al, dx" : "=a"(ret) : "d"(port));
    
    return ret;
}
#endif // PHOENIXOS_BOOTLOADER__IO_H
