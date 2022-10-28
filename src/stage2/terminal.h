#ifndef PHOENIXOS_BOOT_PRINT_H
#define PHOENIXOS_BOOT_PRINT_H
#include <stddef.h>
#include <stdint.h>

void terminal_Initialize(void);
void terminal_PrintString(const char* str, size_t length);
void terminal_PrintChar(char c);
void terminal_ScrollDown(uint8_t lines);

void terminal_SetColor(uint8_t _color);

#endif // PHOENIXOS_BOOT_PRINT_H
