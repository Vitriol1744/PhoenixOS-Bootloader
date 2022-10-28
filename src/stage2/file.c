#include "file.h"

#include "libc.h"

#include "disk.h"
#include "echfs.h"

typedef struct
{
	uint8_t status;
	uint8_t first_sector_chs[3];
	uint8_t type;
	uint8_t last_sector_chs[3];
	uint32_t first_sector;
	uint32_t sector_count;
} __attribute__((packed)) mbr_partition_table_entry_t;

typedef struct
{
    uint8_t data[446];
    mbr_partition_table_entry_t entries[4];
} mbr_t;

int32_t fopen(file_handle_t* handle, uint8_t drive, uint8_t partition_index, const char* filename)
{
    mbr_t mbr;
    read_sectors(drive, 0, 1, 0, 1, &mbr);

    handle->drive = drive;
    int32_t ret = echfs_open(handle, drive, mbr.entries[partition_index].first_sector_chs, filename);

    return ret;
}
int32_t fread(file_handle_t* handle, void* buffer, uint32_t bytes)
{
    uint16_t file_cylinder;
    uint16_t file_sector;
    uint16_t file_head;
    chs(handle->file_lba, &file_cylinder, &file_sector, &file_head);
    read_sectors(handle->drive, file_cylinder, file_sector, file_head, (uint8_t)(handle->size / 512 + 1), (void*)0x20000);

    memcpy(buffer, (void*)0x20000, bytes);
    return 0;
}
