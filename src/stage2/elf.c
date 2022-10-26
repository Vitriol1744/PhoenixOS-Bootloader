#include "elf.h"

#include "libc.h"
#include "terminal.h"
#include "pmm.h"

int32_t elf32_Load(uint8_t* elf, uintptr_t* entry_point)
{
    elf32_header_t* elf_header = (elf32_header_t*)elf;
    *entry_point = elf_header->program_entry_position;

    if (strncmp((char*)elf_header->magic_number, "\127ELF", 4))
    {
        printf("Not a valid elf file!");
        return 1;
    }

    void* buffer;
    void* program_header_table = (uint8_t*)elf_header + elf_header->header_size;
    for (uint32_t i = 0; i < elf_header->program_header_count; i++)
    {
        elf32_program_header_t* program_header = (elf32_program_header_t*)program_header_table + sizeof(elf32_program_header_t) * i;
        if (program_header->type == 1)
        {
            uint8_t* program = (uint8_t*)elf + program_header->offset;
            memcpy((uint8_t*)program_header->virtual_address, program, program_header->memory_size);

            *entry_point = program_header->virtual_address;
        }
    }

    return 0;
}