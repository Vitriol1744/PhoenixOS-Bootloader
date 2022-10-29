#ifndef PHOENIXOS_BOOT_FILE_H
#define PHOENIXOS_BOOT_FILE_H

#include <stdint.h>

#include "partition.h"

typedef struct
{
    uint8_t drive;
    partition_t* part;
    uint64_t file_lba;
    uint64_t size;
} file_handle_t;

int32_t fopen(file_handle_t* handle, partition_t* part, const char* filename);
int32_t fread(file_handle_t* handle, void* buffer, uint64_t bytes);

#endif // PHOENIXOS_BOOT_FILE_H
