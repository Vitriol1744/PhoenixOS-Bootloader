#ifndef PHOENIXOS_BOOTLOADER__FRAMEBUFFER_H
#define PHOENIXOS_BOOTLOADER__FRAMEBUFFER_H

#include <stdbool.h>
#include <stdint.h>

struct framebuffer
{
    uint64_t base;
    uint32_t width;
    uint32_t height;
    uint32_t bits_per_pixel;
    uint32_t pitch;
} __attribute__((packed));

/**
 * @brief Gets screen framebuffer
 * 
 * @return Pointer to structure receiving framebuffer info
 */
struct framebuffer* getFramebuffer();

#endif // PHOENIXOS_BOOTLOADER__FRAMEBUFFER_H
