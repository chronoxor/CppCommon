/*!
    \file allocator_pool.h
    \brief Memory pool allocator definition
    \author Ivan Shynkarenka
    \date 08.05.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_ALLOCATOR_POOL_H
#define CPPCOMMON_MEMORY_ALLOCATOR_POOL_H

#include "allocator.h"

namespace CppCommon {

//! Memory pool manager class
/*!
    Memory pool manager uses a pre-allocated memory buffer or several memory
    pages in order to create an effective free-list data structure, that
    allows to allocate and free memory.

    During the allocation memory pool manager will return a first-fit memory
    block in the free list with concatenating joint blocks to avoid memory
    defragmentation.

    If the allocated block is huge and does not fit into the memory pool page
    then it will be allocated directly from auxiliary memory manager.

    Not thread-safe.
*/
template <class TAuxMemoryManager = DefaultMemoryManager>
class PoolMemoryManager
{
public:
    //! Initialize memory pool manager with an auxiliary memory manager
    /*!
        Memory pool will have unlimited pages of size 65536.

        \param auxiliary - Auxiliary memory manager
    */
    explicit PoolMemoryManager(TAuxMemoryManager& auxiliary) : PoolMemoryManager(auxiliary, 65536, 0) {}
    //! Initialize memory pool manager with an auxiliary memory manager, single page size and max pages count
    /*!
        \param auxiliary - Auxiliary memory manager
        \param page - Page size in bytes
        \param pages - Max pages count. Zero value means unlimited count (default is 0)
    */
    explicit PoolMemoryManager(TAuxMemoryManager& auxiliary, size_t page, size_t pages = 0);
    //! Initialize memory pool manager with an auxiliary memory manager and a given buffer
    /*!
        \param auxiliary - Auxiliary memory manager
        \param buffer - Pool buffer
        \param capacity - Pool buffer capacity
    */
    explicit PoolMemoryManager(TAuxMemoryManager& auxiliary, void* buffer, size_t capacity);
    PoolMemoryManager(const PoolMemoryManager&) = delete;
    PoolMemoryManager(PoolMemoryManager&&) = delete;
    ~PoolMemoryManager() { clear(); }

    PoolMemoryManager& operator=(const PoolMemoryManager&) = delete;
    PoolMemoryManager& operator=(PoolMemoryManager&&) = delete;

    //! Allocated memory in bytes
    size_t allocated() const noexcept { return _allocated; }
    //! Count of active memory allocations
    size_t allocations() const noexcept { return _allocations; }

    //! Page size in bytes
    size_t page() const noexcept { return _page; }
    //! Max pages size
    size_t pages() const noexcept { return _max_pages; }

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
    //! Reset the memory manager with a given signle page size and max pages count
    /*!
        \param page - Page size in bytes
        \param pages - Max pages count. Zero value means unlimited count (default is 0)
    */
    void reset(size_t page, size_t pages = 0);
    //! Reset the memory manager with a given buffer
    /*!
        \param buffer - Pool buffer
        \param capacity - Pool buffer capacity
    */
    void reset(void* buffer, size_t capacity);

    //! Clear memory pool
    void clear();

private:
    // Pool page contains allocated and free blocks
    struct Page
    {
        uint8_t* buffer;
        Page* prev;
        Page* next;
    };
    // Allocated block
    struct AllocBlock
    {
        size_t size;
        size_t adjustment;
    };
    // Free block
    struct FreeBlock
    {
        size_t size;
        FreeBlock* next;
    };

    // Allocation statistics
    size_t _allocated;
    size_t _allocations;

    // Auxiliary memory manager
    TAuxMemoryManager& _auxiliary;

    // Pool pages
    bool _external;
    size_t _max_pages;
    size_t _pages;
    size_t _page;
    Page* _current;

    // Free block
    FreeBlock* _free_block;

    //! Calculate the align adjustment of the given buffer
    size_t AlignAdjustment(const void* address, size_t alignment);
    //! Calculate the align adjustment of the given buffer with header
    size_t AlignAdjustment(const void* address, size_t alignment, size_t header);

    //! Allocate memory pool
    Page* AllocateMemoryPool(size_t capacity, Page* prev);
    //! Clear memory pool
    void ClearMemoryPool();
};

//! Pool memory allocator class
template <typename T, class TAuxMemoryManager = DefaultMemoryManager, bool nothrow = false>
using PoolAllocator = Allocator<T, PoolMemoryManager<TAuxMemoryManager>, nothrow>;

/*! \example memory_pool.cpp Pool memory allocator example */

} // namespace CppCommon

#include "allocator_pool.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_POOL_H
