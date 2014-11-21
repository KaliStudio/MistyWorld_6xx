

#ifndef DETOURALLOCATOR_H
#define DETOURALLOCATOR_H

/// Provides hint values to the memory allocator on how long the
/// memory is expected to be used.
enum dtAllocHint
{
	DT_ALLOC_PERM,		///< Memory persist after a function call.
	DT_ALLOC_TEMP		///< Memory used temporarily within a function.
};

/// A memory allocation function.
//  @param[in]		size			The size, in bytes of memory, to allocate.
//  @param[in]		rcAllocHint	A hint to the allocator on how long the memory is expected to be in use.
//  @return A pointer to the beginning of the allocated memory block, or null if the allocation failed.
///  @see dtAllocSetCustom
typedef void* (dtAllocFunc)(int size, dtAllocHint hint);

/// A memory deallocation function.
///  @param[in]		ptr		A pointer to a memory block previously allocated using #dtAllocFunc.
/// @see dtAllocSetCustom
typedef void (dtFreeFunc)(void* ptr);

/// Sets the base custom allocation functions to be used by Detour.
///  @param[in]		allocFunc	The memory allocation function to be used by #dtAlloc
///  @param[in]		freeFunc	The memory de-allocation function to be used by #dtFree
void dtAllocSetCustom(dtAllocFunc *allocFunc, dtFreeFunc *freeFunc);

/// Allocates a memory block.
///  @param[in]		size	The size, in bytes of memory, to allocate.
///  @param[in]		hint	A hint to the allocator on how long the memory is expected to be in use.
///  @return A pointer to the beginning of the allocated memory block, or null if the allocation failed.
/// @see dtFree
void* dtAlloc(int size, dtAllocHint hint);

/// Deallocates a memory block.
///  @param[in]		ptr		A pointer to a memory block previously allocated using #dtAlloc.
/// @see dtAlloc
void dtFree(void* ptr);

#endif
