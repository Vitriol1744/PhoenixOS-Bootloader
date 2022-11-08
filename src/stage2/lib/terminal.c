#include "terminal.h"

#include "filesystem/file.h"

#include "lib/libc.h"
#include "lib/log.h"
#include "lib/psf.h"

#include "pmm.h"

#include "io.h"

static struct framebuffer* framebuffer;
uint8_t* font_data;
static uint32_t x = 0;
static uint32_t y = 0;
static uint32_t foreground_color = 0x00ffffff;
static uint32_t background_color = 0x00000000;

#define PrintCharE9(c) outb(0xe9, c)

bool initialized = false;
void putpixel(uint32_t pixel, uint32_t _x, uint32_t _y)
{
    uint32_t* pixel_offset = (uint32_t*)(_y * framebuffer->pitch + (_x * (framebuffer->bits_per_pixel / 8)) + framebuffer->base);
    *pixel_offset = pixel;
}

void terminal_Initialize(struct framebuffer* _framebuffer)
{
    framebuffer = _framebuffer;

    partition_t part;
    partition_Initialize(&part, disk_GetBootDisk(), 0);

    file_handle_t font_file;
    if (fopen(&font_file, &part, "font.psf") == 0) LogError("Failed to open font.psf!");
    font_data = (uint8_t*)pmm_Allocate((size_t)font_file.size);
    fread(&font_file, font_data, font_file.size);

    initialized = true;
}

void terminal_PrintString(const char* str, size_t length)
{
    for (; length > 0; length--)
    {
        terminal_PrintChar(*str);
        str++;
    }
}
void terminal_PrintChar(uint32_t c)
{
    PrintCharE9(c);
    if (!initialized) return;
    psf2_font_t* font = (psf2_font_t*)font_data;
            
    uint32_t characters_per_line = framebuffer->width / (font->width + 2);
    uint32_t characters_per_row = framebuffer->height / (font->height + 2);
    
    switch (c)
    {
        case '\n': 
        {
            if (++y >= characters_per_row) y = 0;
            break;
        }
        case '\r': x = 0; break;

        default: 
        {
            uint32_t bytesperline = (font->width + 7) / 8;
    
            uint8_t* glyph = (uint8_t*)font_data + font->header_size + (c > 0 && c < font->glyph_count ? c : 0) * font->bytes_per_glyph;
    
            uint32_t ypos = y * font->height;
            uint32_t xpos = x * font->width + 1;
            for (uint32_t _y = ypos; _y < font->height + ypos; _y++)
            {
                uint32_t mask = 1 << (font->width - 1);
                for (uint32_t _x = xpos; _x < font->width + xpos; _x++)
                {
                    putpixel(*((uint32_t*)glyph) & mask ? foreground_color : background_color, _x, _y);
                    mask >>= 1;
                }
                glyph += bytesperline;
            }   
            
            if (x++ > characters_per_line)
            {
                x = 0;
                if (y++ > characters_per_row)
                    terminal_ScrollDown(1);
            } 
        }
        break;
    }
}
void terminal_ScrollDown(uint8_t lines)
{
    psf2_font_t* font = (psf2_font_t*)font_data;
    size_t bytes_to_copy_count = framebuffer->pitch * framebuffer->height - framebuffer->pitch - font->height * lines;
    memcpy((void*)framebuffer->base, (void*)(framebuffer->base + framebuffer->pitch * font->height * lines), bytes_to_copy_count);
}

void terminal_SetColor(uint32_t _foreground_color, uint32_t _background_color) 
{ 
    foreground_color = _foreground_color; 
    background_color = _background_color; 
}
