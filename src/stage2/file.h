#ifndef PHOENIXOS_BOOT_FILE_H
#define PHOENIXOS_BOOT_FILE_H

#include <stdint.h>

typedef struct
{
    uint8_t drive;
    uint32_t file_lba;
    uint32_t size;
} file_handle_t;

int32_t fopen(file_handle_t* handle, uint8_t drive, uint8_t partition_index, const char* filename);
int32_t fread(file_handle_t* handle, void* buffer, uint32_t bytes);

#endif // PHOENIXOS_BOOT_FILE_H
