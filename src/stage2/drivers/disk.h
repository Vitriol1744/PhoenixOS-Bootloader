#ifndef PHOENIXOS_BOOTLOADER__DISK_H
#define PHOENIXOS_BOOTLOADER__DISK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t drive_index;
    uint8_t drive_type;
    uint16_t total_cylinders;
    uint16_t total_sectors;
    uint16_t total_heads;
} disk_t;

bool disk_InitializeDrives(uint8_t boot_drive_index);
/**
 * @brief Returns pointer to the boot drive
 * @return Pointer to the boot drive
 */
disk_t* disk_GetBootDisk();
/**
 * @brief Returns number of partitions on disk
 * @return Number of partitions 
 */
bool disk_Enumerate(disk_t* buffer, size_t* count);
inline static uint32_t disk_GetPartitionCount(void) { return 1; }

/**
 * @brief Reads data from disk
 * @param disk Drive to read data from
 * @param buffer Destination buffer
 * @param offset Offset of data to be read
 * @param bytes Number of bytes to be read
 * 
 * @return Non-zero on success
 */
bool disk_Read(disk_t* disk, void* buffer, uint64_t offset, uint64_t bytes);

/**
 * @brief Converts LBA addressing to CHS
 */
inline static void chs(disk_t* disk, uint32_t lba, uint16_t* cylinder, uint16_t* sector, uint16_t* head)
{
    *sector = (uint16_t)(lba % disk->total_sectors + 1);
    *cylinder = (uint16_t)((lba / disk->total_sectors) / disk->total_heads);
    *head = (uint16_t)((lba / disk->total_sectors) % disk->total_heads);
}
/**
 * @brief Converts CHS addressing to LBA
 */
inline static uint32_t lba(disk_t* disk, uint16_t cylinder, uint16_t sector, uint16_t head)
{
    return (cylinder * disk->total_heads + head) * disk->total_sectors + (sector - 1);
}

#endif // PHOENIXOS_BOOTLOADER__DISK_H
