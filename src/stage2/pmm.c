#include "pmm.h"

uintptr_t allocator_base;

void pmm_Initialize(void) { allocator_base = (uintptr_t)0x20000; }
uintptr_t pmm_Allocate(size_t bytes)
{
    allocator_base += bytes;

    return allocator_base - bytes;
}
void pmm_Free(size_t bytes) { allocator_base -= bytes; }