/*!
    \file allocator_stack.h
    \brief Stack memory allocator definition
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_ALLOCATOR_STACK_H
#define CPPCOMMON_MEMORY_ALLOCATOR_STACK_H

#include "allocator.h"

namespace CppCommon {

//! Stack memory manager class
/*!
    Stack memory manager uses current stack buffer of the given size as a source
    of memory during the allocation.

    When a new block of memory allocated then the stack free size is checked and
    the free offset is moved. If there is no enough space std::bad_alloc will be
    thrown or null is returned (depends on parameter).

    Deallocate method does nothing. When the allocation/deallocation phase
    is finished then the free stack offset could be reset with a corresponding
    method.

    Not thread-safe.
*/
template <size_t N>
class StackMemoryManager
{
public:
    StackMemoryManager();
    StackMemoryManager(const StackMemoryManager&) = delete;
    StackMemoryManager(StackMemoryManager&&) = delete;
    ~StackMemoryManager() noexcept { reset(); }

    StackMemoryManager& operator=(const StackMemoryManager&) = delete;
    StackMemoryManager& operator=(StackMemoryManager&&) = delete;

    //! Allocated memory in bytes
    size_t allocated() const noexcept { return _allocated; }
    //! Count of active memory allocations
    size_t allocations() const noexcept { return _allocations; }

    //! Stack buffer
    const uint8_t* buffer() const noexcept { return _buffer; }
    //! Stack capacity
    size_t capacity() const noexcept { return _capacity; }
    //! Stack allocated size
    size_t size() const noexcept { return _size; }

    //! Maximum memory block size, that could be allocated by the memory manager
    size_t max_size() const noexcept { return _capacity; }

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
    // Stack buffer
    uint8_t _buffer[N];
    size_t _capacity;
    size_t _size;
};

//! Stack memory allocator class
template <typename T, size_t N, bool nothrow = false>
using StackAllocator = Allocator<T, StackMemoryManager<N>, nothrow>;

} // namespace CppCommon

#include "allocator_stack.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_STACK_H
