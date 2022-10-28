#include "pmm.h"

#include "libc.h"

static uintptr_t allocator_base;

void pmm_Initialize(void) { allocator_base = (uintptr_t)0x30000; }
uintptr_t pmm_Allocate(size_t bytes)
{
    allocator_base += bytes;

    return allocator_base - bytes;
}
uintptr_t pmm_AllocateAligned(size_t bytes, size_t alignment)
{
    while (allocator_base % alignment != 0) allocator_base++;
    uintptr_t ret = pmm_Allocate(bytes);
    return ret;
}
uintptr_t pmm_Callocate(size_t bytes)
{
    uintptr_t ret = pmm_Allocate(bytes);
    memset(ret, 0, bytes);

    return ret;
}
uintptr_t pmm_CallocateAligned(size_t bytes, size_t alignment)
{
    while (allocator_base % alignment != 0) allocator_base++;
    uintptr_t ret = pmm_Callocate(bytes);
    return ret;
}
void pmm_Free(size_t bytes) { allocator_base -= bytes; }
