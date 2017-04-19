/*!
    \file allocator_arena.h
    \brief Arena memory allocator definition
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_ALLOCATOR_ARENA_H
#define CPPCOMMON_MEMORY_ALLOCATOR_ARENA_H

#include <cassert>
#include <memory>

namespace CppCommon {

//! Arena memory manager class
/*!
    Arena memory manager uses pre-allocated memory buffer (arena) as a source
    of memory during the allocation.

    When a new block of memory allocated arena free size is checked and free
    offset is moved. If there is no enough space std::bad_alloc will be thrown
    or null is returned (depends on parameter).

    Deallocate method does nothing. When the allocation/deallocation phase
    is finished free arena offset could be reset with a corresponding method.

    Not thread-safe.
*/
template <bool nothrow = false, std::size_t alignment = alignof(std::max_align_t)>
class MemoryManagerArena
{
public:
    //! Pre-allocate arena with a given capacity
    /*!
        \param capacity - Arena capacity in bytes
    */
    explicit MemoryManagerArena(size_t capacity);
    //! Initialize arena with a given buffer
    /*!
        \param buffer - Arena buffer
        \param size - Arena buffer size
    */
    explicit MemoryManagerArena(uint8_t* buffer, size_t size);
    MemoryManagerArena(const MemoryManagerArena&) noexcept = delete;
    MemoryManagerArena(MemoryManagerArena&&) noexcept = default;
    ~MemoryManagerArena();

    MemoryManagerArena& operator=(const MemoryManagerArena&) noexcept = delete;
    MemoryManagerArena& operator=(MemoryManagerArena&&) noexcept = default;

    //! Get the maximum number of elements, that could potentially be allocated by the allocator
    /*!
        \return The number of elements that might be allocated as maximum by a call to the allocate() method
    */
    size_t max_size() const noexcept { return _capacity; }

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
    void deallocate(void* ptr, size_type num) {}

    //! Reset the memory manager
    void reset();

private:
    bool _external;
    uint8_t* _arena;
    size_t _capacity;
    size_t _size;
};

//! Arena memory allocator class
template <typename T, bool nothrow = false, std::size_t alignment = alignof(std::max_align_t)>
using ArenaAllocator = Allocator<T, MemoryManagerArena<alignment, nothrow, alignment>>;

} // namespace CppCommon

#include "allocator_arena.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_ARENA_H
