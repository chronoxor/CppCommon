/*!
    \file allocator_arena.h
    \brief Arena memory allocator definition
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_ALLOCATOR_ARENA_H
#define CPPCOMMON_MEMORY_ALLOCATOR_ARENA_H

#include "allocator.h"

namespace CppCommon {

//! Arena memory manager class
/*!
    Arena memory manager uses pre-allocated memory buffer (arena) as a source
    of memory during the allocation.

    When a new block of memory allocated then the arena free size is checked and
    the free offset is moved. If there is no enough space std::bad_alloc will be
    thrown or null is returned (depends on parameter).

    Deallocate method does nothing. When the allocation/deallocation phase
    is finished then the free arena offset could be reset with a corresponding
    method.

    Not thread-safe.
*/
template <std::size_t alignment = alignof(std::max_align_t)>
class ArenaMemoryManager
{
public:
    //! Pre-allocate arena with a given capacity
    /*!
        \param capacity - Arena capacity in bytes
    */
    explicit ArenaMemoryManager(size_t capacity);
    //! Initialize arena with a given buffer
    /*!
        \param buffer - Arena buffer
        \param size - Arena buffer size
    */
    explicit ArenaMemoryManager(uint8_t* buffer, size_t size);
    ArenaMemoryManager(const ArenaMemoryManager&) noexcept = delete;
    ArenaMemoryManager(ArenaMemoryManager&&) noexcept = default;
    ~ArenaMemoryManager();

    ArenaMemoryManager& operator=(const ArenaMemoryManager&) noexcept = delete;
    ArenaMemoryManager& operator=(ArenaMemoryManager&&) noexcept = default;

    //! Arena buffer
    const uint8_t* buffer() const noexcept { return _buffer; }
    //! Arena capacity
    size_t capacity() const noexcept { return _capacity; }
    //! Arena allocated size
    size_t size() const noexcept { return _size; }

    //! Get the maximum number of elements, that could potentially be allocated by the allocator
    /*!
        \return The number of elements that might be allocated as maximum by a call to the allocate() method
    */
    size_t max_size() const noexcept { return _capacity; }

    //! Allocate a block of storage of the given size
    /*!
        \param size - Block size
        \param hint - Allocation hint (default is nullptr)
        \return A pointer to the block of storage
    */
    void* allocate(size_t size, const void* hint = nullptr);
    //! Release a block of storage previously allocated
    /*!
        \param ptr - Pointer to a block of storage
        \param size - Block size
    */
    void deallocate(void* ptr, size_t size);

    //! Reset the memory manager
    void reset();
    //! Reset the memory manager with a given capacity
    /*!
        \param capacity - Arena capacity in bytes
    */
    void reset(size_t capacity);
    //! Reset the memory manager with a given buffer
    /*!
        \param buffer - Arena buffer
        \param size - Arena buffer size
    */
    void reset(uint8_t* buffer, size_t size);

private:
    bool _external;
    uint8_t* _buffer;
    size_t _capacity;
    size_t _size;
};

//! Arena memory allocator class
template <typename T, bool nothrow = false, std::size_t alignment = alignof(std::max_align_t)>
using ArenaAllocator = Allocator<T, ArenaMemoryManager<alignment>, nothrow>;

} // namespace CppCommon

#include "allocator_arena.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_ARENA_H
