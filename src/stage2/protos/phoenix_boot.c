#include "phoenix_boot.h"

#include "arch/x86/vbe.h"

#include "filesystem/file.h"

#include "lib/elf.h"
#include "lib/libc.h"
#include "lib/log.h"
#include "lib/terminal.h"

#include "io.h"
#include "partition.h"
#include "pmm.h"

#include <stdbool.h>
#include <limits.h>

#include "boot_info.h"

#define PHOS_KERNEL_FILE_NAME "PhoenixOS.elf"

typedef uint64_t phys_addr_t;
typedef uint64_t virt_addr_t;

boot_info_t* boot_info;

static phys_addr_t* pml4;

static void mapPage(virt_addr_t virtual_address, phys_addr_t physical_address);
static void setupPaging(void)
{
    pml4 = (phys_addr_t*)pmm_AllocateAligned(4096, 0x1000);
    memset(pml4, 0, 4096);

    for (uint64_t i = 0; i < 512; i++)
        mapPage(i * 0x1000, i * 0x1000);

    // Load PML4 into CR3 register
    __asm__("mov eax, %0\nmov cr3, eax" : : "r"(pml4));
}

static phys_addr_t* getNextLevel(phys_addr_t* level, uint64_t next_level_index)
{
    if (level[next_level_index] & 0x1)
		return (phys_addr_t*)(level[next_level_index] & ~0xfff);

	const phys_addr_t* next_level = (phys_addr_t*)pmm_CallocateAligned(4096, 0x1000);
	level[next_level_index] = (phys_addr_t)next_level | 0b11;

    phys_addr_t* ret = (phys_addr_t*)(level[next_level_index] & ~0xfff);
	return ret;
}

static void mapPage(virt_addr_t virtual_address, phys_addr_t physical_address)
{
    size_t pml4_entry = (virtual_address >> 39) & 0x1ff;
    size_t pml3_entry = (virtual_address >> 30) & 0x1ff;
    size_t pml2_entry = (virtual_address >> 21) & 0x1ff;
    size_t pml1_entry = (virtual_address >> 12) & 0x1ff;

    phys_addr_t* pml3 = getNextLevel(pml4, pml4_entry);
    phys_addr_t* pml2 = getNextLevel(pml3, pml3_entry);
    phys_addr_t* pml1 = getNextLevel(pml2, pml2_entry);

    pml1[pml1_entry] = physical_address | 3;
}

extern noreturn __attribute__((cdecl)) void go_long_mode(uint64_t, uint64_t);

static bool         cpuid_IsSupported(void)
{
    int ret = false;

    __asm__(
        "pushfd\n"                          //  Save EFLAGS
        "pushfd\n"                          //  Store EFLAGS
        "xor dword ptr [esp], 0x00200000\n" //  Invert the ID bit in stored EFLAGS
        "popfd\n"                           //  Load stored EFLAGS (with ID bit inverted)
        "pushfd\n"                          //  Store EFLAGS again (ID bit may or may not be inverted)
        "pop eax\n"                         //  eax = modified EFLAGS (ID bit may or may not be inverted)
        "xor eax, [esp]\n"                  //  eax = whichever bits were changed
        "popfd\n"                           //  Restore original EFLAGS
        "and eax,0x00200000\n"              //  eax = zero if ID bit can't be changed, else non-zero
        "mov %0, eax" : "=r"(ret));         //  return value

    return ret;
}

void panic(char *message) 
{
    printf("[PANIC]: %s\r\n", message);
    
    for(;;) {
        __asm__("cli");
        __asm__("hlt");
    }
}

extern uint8_t* font_data;
noreturn void bootPhoenixOS()
{
    struct framebuffer* framebuffer = getFramebuffer();

    file_handle_t kernel_file;
    size_t disk_count;
    disk_Enumerate(NULL, &disk_count);
    disk_t* disks = pmm_Allocate(disk_count * sizeof(disk_t));
    disk_Enumerate(disks, &disk_count);

    for (uint32_t i = 0; i < disk_count; i++)
    {
        disk_t* disk = disks + i;
        for (uint32_t part_i = 0; part_i < disk_GetPartitionCount(); part_i++)
        {
            partition_t part;
            partition_Initialize(&part, disk, 0);

            LogInfo("Drive: 0x%x", disk->drive_index);
            if (fopen(&kernel_file, &part, PHOS_KERNEL_FILE_NAME) == 0) panic("Failed to open kernel.elf!\n");
            else goto kernel_found;
        }
    }
    kernel_found:
    LogInfo("");
    uint8_t* kernel = (uint8_t*)pmm_Allocate((size_t)kernel_file.size);
    fread(&kernel_file, kernel, kernel_file.size);
    
    uint64_t kernel_physical_address, kernel_virtual_address;
    uint8_t kernel_program_bitness = elf_GetBitness(kernel);
    uint32_t kernel_size;
    kernel_size = kernel_program_bitness == ELF_BITNESS_64 ? 
        (uint32_t)elf64_Load(kernel, &kernel_physical_address, &kernel_virtual_address) :
        (uint32_t)elf32_Load(kernel, &kernel_physical_address, &kernel_virtual_address);

    setupPaging();
    for (uint32_t i = 0; i < kernel_size / 0x1000 + 1; i++)
        mapPage(kernel_virtual_address + i * 0x1000, kernel_physical_address + i * 0x1000);

    for (uint32_t i = 0; i < (framebuffer->pitch * framebuffer->height) / 0x1000 + 1; i++)
        mapPage(framebuffer->base + i * 0x1000, framebuffer->base + i * 0x1000);

    mapPage((virt_addr_t)framebuffer, (phys_addr_t)framebuffer);
    LogInfo("Kernel entrypoint: 0x%x", kernel_virtual_address);

    LogTrace("Paging has been enabled!");
    LogTrace("is cpuid supported: %d", cpuid_IsSupported());

    // Disable pic
    outb(0xa1, 0xff);
    outb(0x21, 0xff);

    boot_info = pmm_Allocate(sizeof(boot_info_t));
    boot_info->framebuffer = (uint64_t)pmm_Allocate(sizeof(framebuffer_t));
    memcpy((void*)boot_info->framebuffer, framebuffer, sizeof(framebuffer_t));
    boot_info->font_data = (uint64_t)font_data;

    // TODO: 5 Level Paging
    go_long_mode(kernel_virtual_address, (uint64_t)boot_info);
}
