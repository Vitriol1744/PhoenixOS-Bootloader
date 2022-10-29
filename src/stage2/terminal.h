#ifndef PHOENIXOS_BOOT_PRINT_H
#define PHOENIXOS_BOOT_PRINT_H
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Initializes the terminal
 */
void terminal_Initialize(void);
/**
 * @brief Prints first @ref 'length' characters of string pointed by @ref 'str'
 */
void terminal_PrintString(const char* str, size_t length);
/**
 * @brief Prints @ref 'c'
 */
void terminal_PrintChar(char c);
/**
 * @brief Scrolls down by @ref 'n' lines
 */
void terminal_ScrollDown(uint8_t n);
/**
 * @brief Sets text color to @ref 'color'
 */
void terminal_SetColor(uint8_t color);

#endif // PHOENIXOS_BOOT_PRINT_H
