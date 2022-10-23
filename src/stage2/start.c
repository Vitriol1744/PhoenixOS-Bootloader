#include "terminal.h"
#include "pmm.h"
#include "disk.h"
#include "file.h"

#include "libc.h"

#include <stdnoreturn.h>

#ifdef PH_USE_UEFI
#include <efi.h>
#include <efilib.h>

EFI_SYSTEM_TABLE* g_ST;
EFI_BOOT_SERVICES* g_BS;

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    g_ST = SystemTable;
    g_BS = g_ST->BootServices;
 
    EFI_STATUS status = g_ST->ConOut->OutputString(g_ST->ConOut, L"PhoenixOS - Bootloader\r\n");
    if (EFI_ERROR(status)) return status;
 
    status = g_ST->ConIn->Reset(g_ST->ConIn, FALSE);
    if (EFI_ERROR(status)) return status;
 
    pmm_Initialize();

    EFI_INPUT_KEY key;
    while ((status = g_ST->ConIn->ReadKeyStroke(g_ST->ConIn, &key)) == EFI_NOT_READY) ;
 
    return status;
}
#endif

extern void* __bss_start[];
extern void* __bss_end[];

void _start(char var)
{
    // Clear out .bss section
    for (uint8_t* ptr = (uint8_t*)__bss_start; ptr < (uint8_t*)__bss_end; ptr++) *ptr = 0;
    terminal_Initialize();

    uint32_t drive = 0x80;
    disk_Initialize();

    file_handle_t file;
    fopen(&file, drive, 0, "kernel.bin");

    uint8_t* kernel[1024];
    fread(&file, kernel, 1024);
    // TODO: should use jmp instead of call
    __asm__ ("push 0\n\ncall eax" : : "a"(kernel));
    goto halt;

    char c = 0x41;
    unsigned int counter = 0;
    for (int i = 0; i < 959; i++) terminal_PrintChar(0x62);
    terminal_PrintChar(0x41);
halt:
    printf("File system working!");
    __asm__("a: jmp a"); // infinite loop
}