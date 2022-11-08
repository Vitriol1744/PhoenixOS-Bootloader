#include "disk.h"

#include "lib/libc.h"
#include "pmm.h"
#include "arch/arch.h"

static disk_t* disks;
static uint32_t disk_count = 1;
static disk_t* boot_disk;

bool disk_InitializeDrives(uint8_t boot_disk_index)
{
    disks = pmm_Allocate(sizeof(disk_t) * disk_count);

    uint32_t current_disk_index = 0;
    for (uint8_t i = 0x01; i < 0xf0; i++)
    {
        if (current_disk_index >= disk_count)
        {
            pmm_Allocate(sizeof(disk_t) * 1);
            disk_count += 1;
        }

        disk_t* current_disk = &disks[current_disk_index];

        uint8_t drive_type;
        uint16_t drive_cylinders, drive_sectors, drive_heads;

        if (!get_drive_parameters(i, &drive_type, &drive_cylinders, &drive_sectors, &drive_heads)) continue;
        if (!disk_reset(i)) continue;

        current_disk->drive_index       = i;
        current_disk->drive_type        = drive_type;
        current_disk->total_cylinders   = drive_cylinders;
        current_disk->total_sectors     = drive_sectors;
        current_disk->total_heads       = drive_heads;
        
        if (i == boot_disk_index) boot_disk = current_disk;
        current_disk_index++;
    }

    return true;
}

disk_t* disk_GetBootDisk() { return boot_disk; }
bool disk_Enumerate(disk_t* buffer, size_t* count)
{
    if (buffer == NULL)
    {
        *count = disk_count;
        return true;
    }

    memcpy(buffer, disks, disk_count * sizeof(disk_t));
    return true;
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
