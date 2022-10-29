#ifndef PHOENIXOS_BOOT_ECHFS_H
#define PHOENIXOS_BOOT_ECHFS_H

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
 */
int32_t echfs_OpenFile(file_handle_t* handle, partition_t* part, const char* filename);
int32_t echfs_Read(file_handle_t* handle, void* buffer, size_t bytes);

#endif // PHOENIXOS_BOOT_ECHFS_H
