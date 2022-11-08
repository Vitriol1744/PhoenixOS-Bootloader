#ifndef PHOENIXOS_BOOTLOADER__TERMINAL_H
#define PHOENIXOS_BOOTLOADER__TERMINAL_H

#include <stddef.h>
#include <stdint.h>

#include "arch/framebuffer.h"

/**
 * @brief Initializes the terminal
 */
void terminal_Initialize(struct framebuffer* framebuffer);
/**
 * @brief Prints String
 * @param str Pointer to string
 * @param length Number of characters to print
 */
void terminal_PrintString(const char* str, size_t length);
/**
 * @brief Prints character
 * @param c Character to be printed
 */
void terminal_PrintChar(uint32_t c);
/**
 * @brief Scrolls down
 * @param n Number of lines to scroll
 */
void terminal_ScrollDown(uint8_t n);
/**
 * @brief Sets foreground and background colors for text
 * @param color Text color
 */
void terminal_SetColor(uint32_t foreground_color, uint32_t background_color);

#endif // PHOENIXOS_BOOTLOADER__TERMINAL_H
