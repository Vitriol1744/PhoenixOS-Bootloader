#include "libc.h"

#include <stdbool.h>
#include <stdarg.h>

#include "terminal.h"

size_t strlen(const char* str)
{
    size_t ret = 0;
    while (*str != 0)
    {
        ++str;
        ++ret;
    }

    return ret;
}
int strcmp(const char* lhs, const char* rhs)
{
    while (*lhs != '\0' && *rhs != '\0' && *rhs == *lhs)
    {
        ++rhs;
        ++lhs;
    }
    if (*(lhs) != *(rhs)) return *lhs - *rhs;

    return 0;
}
int strncmp(const char* lhs, const char* rhs, size_t n)
{
    while (*rhs == *lhs && n > 0)
    {
        ++rhs;
        ++lhs;
        --n;
    }
    if (*(lhs-1) != *(rhs-1)) return *lhs - *rhs;

    return 0;
}
void* memset(void* dest, int c, size_t n)
{
    void* ret = dest;
    while (n > 0)
    {
        *(char*)dest++ = c;
        n--;
    }
    return ret;
}
void* memcpy(void* dest, const void* src, size_t bytes)
{
    uint8_t* d = (uint8_t*)dest;
    uint8_t* s = (uint8_t*)src;
    while (bytes > 0)
    {
        *d = *s;
        ++d; 
        ++s;
        --bytes;
    }

    return dest;
}

int atoi(const char* str)
{
    int ret = 0;
    bool is_negative = str[0] == '-';

    int32_t index = 1;
    size_t string_length = strlen(str);

    ret += str[string_length--] - '0';
    for (; string_length > 0 && str[string_length] != '-'; string_length--, index++)
    {
        ret += (str[string_length] - '0') * 10 * index;
    }

    if (is_negative) ret = -ret;
    return ret;
}

char* itoa(int value, char* str, int base)
{
    int i = 0;
    bool is_negative = false;
  
    if (value == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
  
    if (value < 0 && base == 10)
    {
        is_negative = true;
        value = -value;
    }
  
    while (value != 0)
    {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value = value / base;
    }
  
    if (is_negative) str[i++] = '-';
    str[i] = '\0';
  
    int start = 0;
    int end = i -1;
    while (start < end)
    {
        char c = *(str + start);
        *(str + start) = *(str + end);
        *(str + end) = c;
        start++;
        end--;
    }
  
    return str;
}

void printf(const char* fmt, ...)
{
    char* c;
    va_list args;
    va_start(args, fmt);

    for (c = (char*)fmt; *c != '\0'; c++)
    {
        while (*c != '%')
        {
            if (*c == '\0') goto loop_end;
            if (*c == '\n') terminal_PrintChar('\r');
            terminal_PrintChar(*c);
            c++;
        }
        c++;
        bool to_lower = false;
        switch (*c)
        {
            case 'b':
            {
                uint64_t value = va_arg(args, uint64_t);
                char string[20];
                char* str = string;
                itoa(value, str, 2);
                printf("0b");
                while (*str != '\0')
                {
                    terminal_PrintChar(*str);
                    ++str;
                }
                break;
            }
            case 'c':
            {
                char _c = va_arg(args, int);
                terminal_PrintChar(_c);
                break;
            }
            case 'd':
            {
                uint64_t value = va_arg(args, uint64_t);
                char string[20];
                char* str = string;
                itoa(value, str, 10);
                while (*str != '\0')
                {
                    terminal_PrintChar(*str);
                    ++str;
                }
                break;
            }
            case 'i':
            {
                int64_t value = va_arg(args, int64_t);
                char string[20];
                char* str = string;
                itoa(value, str, 10);
                while (*str != '\0')
                {
                    terminal_PrintChar(*str);
                    ++str;
                }
                break;
            }
            case 'o':
            {
                uint64_t value = va_arg(args, uint64_t);
                char string[20];
                char* str = string;
                itoa(value, str, 8);
                terminal_PrintChar('0');
                while (*str != '\0')
                {
                    terminal_PrintChar(*str);
                    ++str;
                }
                break;
            }
            case 'r':
            {
                char* string = va_arg(args, char*);
                char* start = string;
                while (*string != '\0') ++string;
                while (start <= string) terminal_PrintChar(*string--);
                break;
            }
            case 's':
            {
                char* string = va_arg(args, char*);
                while (*string != '\0')
                {
                    terminal_PrintChar(*string);
                    ++string;
                }
                break;
            }
            case 'u':
            {
                uint64_t value = va_arg(args, uint64_t);
                char string[20];
                char* str = string;
                itoa(value, str, 10);
                while (*str != '\0')
                {
                    terminal_PrintChar(*str);
                    ++str;
                }
                break;
            }
            case 'x': 
            {
                uint64_t value = va_arg(args, uint64_t);
                char string[20];

                char* str = string;
                itoa(value, str, 16);
                //printf("0x");
                while (*str != '\0')
                {
                    //if (to_lower && *str >= 'A' && *str <= 'Z') c += 32;
                    //else if (!to_lower && *str >= 'a' && *str <= 'z') c -= 32;
                    terminal_PrintChar(*str++);
                }
                break;
            }
        }
    }
loop_end:
    va_end(args);
}