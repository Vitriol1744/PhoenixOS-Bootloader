#include "print.h"

#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 25

static uint16_t* buffer = (uint16_t*)0xb8000;
static uint8_t x = 0;
static uint8_t y = 13;
static uint8_t color = 0x02;

void print_char(char c)
{
    buffer[y * TERMINAL_WIDTH + x] = color << 8 | c;
    if (++x >= TERMINAL_WIDTH)
    {
        x = 0;
        if (++y >= TERMINAL_HEIGHT)
            y = 0;
    }
}