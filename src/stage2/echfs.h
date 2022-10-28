#ifndef PHOENIXOS_BOOT_ECHFS_H
#define PHOENIXOS_BOOT_ECHFS_H

#include "file.h"

int32_t echfs_open(file_handle_t* handle, uint8_t drive, uint8_t partition_chs[3], const char* filename);

#endif // PHOENIXOS_BOOT_ECHFS_H
