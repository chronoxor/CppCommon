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

    //! Maximum memory block size, that could be allocated by the memory manager
    size_t max_size() const noexcept { return _auxiliary.max_size(); }

    //! Auxiliary memory manager
    TAuxMemoryManager& auxiliary() noexcept { return _auxiliary; }

    //! Allocate a new memory block of the given size
    /*!
        \param size - Block size
        \param hint - Allocation hint (default is nullptr)
        \return A pointer to the allocated memory block or nullptr in case of allocation failed
    */
    void* malloc(size_t size, const void* hint = nullptr);
    //! Free the previously allocated memory block
    /*!
        \param ptr - Pointer to the memory block
        \param size - Block size
    */
    void free(void* ptr, size_t size);

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
template <typename T, class TAuxMemoryManager = DefaultMemoryManager, bool nothrow = false, std::size_t alignment = alignof(std::max_align_t)>
using HybridAllocator = Allocator<T, HybridMemoryManager<TAuxMemoryManager, alignment>, nothrow>;

} // namespace CppCommon

#include "allocator_hybrid.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_HYBRID_H
