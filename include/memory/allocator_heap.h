/*!
    \file allocator_heap.h
    \brief Heap memory allocator definition
    \author Ivan Shynkarenka
    \date 07.06.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_ALLOCATOR_HEAP_H
#define CPPCOMMON_MEMORY_ALLOCATOR_HEAP_H

#include "allocator.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef DELETE
#undef ERROR
#undef Yield
#undef min
#undef max
#undef uuid_t
#endif

namespace CppCommon {

//! Heap memory manager class
/*!
    Heap memory manager will allocate memory in system heap.
    Windows: HeapAlloc()/HeapFree()
    Unix: malloc()/free()

    Not thread-safe.
*/
class HeapMemoryManager
{
public:
    HeapMemoryManager() noexcept : _allocated(0), _allocations(0) {}
    HeapMemoryManager(const HeapMemoryManager&) = delete;
    HeapMemoryManager(HeapMemoryManager&&) = delete;
    ~HeapMemoryManager() noexcept { reset(); }

    HeapMemoryManager& operator=(const HeapMemoryManager&) = delete;
    HeapMemoryManager& operator=(HeapMemoryManager&&) = delete;

    //! Allocated memory in bytes
    size_t allocated() const noexcept { return _allocated; }
    //! Count of active memory allocations
    size_t allocations() const noexcept { return _allocations; }

    //! Maximum memory block size, that could be allocated by the memory manager
    size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); }

    //! Allocate a new memory block of the given size
    /*!
        \param size - Block size
        \param alignment - Block alignment (default is alignof(std::max_align_t))
        \return A pointer to the allocated memory block or nullptr in case of allocation failed
    */
    void* malloc(size_t size, size_t alignment = alignof(std::max_align_t));
    //! Free the previously allocated memory block
    /*!
        \param ptr - Pointer to the memory block
        \param size - Block size
    */
    void free(void* ptr, size_t size);

    //! Reset the memory manager
    void reset();

private:
    // Allocation statistics
    size_t _allocated;
    size_t _allocations;
};

//! Heap memory allocator class
template <typename T, bool nothrow = false>
using HeapAllocator = Allocator<T, HeapMemoryManager, nothrow>;

} // namespace CppCommon

#include "allocator_heap.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_HEAP_H
