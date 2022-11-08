#ifndef PHOENIXOS_BOOTLOADER__PSF_H
#define PHOENIXOS_BOOTLOADER__PSF_H

#include "libc.h"

#define PSF2_MAGIC0             0x72
#define PSF2_MAGIC1             0xb5
#define PSF2_MAGIC2             0x4a
#define PSF2_MAGIC3             0x86

#define PSF2_HAS_UNICODE_TABLE  0x01

#define PSF2_MAXVERSION         0

#define PSF2_SEPARATOR          0xFF
#define PSF2_STARTSEQ           0xFE

typedef struct
{
    uint32_t magic;
    uint32_t version;
    uint32_t header_size;
    uint32_t flags;
    uint32_t glyph_count;
    uint32_t bytes_per_glyph;
    uint32_t height, width;
} psf2_font_t;
#endif // PHOENIXOS_BOOTLOADER__PSF_H
