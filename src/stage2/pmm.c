#include "pmm.h"

#include "libc.h"

static uintptr_t allocator_base;
static uintptr_t below1m_base;
static uintptr_t below1m_top;

void pmm_Initialize(void) 
{ 
    below1m_base = 0x30000;
    allocator_base = 0x100000; 
    below1m_top = 0x0007FFFF; 
}
void* pmm_Allocate(size_t bytes)
{
    allocator_base += bytes;

    return (void*)(allocator_base - bytes);
}
void* pmm_AllocateAligned(size_t bytes, size_t alignment)
{
    allocator_base += alignment - (allocator_base % alignment);
    
    return pmm_Allocate(bytes);
}
void* pmm_Callocate(size_t bytes)
{
    void* ret = pmm_Allocate(bytes);
    memset(ret, 0, bytes);

    return ret;
}
void* pmm_CallocateAligned(size_t bytes, size_t alignment)
{
    while (allocator_base % alignment != 0) allocator_base++;
    void* ret = pmm_Callocate(bytes);
    return ret;
}
void* pmm_AllocateBelow1M(size_t bytes)
{
    if (below1m_base + bytes >= below1m_top) return NULL;
    below1m_base += bytes;

    return (void*)(below1m_base - bytes);
}
void* pmm_AllocateAlignedBelow1M(size_t bytes, size_t alignment)
{
    below1m_base += alignment - (below1m_base % alignment);

    return pmm_AllocateBelow1M(bytes);
}
void pmm_FreeBelow1M(size_t bytes) { below1m_base -= bytes; }
