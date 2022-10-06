#include "print.h"

void print(const char* str)
{
    static char* buffer = (char*)0xb8000;

    while (*str)
    {
        print_char(*str);
        ++str;
    }
}

void _start()
{
    char* buffer = (char*)0xb8000;

    print("Hello, World!");
    for (;;);
}