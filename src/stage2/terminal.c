#include "terminal.h"

#include "io.h"

#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 25

static uint16_t* video_memory = (uint16_t*)0xb8000;
static uint8_t x = 0;
static uint8_t y = 0;
static uint8_t color = 0;

static void setCursor(uint16_t x, uint16_t y)
{
    uint16_t pos = y * TERMINAL_WIDTH + x;
 
	outb(0x3d4, 0x0f);
	outb(0x3d5, (uint8_t) (pos & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint8_t) ((pos >> 8) & 0xff));
}

void terminal_Initialize(void)
{
    // Set font color(0x02 - Green)
    color = 0x02;
    
    // Get cursor position
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;

    y = pos / TERMINAL_WIDTH;
    x = pos % TERMINAL_WIDTH;

    uint16_t* last_character = video_memory + (y * TERMINAL_WIDTH + x);

    uint16_t* vga = video_memory + (8 * TERMINAL_WIDTH);

    while (vga < last_character)
    {
        char c = *vga;
        *vga = 0x03 << 8 | c;
        vga++;
    }
}

void terminal_PrintString(const char* str, size_t length)
{
    for (; length > 0; length--)
    {
        terminal_PrintChar(*str);
        str++;
    }
}
void terminal_PrintChar(char c)
{
    switch (c)
    {
        case '\n': 
        {
            if (++y >= TERMINAL_HEIGHT)
            {
                terminal_ScrollDown(1);
                y = TERMINAL_HEIGHT - 1;
            }
            break;
        }
        case '\r': x = 0; break;

        default: 
            video_memory[y * TERMINAL_WIDTH + x] = color << 8 | c;
            if (++x >= TERMINAL_WIDTH)
            {
                x = 0;
                if (++y >= TERMINAL_HEIGHT)
                {
                    terminal_ScrollDown(1);
                    y = TERMINAL_HEIGHT - 1;
                }
            }
            break;
    }
    setCursor(x, y);
}
void terminal_ScrollDown(uint8_t lines)
{
    while (lines > 0)
    {
        for (size_t i = 0; i < TERMINAL_WIDTH * TERMINAL_HEIGHT - TERMINAL_WIDTH; i++)
            video_memory[i] = video_memory[i + TERMINAL_WIDTH];

        uint16_t* video_memory_end = video_memory + TERMINAL_WIDTH * TERMINAL_HEIGHT * 2 - TERMINAL_WIDTH * 2;
        uint16_t* c = video_memory_end - TERMINAL_WIDTH;
        while (c < video_memory_end) *c++ = ' ';

        lines--;
    }
}

void terminal_SetColor(uint8_t _color) { color = _color; }