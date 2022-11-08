#include "file.h"

#include "drivers/disk.h"

#include "filesystem/echfs.h"

#include "lib/libc.h"
#include "pmm.h"


int32_t fopen(file_handle_t* handle, partition_t* part, const char* filename)
{
    handle->drive = 0x80;
    handle->part = part;
    
    return part->fopen((struct file_handle_t*)handle, (struct partition_t*)part, filename);
}
int32_t fread(file_handle_t* handle, void* buffer, uint64_t bytes)
{
    return handle->part->fread((struct file_handle_t*)handle, buffer, bytes);
}
