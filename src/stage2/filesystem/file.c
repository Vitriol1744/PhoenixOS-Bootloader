#include "file.h"

#include "libc.h"
#include "pmm.h"

#include "disk.h"
#include "echfs.h"

int32_t fopen(file_handle_t* handle, partition_t* part, const char* filename)
{
    handle->drive = 0x80;
    handle->part = part;
    
    return part->fopen(handle, part, filename);
}
int32_t fread(file_handle_t* handle, void* buffer, uint64_t bytes)
{
    return handle->part->fread(handle, buffer, bytes);
}
