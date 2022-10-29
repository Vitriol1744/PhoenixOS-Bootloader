#include "elf.h"

#include "libc.h"
#include "terminal.h"
#include "pmm.h"

#define PT_LOAD 0x00000001

uint32_t elf32_Load(uint8_t* elf, uint64_t* kernel_physical_address, uint64_t* kernel_virtual_address)
{
    elf32_header_t* elf_header = (elf32_header_t*)elf;

    if (elf_header->magic_number != 0x464c457f)
    {
        printf("Not a valid elf file!");
        return 0;
    }

    void* program_header_table = (uint8_t*)elf_header + elf_header->header_size;
    for (uint32_t i = 0; i < elf_header->program_header_count; i++)
    {
        elf32_program_header_t* program_header = (elf32_program_header_t*)program_header_table + sizeof(elf32_program_header_t) * i;
        if (program_header->type == PT_LOAD)
        {
            uint8_t* program = (uint8_t*)elf + program_header->offset;
            *kernel_physical_address = (uint64_t)pmm_AllocateAligned(program_header->memory_size, program_header->alignment); 
            memcpy((uint8_t*)program_header->physical_address, program, (size_t)program_header->memory_size);

            *kernel_virtual_address = program_header->virtual_address;
            return program_header->memory_size;
        }
    }

    return 0;
}
uint64_t elf64_Load(uint8_t* elf, uint64_t* kernel_physical_address, uint64_t* kernel_virtual_address)
{
    elf64_header_t* elf_header = (elf64_header_t*)elf;

    if (elf_header->magic_number != 0x464c457f)
    {
        printf("Not a valid elf file!");
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
            printf("Kernel Size: %d bytes\n", program_header->memory_size);
            *kernel_physical_address = (uint64_t)pmm_AllocateAligned((uint32_t)program_header->memory_size, (uint32_t)program_header->alignment);
            printf("Kernel Physical Address: %x\n", *kernel_physical_address);
            memcpy((uint8_t*)*kernel_physical_address, program, (size_t)program_header->memory_size);

            *kernel_virtual_address = (uint64_t)program_header->virtual_address;
            return program_header->memory_size;
        }
    }

    return 0;
}
