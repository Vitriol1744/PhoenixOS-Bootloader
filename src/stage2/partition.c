#include "partition.h"

#include "pmm.h"

#include "filesystem/echfs.h"

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

void partition_Initialize(partition_t* this, disk_t* disk, uint32_t partition_index)
{
    this->index = partition_index;
    this->disk = disk;
    
    mbr_t mbr;
    disk_Read(disk, (uint8_t*)&mbr, 0, 512);

    uint8_t head = mbr.entries[this->index].first_sector_chs[0];
    uint8_t sector = mbr.entries[this->index].first_sector_chs[1] & 0b00111111;
    uint16_t cylinder = mbr.entries[this->index].first_sector_chs[1] & 0b11000000 << 2;
    cylinder |= mbr.entries[this->index].first_sector_chs[2];
    this->lba = lba(this->disk, cylinder, sector, head);

    if (echfs_Verify(this))
    {
        this->fopen = (open_file_func_t)echfs_OpenFile;
        this->fread = (read_file_func_t)echfs_Read;
        return;
    }
}
