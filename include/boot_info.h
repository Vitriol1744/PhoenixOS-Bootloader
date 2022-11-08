#ifndef PHOENIXOS_BOOT__BOOT_INFO_H
#define PHOENIXOS_BOOT__BOOT_INFO_H

#include <stdint.h>

typedef struct
{
    uint64_t base;
    uint32_t width;
    uint32_t height;
    uint32_t bits_per_pixel;
    uint32_t pitch;
} __attribute__((packed)) framebuffer_t;

typedef struct
{
    uint64_t framebuffer;
    uint64_t font_data;
} __attribute__((packed)) boot_info_t;
#endif // PHOENIXOS_BOOT__BOOT_INFO_H
