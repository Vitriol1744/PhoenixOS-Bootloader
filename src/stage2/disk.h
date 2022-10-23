#ifndef PHOENIXOS_BOOT_DISK_H
#define PHOENIXOS_BOOT_DISK_H

#include <stdint.h>
#include <stddef.h>

void disk_Initialize(void);
uint8_t disk_GetDriveType(void);
uint16_t disk_GetCylinderCount(void);
uint16_t disk_GetSectorCount(void);
uint16_t disk_GetHeadCount(void);

inline static void chs(uint32_t lba, uint16_t* cylinder, uint16_t* sector, uint16_t* head)
{
    *sector = lba % disk_GetSectorCount() + 1;
    *cylinder = (lba / disk_GetSectorCount()) / disk_GetHeadCount();
    *head = (lba / disk_GetSectorCount()) % disk_GetHeadCount();
}

inline static uint32_t lba(uint16_t cylinder, uint16_t sector, uint16_t head)
{
    return (cylinder * disk_GetHeadCount() + head) * disk_GetSectorCount() + (sector - 1);
}

int __attribute__((cdecl)) get_drive_parameters(uint8_t drive, uint8_t* drive_type, uint16_t* cylinders, uint16_t* sectors, uint16_t* heads);
int __attribute__((cdecl)) read_sectors(uint8_t drive, uint16_t cylinder, uint16_t sectors, uint16_t heads, uint8_t count, void* buffer);

#endif // PHOENIXOS_BOOT_DISK_H