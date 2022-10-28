#include "disk.h"
#include "elf.h"
#include "file.h"
#include "io.h"
#include "pmm.h"
#include "terminal.h"

#include "libc.h"

#include <stdbool.h>
#include <stdnoreturn.h>

#include "cpuid.h"

extern void* __bss_start[];
extern void* __bss_end[];

static uint64_t* pml4;
uint64_t kernel_func = 0xffffffff80000000;

static void mapPage(uint64_t virtual_address, uint64_t physical_address);
static void setupPaging(void)
{
    pml4 = (uint64_t*)pmm_AllocateAligned(512, 0x1000);
    memset(pml4, 0, 4096);

    for (uint64_t i = 0; i < 512; i++)
    {
        mapPage(i * 0x1000, i * 0x1000);
    }

    // Load PML4 into CR3 register
    __asm__("mov eax, %0\nmov cr3, eax" : : "r"(pml4));
}

static uint64_t* getNextLevel(uint64_t* level, uint64_t next_level_index)
{
    if (level[next_level_index] & 0x1)
	{
		return (uint64_t*)(level[next_level_index] & ~((uintptr_t) 0xfff));
	}

	const uintptr_t next_level = (uintptr_t) pmm_CallocateAligned(4096, 0x1000);
	level[next_level_index] = next_level | 0b11;

    uint64_t* ret = (uint64_t*)(level[next_level_index] & ~((uintptr_t) 0xFFF));
	return ret;
}

static void mapPage(uint64_t virtual_address, uint64_t physical_address)
{
    size_t pml4_entry = (virtual_address & ((uint64_t)0x1ff << 39)) >> 39;
    size_t pml3_entry = (virtual_address & ((uint64_t)0x1ff << 30)) >> 30;
    size_t pml2_entry = (virtual_address & ((uint64_t)0x1ff << 21)) >> 21;
    size_t pml1_entry = (virtual_address & ((uint64_t)0x1ff << 12)) >> 12;

    uint64_t* pml3 = getNextLevel(pml4, pml4_entry);
    uint64_t* pml2 = getNextLevel(pml3, pml3_entry);
    uint64_t* pml1 = getNextLevel(pml2, pml2_entry);

    pml1[pml1_entry] = physical_address | 3;
    //printf("pml1[0]: %x\n", pml1[pml1_entry]);
    //__asm__("hlt");
}

extern __attribute__((cdecl)) void go_long_mode(uint64_t);

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

 //outputs a character to the debug console
 #define BochsConsolePrintChar(c) outportb(0xe9, c)
 //stops simulation and breaks into the debug console
 #define BochsBreak() outportw(0x8A00,0x8A00); outportw(0x8A00,0x08AE0);

void stage2_main(uint8_t drive)
{
    // Clear out .bss section
    for (uint8_t* ptr = (uint8_t*)__bss_start; ptr < (uint8_t*)__bss_end; ptr++)
        *ptr = 0;

    terminal_Initialize();
    pmm_Initialize();
    disk_Initialize();

    printf("[BOOT]: Drive: 0x%x\n", drive);
    file_handle_t file;
    if (fopen(&file, drive, 0, "kernel.elf"))
        printf("[BOOT]: Failed to open kernel.elf!\n");

    uint8_t* kernel = (uint8_t*)pmm_Allocate(file.size);
    fread(&file, kernel, file.size);

    uint64_t kernel_physical_address, kernel_virtual_address;
    elf64_Load(kernel, &kernel_physical_address, &kernel_virtual_address);

    setupPaging();
    mapPage(kernel_virtual_address, kernel_physical_address);
    printf("[BOOT]: Kernel entrypoint: 0x%x\n", kernel_virtual_address);

    printf("[BOOT]: Paging has been enabled!\n");
    printf("is cpuid supported: %d\n", cpuid_IsSupported());

    // Disable pic
    outb(0xa1, 0xff);
    outb(0x21, 0xff);

    // TODO: 5 Level Paging
    go_long_mode(kernel_virtual_address);
}
