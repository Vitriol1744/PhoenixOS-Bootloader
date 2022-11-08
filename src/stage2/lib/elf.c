#include "elf.h"

#include "lib/libc.h"
#include "lib/log.h"
#include "pmm.h"

#define ELF_MAGIC 0x464c457f

#define PT_LOAD 0x00000001

uint8_t elf_GetBitness(uint8_t* elf)
{
    return ((elf64_header_t*)elf)->bitness;
}
uint32_t elf32_Load(uint8_t* elf, uint64_t* physical_address, uint64_t* virtual_address)
{
    elf32_header_t* elf_header = (elf32_header_t*)elf;

    if (elf_header->magic_number != ELF_MAGIC)
    {
        LogError("Not a valid elf file!");
        return 0;
    }
    if (elf_header->bitness != ELF_BITNESS_32)
    {
        LogError("Program is not 32 bit!");
        return 0;
    }

    void* program_header_table = (uint8_t*)elf_header + elf_header->header_size;
    for (uint32_t i = 0; i < elf_header->program_header_count; i++)
    {
        elf32_program_header_t* program_header = (elf32_program_header_t*)program_header_table + sizeof(elf32_program_header_t) * i;
        if (program_header->type == PT_LOAD)
        {
            uint8_t* program = (uint8_t*)elf + program_header->offset;
            *physical_address = (uint64_t)pmm_AllocateAligned(program_header->memory_size, program_header->alignment); 
            memcpy((uint8_t*)program_header->physical_address, program, (size_t)program_header->memory_size);

            *virtual_address = program_header->virtual_address;
            return program_header->memory_size;
        }
    }

    return 0;
}
uint64_t elf64_Load(uint8_t* elf, uint64_t* physical_address, uint64_t* virtual_address)
{
    elf64_header_t* elf_header = (elf64_header_t*)elf;

    if (elf_header->magic_number != ELF_MAGIC)
    {
        LogError("Not a valid elf file!");
        return 0;
    }
    if (elf_header->bitness != ELF_BITNESS_64)
    {
        LogError("Program is not 64 bit!");
        return 0;
    }

    //TODO: KASLR, KPTI/KEISER
    void* program_header_table = (uint8_t*)elf_header + elf_header->header_size;
    for (uint32_t i = 0; i < elf_header->program_header_count; i++)
    {
        elf64_program_header_t* program_header = (elf64_program_header_t*)program_header_table + sizeof(elf64_program_header_t) * i;
        if (program_header->type == PT_LOAD)
        {
            uint8_t* program = (uint8_t*)elf + program_header->offset;
            LogInfo("Program Size: %d bytes", program_header->memory_size);
            *physical_address = (uint64_t)pmm_AllocateAligned((uint32_t)program_header->memory_size, (uint32_t)program_header->alignment);
            LogInfo("Program Physical Address: %x", *physical_address);
            memcpy((uint8_t*)*physical_address, program, (size_t)program_header->memory_size);

            *virtual_address = (uint64_t)program_header->virtual_address;
            return program_header->memory_size;
        }
    }

    return 0;
}
