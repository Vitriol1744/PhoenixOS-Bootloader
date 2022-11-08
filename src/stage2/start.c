#include "drivers/disk.h"

#include "lib/log.h"
#include "lib/terminal.h"

#include "protos/phoenix_boot.h"

#include "io.h"
#include "pmm.h"

#include <stdnoreturn.h>

#define BochsBreak() outw(0x8a00, 0x8a00); outw(0x8a00, 0x08ae0)

extern void* __bss_start[];
extern void* __bss_end[];

extern bool a20_is_enabled();
extern bool a20_enable();

void stage2_main(uint8_t drive); // to suppress missing prototype warning
__attribute__((section(".entry")))
noreturn void stage2_main(uint8_t drive)
{
    // Clear out .bss section
    for (uint8_t* p = (uint8_t*)__bss_start; p < (uint8_t*)__bss_end; p++) *p = 0;

    if (a20_is_enabled() == false && a20_enable() == false) goto halt;

    pmm_Initialize();
    disk_InitializeDrives(drive);

    terminal_Initialize(getFramebuffer());
    
    bootPhoenixOS();
halt:
    __asm__("a: hlt; jmp a");
}
