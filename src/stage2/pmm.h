#ifndef PHOENIXOS_BOOT_PMM_H
#define PHOENIXOS_BOOT_PMM_H

#include <stddef.h>
#include <stdint.h>

/**
 * Water-Mark physical memory allocator
 */

/**
 * @brief Initializes physical memory manager
 */
void pmm_Initialize(void);
/**
 * @brief Allocates memory
 * @param n Number of bytes to be allocated
 * 
 * @return Address of allocated memory
 */
void* pmm_Allocate(size_t n);
/**
 * @brief Allocates memory on a specified alignment boundary
 * @param n Number of bytes to be allocated
 * @param alignment The alignment value, which must be an integer power of 2
 * 
 * @return Address of allocated memory
 */
void* pmm_AllocateAligned(size_t n, size_t alignment);
/**
 * @brief Allocates zero-initialized memory
 * @param n Number of bytes to be allocated
 * 
 * @return Address of allocated memory
 */
void* pmm_Callocate(size_t n);
/**
 * @brief Allocates zero-intialized memory on a specified alignment boundary
 * @param n Number of bytes to be allocated
 * @param alignment The alignment value, which must be an integer power of 2.
 * 
 * @return Address of allocated memory
 */
void* pmm_CallocateAligned(size_t n, size_t alignment);
/**
 * @brief Allocates memory below 1MB mark
 * @param n Number of bytes to be allocated
 * 
 * @return Address of allocated memory
 */
void* pmm_AllocateBelow1M(size_t n);
/**
 * @brief Allocates memory on a specified alignment boundary below 1MB mark
 * @param n Number of bytes to be allocated
 * @param alignment The alignment value, which must be an integer power of 2.
 * 
 * @return Address of allocated memory
 */
void* pmm_AllocateAlignedBelow1M(size_t n, size_t alignment);
/**
 * @brief Frees memory at top of allocator base
 * @param n number of bytes to be freed
 */
void pmm_FreeBelow1M(size_t n);

#endif // PHOENIXOS_BOOT_PMM_H
