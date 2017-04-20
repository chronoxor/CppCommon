/*!
    \file allocator_hybrid.h
    \brief Hybrid memory allocator definition
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_ALLOCATOR_HYBRID_H
#define CPPCOMMON_MEMORY_ALLOCATOR_HYBRID_H

#include "allocator.h"

namespace CppCommon {

//! Hybrid memory manager class
/*!
    Hybrid memory manager uses pre-allocated memory buffer (arena) as a first
    source of memory during the allocation. If arena buffer has insufficient
    space to allocate the required block then an auxiliary memory manager will
    be used.

    Not thread-safe.
*/
template <class TAuxMemoryManager = DefaultMemoryManager, std::size_t alignment = alignof(std::max_align_t)>
class HybridMemoryManager
{
public:
    //! Initialize memory manager with an auxiliary memory manager and a given capacity
    /*!
        \param auxiliary - Auxiliary memory manager
        \param capacity - Arena capacity in bytes
    */
    explicit HybridMemoryManager(TAuxMemoryManager& auxiliary, size_t capacity);
    //! Initialize memory manager with an auxiliary memory manager and a given buffer
    /*!
        \param auxiliary - Auxiliary memory manager
        \param buffer - Arena buffer
        \param size - Arena buffer size
    */
    explicit HybridMemoryManager(TAuxMemoryManager& auxiliary, uint8_t* buffer, size_t size);
    HybridMemoryManager(const HybridMemoryManager&) noexcept = delete;
    HybridMemoryManager(HybridMemoryManager&&) noexcept = default;
    ~HybridMemoryManager();

    HybridMemoryManager& operator=(const HybridMemoryManager&) noexcept = delete;
    HybridMemoryManager& operator=(HybridMemoryManager&&) noexcept = default;

    //! Arena buffer
    const uint8_t* buffer() const noexcept { return _buffer; }
    //! Arena capacity
    size_t capacity() const noexcept { return _capacity; }
    //! Arena allocated size
    size_t size() const noexcept { return _size; }

    //! Auxiliary memory manager
    TAuxMemoryManager& auxiliary() noexcept { return _auxiliary; }

    //! Get the maximum number of elements, that could potentially be allocated by the allocator
    /*!
        \return The number of elements that might be allocated as maximum by a call to the allocate() method
    */
    size_t max_size() const noexcept { return auxiliary().max_size(); }

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
    TAuxMemoryManager& _auxiliary;
    size_t _auxiliary_allocated;
    bool _external;
    uint8_t* _buffer;
    size_t _capacity;
    size_t _size;
};

//! Hybrid memory allocator class
template <typename T, class TAuxMemoryManager = DefaultMemoryManager<nothrow>, bool nothrow = false, std::size_t alignment = alignof(std::max_align_t)>
using HybridAllocator = Allocator<T, HybridMemoryManager<TAuxMemoryManager, alignment>, nothrow>;

} // namespace CppCommon

#include "allocator_hybrid.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_HYBRID_H
