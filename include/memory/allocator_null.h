/*!
    \file allocator_null.h
    \brief Null memory allocator definition
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_ALLOCATOR_NULL_H
#define CPPCOMMON_MEMORY_ALLOCATOR_NULL_H

#include "allocator.h"

namespace CppCommon {

//! Null memory manager class
/*!
    Null memory manager will always return nullptr for any attempt
    to allocate a memory buffer of the given size.

    Not thread-safe.
*/
class NullMemoryManager
{
public:
    NullMemoryManager() noexcept : _allocated(0), _allocations(0) {}
    NullMemoryManager(const NullMemoryManager&) = delete;
    NullMemoryManager(NullMemoryManager&&) = delete;
    ~NullMemoryManager() noexcept { reset(); }

    NullMemoryManager& operator=(const NullMemoryManager&) = delete;
    NullMemoryManager& operator=(NullMemoryManager&&) = delete;

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

//! Null memory allocator class
template <typename T, bool nothrow = false>
using NullAllocator = Allocator<T, NullMemoryManager, nothrow>;

} // namespace CppCommon

#include "allocator_null.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_NULL_H
