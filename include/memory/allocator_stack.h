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
template <size_t N, std::size_t alignment = alignof(std::max_align_t)>
class StackMemoryManager
{
public:
    StackMemoryManager();
    StackMemoryManager(const StackMemoryManager&) noexcept = delete;
    StackMemoryManager(StackMemoryManager&&) noexcept = default;
    ~StackMemoryManager() = default;

    StackMemoryManager& operator=(const StackMemoryManager&) noexcept = delete;
    StackMemoryManager& operator=(StackMemoryManager&&) noexcept = default;

    //! Stack buffer
    const uint8_t* buffer() const noexcept { return _buffer; }
    //! Stack capacity
    size_t capacity() const noexcept { return _capacity; }
    //! Stack allocated size
    size_t size() const noexcept { return _size; }

    //! Get the maximum number of elements, that could potentially be allocated by the allocator
    /*!
        \return The number of elements that might be allocated as maximum by a call to the allocate() method
    */
    size_t max_size() const noexcept { return capacity(); }

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
    void deallocate(void* ptr, size_t num);

    //! Reset the memory manager
    void reset();

private:
    uint8_t _buffer[N];
    size_t _capacity;
    size_t _size;
};

//! Stack memory allocator class
template <typename T, size_t N, bool nothrow = false, std::size_t alignment = alignof(std::max_align_t)>
using StackAllocator = Allocator<T, StackMemoryManager<N, alignment>, nothrow>;

} // namespace CppCommon

#include "allocator_stack.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_STACK_H
