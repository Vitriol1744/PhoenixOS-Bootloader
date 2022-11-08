#ifndef PHOENIXOS_BOOTLOADER__ELF_H
#define PHOENIXOS_BOOTLOADER__ELF_H

#include <stdint.h>

#define ELF_BITNESS_32 1
#define ELF_BITNESS_64 2

typedef struct
{
    uint32_t magic_number;
    uint8_t bitness;
    uint8_t endianness;
    uint8_t header_version;
    uint8_t abi;
    uint64_t padding;
    uint16_t type;
    uint16_t instruction_set;
    uint32_t elf_version;
    uint32_t program_entry_position;
    uint32_t program_header_table_position;
    uint32_t section_header_table_position;
    uint32_t flags;
    uint16_t header_size;
    uint16_t program_header_size;
    uint16_t program_header_count;
    uint16_t section_header_size;
    uint16_t section_header_count;
    uint16_t section_names_header_index;
} __attribute__((packed)) elf32_header_t;

typedef struct
{
    uint32_t type;
    uint32_t offset;
    uint32_t virtual_address;
    uint32_t physical_address;
    uint32_t file_size;
    uint32_t memory_size;
    uint32_t flags;
    uint32_t alignment;
} __attribute__((packed)) elf32_program_header_t;

typedef struct
{
    uint32_t magic_number;
    uint8_t bitness;
    uint8_t endianness;
    uint8_t header_version;
    uint8_t abi;
    uint64_t padding;
    uint16_t type;
    uint16_t instruction_set;
    uint32_t elf_version;
    uint64_t program_entry_position;
    uint64_t program_header_table_position;
    uint64_t section_header_table_position;
    uint32_t flags;
    uint16_t header_size;
    uint16_t program_header_size;
    uint16_t program_header_count;
    uint16_t section_header_size;
    uint16_t section_header_count;
    uint16_t section_names_header_index;
} __attribute__((packed)) elf64_header_t;

typedef struct
{
    uint32_t type;
    uint32_t flags;
    uint64_t offset;
    uint64_t virtual_address;
    uint64_t physical_address;
    uint64_t file_size;
    uint64_t memory_size;
    uint64_t alignment;
} __attribute__((packed)) elf64_program_header_t;

/**
 * @brief Checks elf program bitness
 * @param elf Pointer to elf data
 * 
 * @return Bitness of elf program
 */
uint8_t elf_GetBitness(uint8_t* elf);
/**
 * @brief Parses elf data of 32 bit program
 * @param elf Pointer to elf data
 * @param physical_address The address at which program will be loaded
 * @param virtual_address The virtual address at which program should be mapped to
 * 
 * @return Size of the program on success, 0 on failure
 */
uint32_t elf32_Load(uint8_t* elf, uint64_t* physical_address, uint64_t* virtual_address);
/**
 * @brief Parses elf data of 64 bit program
 * @param elf Pointer to elf data
 * @param physical_address The address at which program will be loaded
 * @param virtual_address The virtual address at which program should be mapped to
 * 
 * @return Size of the program on success, 0 on failure
 */
uint64_t elf64_Load(uint8_t* elf, uint64_t* physical_address, uint64_t* virtual_address);

#endif // PHOENIXOS_BOOTLOADER__ELF_H
