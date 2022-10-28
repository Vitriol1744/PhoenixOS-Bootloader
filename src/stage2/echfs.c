#include "echfs.h"
#include "disk.h"
#include "libc.h"

#define ECHFS_SIGNATURE "_ECH_FS_"

typedef struct
{
    uint8_t jump_instruction[4];
    uint8_t signature[8];
    uint64_t total_block_count;
    uint64_t main_directory_length;
    uint64_t bytes_per_block;
    uint64_t reserved;
    uint64_t uuid[2];
} __attribute__((packed)) identity_table_t;

typedef struct
{
    uint64_t directory_id;
    uint8_t object_type;
    uint8_t name[201];
    uint64_t unix_atime;
    uint64_t unix_mtime;
    uint16_t permissions;
    uint16_t owner_id;
    uint16_t group_id;
    uint64_t unix_ctime;
    uint64_t starting_block;
    uint64_t file_size;
} __attribute__((packed)) directory_entry_t;

int32_t echfs_open(file_handle_t* handle, uint8_t drive, uint8_t partition_chs[3], const char* filename)
{
    uint8_t head = partition_chs[0];
    uint8_t sector = partition_chs[1] & 0b00111111;
    uint16_t cylinder = partition_chs[1] & 0b11000000 << 2;
    cylinder |= partition_chs[2];
    uint32_t partition_lba = lba(cylinder, sector, head);

    uint8_t buffer[512];
    identity_table_t* identity_table = (identity_table_t*)buffer;
    read_sectors(drive, cylinder, sector, head, 1, &buffer);
    if (strcmp((char*)identity_table->signature, ECHFS_SIGNATURE)) return -1;

    uint32_t allocation_table_offset = 16;
    uint32_t allocation_table_size = (identity_table->total_block_count * sizeof(uint64_t) + 512 - 1) / 512;

    uint32_t main_directory_offset = allocation_table_offset + allocation_table_size;

    uint16_t dir_cylinder = 0;
    uint16_t dir_sector = 0;
    uint16_t dir_head = 0;

    chs(main_directory_offset + partition_lba, &dir_cylinder, &dir_sector, &dir_head);

    read_sectors(drive, dir_cylinder, dir_sector, dir_head, 1, (void*)0x30000);
    directory_entry_t* dir_entry = (directory_entry_t*)0x30000;

    if (!strcmp((char*)dir_entry->name, filename))
    {
        handle->file_lba = dir_entry->starting_block + partition_lba;
        handle->size = dir_entry->file_size;
    }
    return 0;
}