#ifndef PHOENIXOS_BOOT_PMM_H
#define PHOENIXOS_BOOT_PMM_H

#include <stddef.h>
#include <stdint.h>

void pmm_Initialize(void);
uintptr_t pmm_Allocate(size_t bytes);
uintptr_t pmm_AllocateAligned(size_t bytes, size_t alignment);
uintptr_t pmm_Callocate(size_t bytes);
uintptr_t pmm_CallocateAligned(size_t bytes, size_t alignment);
// Free's recently allocated memory
void pmm_Free(size_t bytes);

#endif // PHOENIXOS_BOOT_PMM_H
