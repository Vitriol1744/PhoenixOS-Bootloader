#include "disk.h"

static uint8_t drive_type;
static uint16_t cylinder_count;
static uint16_t sector_count;
static uint16_t head_count;

void disk_Initialize(void)
{
    // hdd's start from 0x80 index
    for (uint8_t drive_num = 0x80; drive_num < 0xf0; drive_num++)
    {
        get_drive_parameters(drive_num, &drive_type, &cylinder_count, &sector_count, &head_count);
        return;
    }
}
uint8_t disk_GetDriveType(void) { return drive_type; }
uint16_t disk_GetCylinderCount(void) { return cylinder_count; }
uint16_t disk_GetSectorCount(void) { return sector_count; }
uint16_t disk_GetHeadCount(void) { return head_count; }