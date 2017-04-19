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
template <bool nothrow = false>
class NullMemoryManager
{
public:
    NullMemoryManager() noexcept = default;
    NullMemoryManager(const NullMemoryManager&) noexcept = default;
    NullMemoryManager(NullMemoryManager&&) noexcept = default;
    ~NullMemoryManager() noexcept = default;

    NullMemoryManager& operator=(const NullMemoryManager&) noexcept = default;
    NullMemoryManager& operator=(NullMemoryManager&&) noexcept = default;

    //! Get the maximum number of elements, that could potentially be allocated by the allocator
    /*!
        \return The number of elements that might be allocated as maximum by a call to the allocate() method
    */
    size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); }

    //! Allocate a block of storage suitable to contain the given count of elements
    /*!
        \param num - Number of elements to be allocated
        \param hint - Allocation hint (default is 0)
        \return A pointer to the initial element in the block of storage
    */
    void* allocate(size_t num, const void* hint = 0);
    //! Release a block of storage previously allocated
    /*!
        \param ptr - Pointer to a block of storage
        \param num - Number of releasing elements
    */
    void deallocate(void* ptr, size_t num) {}

    //! Reset the memory manager
    void reset() {}
};

//! Null memory allocator class
template <typename T, bool nothrow = false>
using NullAllocator = Allocator<T, NullMemoryManager<nothrow>>;

} // namespace CppCommon

#include "allocator_null.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_NULL_H
