#include "terminal.h"
#include "pmm.h"
#include "disk.h"
#include "file.h"
#include "elf.h"
#include "io.h"

#include "libc.h"

#include <stdnoreturn.h>

extern void* __bss_start[];
extern void* __bss_end[];

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));
\
extern void load_page_directory(unsigned int*);
extern void enable_paging();

void stage2_main(uint8_t drive)
{
    // Clear out .bss section
    for (uint8_t* ptr = (uint8_t*)__bss_start; ptr < (uint8_t*)__bss_end; ptr++) *ptr = 0;
    
    terminal_Initialize();
    pmm_Initialize();
    disk_Initialize();

    printf("[BOOT]: Drive: 0x%x\n", drive);
    file_handle_t file;
    if (fopen(&file, drive, 0, "kernel.elf")) printf("[BOOT]: Failed to open kernel.elf!\n");

    uint8_t* kernel = (uint8_t*)pmm_Allocate(file.size);
    fread(&file, kernel, file.size);
    
    uintptr_t kernel_entrypoint;
    elf32_Load(kernel, &kernel_entrypoint);
    printf("[BOOT]: Kernel entrypoint: 0x%x\n", kernel_entrypoint);

    for (uint32_t i = 0; i < 1024; i++)
    {
        page_directory[i] = 0x02;
        first_page_table[i] = (i * 0x1000) | 3;
    }
    page_directory[0] = ((uint32_t)first_page_table) | 3;

    load_page_directory(page_directory);
    enable_paging();

    printf("[BOOT]: Paging has been enabled!\n");

    // Disable pic
    outb(0xa1, 0xff);
    outb(0x21, 0xff);

    //TODO: Set up paging and switch to long mode

    // Jump to kernel
    printf("[BOOT]: Jumping to 0x%x...\n", kernel_entrypoint);
    __asm__ (
        "push 0;"
        "mov sp, 0x7c00;"
        "jmp %0" : : 
        "a"(kernel_entrypoint));

    // Shouldn't get here
halt:
    __asm__("a: jmp a"); // infinite loop
}