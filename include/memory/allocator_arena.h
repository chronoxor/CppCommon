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
    Arena memory manager uses pre-allocated memory buffer (arena) as a first
    source of memory during the allocation. If arena buffer has insufficient
    space to allocate the required block then an auxiliary memory manager will
    be used.

    Arena memory manager is suitable for multiple allocations during long
    operations with a single reset at the end (e.g. HTTP request processing).

    Not thread-safe.
*/
template <class TAuxMemoryManager = DefaultMemoryManager>
class ArenaMemoryManager
{
public:
    //! Initialize arena memory manager with an auxiliary memory manager
    /*!
        Arena chunk capacity will be 65536.

        \param auxiliary - Auxiliary memory manager
    */
    explicit ArenaMemoryManager(TAuxMemoryManager& auxiliary) : ArenaMemoryManager(auxiliary, 65536) {}
    //! Initialize arena memory manager with an auxiliary memory manager and a given chunk capacity
    /*!
        \param auxiliary - Auxiliary memory manager
        \param capacity - Arena chunk capacity in bytes
    */
    explicit ArenaMemoryManager(TAuxMemoryManager& auxiliary, size_t capacity);
    //! Initialize arena memory manager with an auxiliary memory manager and a given buffer
    /*!
        \param auxiliary - Auxiliary memory manager
        \param buffer - Arena buffer
        \param capacity - Arena buffer capacity
    */
    explicit ArenaMemoryManager(TAuxMemoryManager& auxiliary, void* buffer, size_t capacity);
    ArenaMemoryManager(const ArenaMemoryManager&) = delete;
    ArenaMemoryManager(ArenaMemoryManager&&) = delete;
    ~ArenaMemoryManager() { clear(); }

    ArenaMemoryManager& operator=(const ArenaMemoryManager&) = delete;
    ArenaMemoryManager& operator=(ArenaMemoryManager&&) = delete;

    //! Allocated memory in bytes
    size_t allocated() const noexcept { return _allocated; }
    //! Count of active memory allocations
    size_t allocations() const noexcept { return _allocations; }

    //! Arena buffer
    const uint8_t* buffer() const noexcept { return _buffer; }
    //! Arena capacity
    size_t capacity() const noexcept { return _capacity; }
    //! Arena allocated size
    size_t size() const noexcept { return _size; }

    //! Maximum memory block size, that could be allocated by the memory manager
    size_t max_size() const noexcept { return _auxiliary.max_size(); }

    //! Auxiliary memory manager
    TAuxMemoryManager& auxiliary() noexcept { return _auxiliary; }

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
    //! Reset the memory manager with a given chunk capacity
    /*!
        \param capacity - Arena chunk capacity in bytes
    */
    void reset(size_t capacity);
    //! Reset the memory manager with a given buffer
    /*!
        \param buffer - Arena buffer
        \param capacity - Arena buffer capacity
    */
    void reset(void* buffer, size_t capacity);

    //! Clear arena memory allocator
    void clear();

private:
    // Arena chunk
    struct Chunk
    {
        uint8_t* buffer;
        size_t capacity;
        size_t size;
        Chunk* prev;
    };

    // Allocation statistics
    size_t _allocated;
    size_t _allocations;

    // Auxiliary memory manager
    TAuxMemoryManager& _auxiliary;

    // Arena chunks
    Chunk* _current;
    size_t _reserved;

    // External buffer
    bool _external;
    uint8_t* _buffer;
    size_t _capacity;
    size_t _size;

    //! Allocate arena
    Chunk* AllocateArena(size_t capacity, Chunk* prev);
    //! Clear arena
    void ClearArena();
};

//! Arena memory allocator class
template <typename T, class TAuxMemoryManager = DefaultMemoryManager, bool nothrow = false>
using ArenaAllocator = Allocator<T, ArenaMemoryManager<TAuxMemoryManager>, nothrow>;

/*! \example memory_arena.cpp Arena memory allocator example */

} // namespace CppCommon

#include "allocator_arena.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_ARENA_H
