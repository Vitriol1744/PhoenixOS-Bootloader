#ifndef PHOENIXOS_BOOT_ARCH_H
#define PHOENIXOS_BOOT_ARCH_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Resets the drive
 * @param drive Drive index
 * 
 * @return Non-zero on success
 */
bool __attribute__((cdecl)) disk_reset(uint8_t drive);
/**
 * @brief Outputs drive parameters
 * @param drive Drive index
 * @param drive_type A pointer to variable receiving drive type
 * @param cylinders A pointer to variable receiving number of total cylinders
 * @param sectors A pointer to variable receiving number of sectors per head
 * @param heads A pointer to variable receiving number of heads per cylinder
 * 
 * @return Non-zero on success
 */
bool __attribute__((cdecl)) get_drive_parameters(uint8_t drive, uint8_t* drive_type, uint16_t* cylinders, uint16_t* sectors, uint16_t* heads);
/**
 * @brief Reads specified number of sectors from the specified CHS address
 * @param drive Drive index
 * @param cylinder Cylinder number
 * @param sector Sector number
 * @param head Head number
 * @param count Number of sectors to read
 * @param buffer Destination buffer
 * 
 * @return Number of sectors transferred on success, 0 on failure
 */
int __attribute__((cdecl)) read_sectors(uint8_t drive, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t count, void* buffer);

#endif // PHOENIXOS_BOOT_ARCH_H
