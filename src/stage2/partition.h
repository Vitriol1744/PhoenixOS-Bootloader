#ifndef PHOENIXOS_BOOT_PARTITION_H
#define PHOENIXOS_BOOT_PARTITION_H

#include "disk.h"

struct file_handle_t;
struct partition_t;

typedef uint32_t(*open_file_func_t)(struct file_handle_t* handle, struct partition_t* part, const char* filename);
typedef uint32_t(*read_file_func_t)(struct file_handle_t* handle, void* buffer, size_t bytes);

typedef struct
{
    uint32_t    index;
    disk_t*     disk;
    uint32_t    lba;
    uint32_t    block_size;

    // file system specific info
    void* fs_specific_info;
    open_file_func_t fopen;
    read_file_func_t fread;
} partition_t;

void partition_Initialize(partition_t* part, disk_t* disk, uint32_t partition_index);

#endif // PHOENIXOS_BOOT_PARTITION_H
