#ifndef PHOENIXOS_BOOTLOADER__ECHFS_H
#define PHOENIXOS_BOOTLOADER__ECHFS_H

#include "file.h"

#include <stdbool.h>

/**
 * @brief Verifies if partition is echfs formatted
 * @param part Pointer to partition to be verified
 * 
 * @return Non-zero if partition is echfs formatted, 0 otherwise
 */
bool echfs_Verify(partition_t* part);
/**
 * @brief Opens file on @ref echfs formatted partition specified by @ref part with specified @ref filename
 * 
 * @return Non-zero on success
 */
int32_t echfs_OpenFile(file_handle_t* handle, partition_t* part, const char* filename);
/**
 * @brief Reads data from file
 * @param handle Pointer to file to read data from
 * @param buffer Destination buffer
 * @param bytes Number of bytes to read
 * 
 * @return Non-zero on success 
 */
int32_t echfs_Read(file_handle_t* handle, void* buffer, size_t bytes);

#endif // PHOENIXOS_BOOTLOADER__ECHFS_H
