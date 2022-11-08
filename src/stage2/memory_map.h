#ifndef PHOENIXOS_MEMORY_MAP_H
#define PHOENIXOS_MEMORY_MAP_H

#include <stdint.h>

typedef struct SMAP_entry {
	uint64_t Base;
	uint64_t Length;
	uint32_t Type; // entry Type
	uint32_t ACPI; // extended
} __attribute__((packed)) SMAP_entry_t;


#define MEMMAP_USABLE                 1
#define MEMMAP_RESERVED               2
#define MEMMAP_ACPI_RECLAIMABLE       3
#define MEMMAP_ACPI_NVS               4
#define MEMMAP_BAD_MEMORY             5

extern uint16_t mmap_entry_count;

int __attribute__((cdecl)) get_memory_map(SMAP_entry_t *Out);

#endif