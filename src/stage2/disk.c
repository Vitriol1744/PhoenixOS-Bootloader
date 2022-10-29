#include "disk.h"

#include "libc.h"
#include "pmm.h"
#include "arch/arch.h"

void disk_Initialize(disk_t* this, uint8_t drive_index)
{
    this->drive_index = drive_index;
    disk_reset(drive_index);
    get_drive_parameters(drive_index, &this->drive_type, &this->total_cylinders, &this->total_sectors, &this->total_heads);
}
bool disk_Read(disk_t* disk, void* buffer, uint64_t offset, uint64_t bytes)
{
    uint8_t* temp = (uint8_t*)pmm_AllocateBelow1M(512);
    if (temp == NULL) return false;
    
    uint16_t cylinder, sector, head;
    size_t bytes_read = 0;
    while (bytes_read < bytes)
    {
        size_t lba = ((uint32_t)offset + bytes_read) / 512;

        chs(disk, lba , &cylinder, &sector, &head);
        if (read_sectors(disk->drive_index, cylinder, sector, head, 1, temp) == 0) return false;

        size_t to_read = (size_t)bytes - bytes_read;
        size_t off = (offset + bytes_read) % 512;
        if (to_read > 512 - off) to_read = 512 - off;

        memcpy((uint8_t*)buffer + bytes_read, temp + off, to_read);
        bytes_read += to_read;
    }

    pmm_FreeBelow1M(512);
    return 1;
}
