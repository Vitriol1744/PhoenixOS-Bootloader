#ifndef PHOENIXOS_BOOTLOADER__FILE_H
#define PHOENIXOS_BOOTLOADER__FILE_H

#include <stdint.h>

#include "partition.h"

typedef struct
{
    uint8_t drive;
    partition_t* part;
    uint64_t file_lba;
    uint64_t size;
} file_handle_t;

/**
 * @brief Opens file
 * @param handle Pointer to file_handle_t structure receiving information about file
 * @param part Partition containing the file
 * @param filename Name of the file
 * 
 * @return Non-zero on success
 */
int32_t fopen(file_handle_t* handle, partition_t* part, const char* filename);
/**
 * @brief Read data from file
 * @param handle Pointer to file to read data from
 * @param buffer Destination buffer
 * @param bytes Number of bytes to read
 * 
 * @return Non-zero on success
 */
int32_t fread(file_handle_t* handle, void* buffer, uint64_t bytes);

#endif // PHOENIXOS_BOOTLOADER__FILE_H
