#include "pmm.h"

#ifdef PH_USE_UEFI
#include "efi.h"

extern EFI_BOOT_SERVICES* g_BS;
EFI_MEMORY_DESCRIPTOR* efi_memory_map;
UINTN efi_memory_map_size;
UINTN efi_map_key;
UINTN efi_descriptor_size;
UINT32 efi_descriptor_version;

void pmm_Initialize(void) 
{
    EFI_STATUS status = g_BS->GetMemoryMap(&efi_memory_map_size, efi_memory_map, &efi_map_key, &efi_descriptor_size, &efi_descriptor_version);
    if (EFI_ERROR(status)) goto failure;

failure:
    return;
}
#else
void pmm_Initialize(void) { }
#endif