#include "echfs.h"

#include "libc.h"
#include "pmm.h"
#include "disk.h"


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

typedef struct
{
    uint32_t main_directory_offset;
    uint64_t main_directory_length;
} echfs_info_t;

bool echfs_Verify(partition_t* part)
{
    uint32_t partition_lba = part->lba;
    uint16_t cylinder, sector, head;
    chs(part->disk, partition_lba, &cylinder, &sector, &head);

    identity_table_t* identity_table = (identity_table_t*)pmm_AllocateBelow1M(512);
    disk_Read(part->disk, identity_table, part->lba * 512, 512);
    part->block_size = (uint32_t)identity_table->bytes_per_block;

    if (strncmp((char*)identity_table->signature, ECHFS_SIGNATURE, 8)) return false;

    echfs_info_t* echfs_info = pmm_Allocate(sizeof(echfs_info));
    uint32_t allocation_table_offset = 16;
    uint32_t allocation_table_size = (uint32_t)(identity_table->total_block_count * sizeof(uint64_t) + identity_table->bytes_per_block - 1) / (uint32_t)identity_table->bytes_per_block;

    echfs_info->main_directory_offset = allocation_table_offset + allocation_table_size;
    echfs_info->main_directory_length = identity_table->main_directory_length;
    part->fs_specific_info = (void*)echfs_info;
    return true;
}
int32_t echfs_OpenFile(file_handle_t* handle, partition_t* part, const char* filename)
{
    echfs_info_t* echfs_info = (echfs_info_t*)part->fs_specific_info;

    directory_entry_t* dir_entry = (directory_entry_t*)pmm_Allocate(1024);
    for (uint64_t i = 0; i < echfs_info->main_directory_length * part->block_size; i += sizeof(directory_entry_t))
    {        
        uint64_t offset = echfs_info->main_directory_offset * part->block_size + part->lba * 512 + i;
        disk_Read(part->disk, dir_entry, offset, sizeof(directory_entry_t));
        if (!strcmp((char*)dir_entry->name, filename))
        {
            printf("filename %s\n", dir_entry->name);
            handle->file_lba = dir_entry->starting_block;
            handle->size = dir_entry->file_size;

            return 0;
        }
    }
    return -1;
}
int32_t echfs_Read(file_handle_t* handle, void* buffer, size_t bytes)
{
    uint32_t ret = disk_Read(handle->part->disk, buffer, handle->file_lba * handle->part->block_size + handle->part->lba * 512, bytes);

    return ret;
}
