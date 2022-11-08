#include "vbe.h"

#include "lib/libc.h"
#include "pmm.h"

// Converts Seg:Off address to Absolute address
#define SegOff2Abs(seg, off) ((seg << 4) + off)

struct framebuffer* getFramebuffer()
{
    static struct framebuffer* framebuffer = NULL;
    if (framebuffer == 0)
    {
        framebuffer = pmm_Allocate(sizeof(struct framebuffer));
        if (!vbe_Initialize(framebuffer)) return NULL;
    }

    return framebuffer;
}

bool vbe_Initialize(struct framebuffer* framebuffer)
{
    vbe_info_block_t* vbe_info = pmm_AllocateBelow1M(512);
    if (vbe_get_controller_info(vbe_info) == false || strncmp((char*)&vbe_info->vbe_signature, "VESA", 4))
    {
        //TODO:printf("VESA: Failed to acquire controller info!\n");
        return false;
    }
    
    uint16_t* video_modes = (uint16_t*)SegOff2Abs(vbe_info->video_mode_seg, vbe_info->video_mode_off);
    uint16_t best = 0x13;
    //TODO: Render logo
    //TODO: Find the best video mode
    vbe_mode_info_t* mode_info = pmm_AllocateBelow1M(512);
    for (uint32_t i = 0; video_modes[i] != 0xffff; i++)
    {
        if (vbe_get_mode_info(video_modes[i], mode_info) == false) continue;
        bool has_framebuffer = (mode_info->attributes & 0x90) == 0x90;

        if (has_framebuffer && mode_info->width == 1024 && mode_info->height == 768 && mode_info->bpp == 32) 
        {
            best = video_modes[i];
            framebuffer->base = mode_info->framebuffer;
            framebuffer->width = mode_info->width;
            framebuffer->height = mode_info->height;
            framebuffer->bits_per_pixel = mode_info->bpp;
            framebuffer->pitch = mode_info->pitch;
            break;
        }
    }
    pmm_FreeBelow1M(512);
    
    vbe_set_video_mode(best);
    for (uint32_t y = 0; y < framebuffer->height; y++)
    {
        for (uint32_t x = 0; x < framebuffer->width; x++)
        {
            uint32_t* pixel_offset = (uint32_t*)(y * framebuffer->pitch + (x * (framebuffer->bits_per_pixel / 8)) + framebuffer->base);
            *pixel_offset = 0x00000000;
        }
    }

    return true;
}
