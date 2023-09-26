/*!
    \file allocator_pool.inl
    \brief Memory pool allocator inline implementation
    \author Ivan Shynkarenka
    \date 08.05.2017
    \copyright MIT License
*/

namespace CppCommon {

template <class TAuxMemoryManager>
inline PoolMemoryManager<TAuxMemoryManager>::PoolMemoryManager(TAuxMemoryManager& auxiliary, size_t page, size_t pages)
    : _allocated(0),
      _allocations(0),
      _auxiliary(auxiliary),
      _external(false),
      _max_pages(0),
      _pages(0),
      _page(0),
      _current(nullptr),
      _free_block(nullptr)
{
    reset(page, pages);
}

template <class TAuxMemoryManager>
inline PoolMemoryManager<TAuxMemoryManager>::PoolMemoryManager(TAuxMemoryManager& auxiliary, void* buffer, size_t capacity)
    : _allocated(0),
      _allocations(0),
      _auxiliary(auxiliary),
      _external(false),
      _max_pages(0),
      _pages(0),
      _page(0),
      _current(nullptr),
      _free_block(nullptr)
{
    reset(buffer, capacity);
}

template <class TAuxMemoryManager>
inline void* PoolMemoryManager<TAuxMemoryManager>::malloc(size_t size, size_t alignment)
{
    assert((size > 0) && "Allocated block size must be greater than zero!");
    assert(Memory::IsValidAlignment(alignment) && "Alignment must be valid!");

    // Allocate huge blocks using the auxiliary memory manager
    if (size > _page)
    {
        void* result = _auxiliary.malloc(size, alignment);
        if (result != nullptr)
        {
            // Update allocation statistics
            _allocated += size;
            ++_allocations;
        }
        return result;
    }

    bool allocate = true;
    FreeBlock* prev_free_block = nullptr;
    FreeBlock* current_free_block = _free_block;

    while ((current_free_block != nullptr) || allocate)
    {
        // Try to allocate a new memory pool page
        if ((current_free_block == nullptr) && allocate)
        {
            if ((_current != nullptr) && (_current->next != nullptr))
            {
                // Use the next memory pool page
                _current = _current->next;

                // Update the current free block
                current_free_block = (FreeBlock*)_current->buffer;
                current_free_block->size = _page;
                current_free_block->next = nullptr;

                // Update the free blocks list
                if (prev_free_block != nullptr)
                    prev_free_block->next = current_free_block;
                else
                    _free_block = current_free_block;
            }
            else
            {
                // Check if there is enough pages to create
                if (_pages > 0)
                {
                    // Allocate a new memory pool page
                    Page* current = AllocateMemoryPool(_page, _current);
                    if (current != nullptr)
                    {
                        // Use the new memory pool page
                        _current = current;

                        // Update the current free block
                        current_free_block = (FreeBlock*)_current->buffer;
                        current_free_block->size = _page;
                        current_free_block->next = nullptr;

                        // Update the free blocks list
                        if (prev_free_block != nullptr)
                            prev_free_block->next = current_free_block;
                        else
                            _free_block = current_free_block;

                        // Update created memory pool pages count
                        --_pages;
                    }
                }
            }

            // Stop the allocation attempts
            allocate = false;
            continue;
        }

        // Reset the allocation attempts
        allocate = true;

        // Calculate memory adjustment including the allocation header
        size_t adjustment = AlignAdjustment(current_free_block, alignment, sizeof(AllocBlock));

        // Calculate aligned block size
        size_t aligned_size = size + adjustment;

        // Concatenate joint free blocks
        while (current_free_block->next != nullptr)
        {
            FreeBlock* base_free_block = current_free_block;
            FreeBlock* next_free_block = current_free_block->next;

            // Right joint
            if (((uint8_t*)base_free_block + base_free_block->size) == (uint8_t*)next_free_block)
            {
                current_free_block->size += next_free_block->size;
                current_free_block->next = next_free_block->next;
            }
            else
                break;
        }

        // If there is no enough free space in the current free block use the next one
        if (current_free_block->size < aligned_size)
        {
            // Optimization: Skip the last too small peace of memory in page
            if (current_free_block->next != nullptr)
                prev_free_block = current_free_block;
            current_free_block = current_free_block->next;
            continue;
        }

        static_assert(sizeof(AllocBlock) >= sizeof(FreeBlock), "Allocated block structure size must not be less than free block structure size!");

        // Allocate from the remaining memory in the current free block
        if ((current_free_block->size - aligned_size) <= sizeof(AllocBlock))
        {
            //Increase allocation size instead of creating a new free block
            aligned_size = current_free_block->size;

            // Update the free blocks list
            if (prev_free_block != nullptr)
                prev_free_block->next = current_free_block->next;
            else
                _free_block = current_free_block->next;
        }
        else
        {
            // Create a new free block containing remaining memory
            FreeBlock* next_free_block = (FreeBlock*)((uint8_t*)current_free_block + aligned_size);
            next_free_block->size = current_free_block->size - aligned_size;
            next_free_block->next = current_free_block->next;

            // Update the free blocks list
            if (prev_free_block != nullptr)
                prev_free_block->next = next_free_block;
            else
                _free_block = next_free_block;
        }

        // Calculate the aligned address
        uint8_t* aligned = (uint8_t*)current_free_block + adjustment;

        // Create a new allocated block
        AllocBlock* alloc_block = (AllocBlock*)(aligned - sizeof(AllocBlock));
        alloc_block->size = aligned_size;
        alloc_block->adjustment = adjustment;

        // Update allocation statistics
        _allocated += size;
        ++_allocations;

        return aligned;
    }

    // Out of memory...
    return nullptr;
}

template <class TAuxMemoryManager>
inline void PoolMemoryManager<TAuxMemoryManager>::free(void* ptr, size_t size)
{
    assert((ptr != nullptr) && "Deallocated block must be valid!");

    // Deallocate huge blocks using the auxiliary memory manager
    if (size > _page)
    {
        _auxiliary.free(ptr, size);

        // Update allocation statistics
        _allocated -= size;
        --_allocations;

        return;
    }

    AllocBlock* alloc_block = (AllocBlock*)((uint8_t*)ptr - sizeof(AllocBlock));

    uint8_t* block_start = (uint8_t*)ptr - alloc_block->adjustment;
    size_t   block_size  = alloc_block->size;
    uint8_t* block_end   = block_start + block_size;

    // Insert a new free block in the begin of the list
    FreeBlock* old_free_block = _free_block;
    FreeBlock* new_free_block = (FreeBlock*)block_start;
    new_free_block->size = block_size;
    new_free_block->next = old_free_block;
    _free_block = new_free_block;

    // Concatenate two joint free blocks
    if (old_free_block != nullptr)
    {
        // Left joint
        if (((uint8_t*)old_free_block + old_free_block->size) == block_start)
        {
            old_free_block->size += new_free_block->size;
            _free_block = old_free_block;
        }
        // Right joint
        else if ((uint8_t*)old_free_block == block_end)
        {
            new_free_block->size += old_free_block->size;
            new_free_block->next = old_free_block->next;
            _free_block = new_free_block;
        }
    }

    // Update allocation statistics
    _allocated -= size;
    --_allocations;
}

template <class TAuxMemoryManager>
inline void PoolMemoryManager<TAuxMemoryManager>::reset()
{
    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    if (_current != nullptr)
    {
        // Reset the current memory pool page
        while (_current->prev != nullptr)
            _current = _current->prev;

        // Reset the free block pointer
        _free_block = (FreeBlock*)_current->buffer;
        _free_block->size = _page;
        _free_block->next = nullptr;
    }
}

template <class TAuxMemoryManager>
inline void PoolMemoryManager<TAuxMemoryManager>::reset(size_t page, size_t pages)
{
    assert((page >= (sizeof(Page) + sizeof(AllocBlock))) && "Memory pool page must be big enough to fit at least one allocation block!");

    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    // Clear previous allocations
    clear();

    // Initialize the memory pool
    _external = false;
    _max_pages = (pages > 0) ? pages : std::numeric_limits<size_t>::max();
    _pages = _max_pages;
    _page = page;

    // Allocate a new memory pool page
    Page* current = AllocateMemoryPool(page, _current);
    if (current != nullptr)
    {
        // Use the new memory pool page
        _current = current;

        // Initialize the free block pointer
        _free_block = (FreeBlock*)_current->buffer;
        _free_block->size = page;
        _free_block->next = nullptr;

        // Update created memory pool pages count
        --_pages;
    }
}

template <class TAuxMemoryManager>
inline void PoolMemoryManager<TAuxMemoryManager>::reset(void* buffer, size_t capacity)
{
    assert((buffer != nullptr) && "Memory pool buffer must be valid!");
    assert((capacity >= (sizeof(Page) + sizeof(AllocBlock))) && "Memory pool buffer capacity must be big enough to fit at least one allocation block!");

    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    // Clear previous allocations
    clear();

    // Initialize the external memory pool
    _external = true;
    _max_pages = 1;
    _pages = 0;
    _page = capacity;
    _current = (Page*)buffer;
    _current->buffer = (uint8_t*)buffer + sizeof(Page);
    _current->prev = nullptr;
    _current->next = nullptr;

    // Initialize the free block pointer
    _free_block = (FreeBlock*)_current->buffer;
    _free_block->size = capacity - sizeof(Page);
    _free_block->next = nullptr;
}

template <class TAuxMemoryManager>
inline void PoolMemoryManager<TAuxMemoryManager>::clear()
{
    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    // Clear memory pool
    ClearMemoryPool();

    // Clear memory pool buffer
    _external = false;
    _max_pages = 0;
    _pages = 0;
    _page = 0;
    _current = nullptr;

    // Reset the free block pointer
    _free_block = nullptr;
}

template <class TAuxMemoryManager>
inline size_t PoolMemoryManager<TAuxMemoryManager>::AlignAdjustment(const void* address, size_t alignment)
{
    return (uint8_t*)Memory::Align(address, alignment) - (uint8_t*)address;
}

template <class TAuxMemoryManager>
inline size_t PoolMemoryManager<TAuxMemoryManager>::AlignAdjustment(const void* address, size_t alignment, size_t header)
{
    size_t adjustment = AlignAdjustment(address, alignment);

    size_t required = header;
    if (adjustment < required)
    {
        required -= adjustment;
        adjustment += alignment * (required / alignment);
        if ((required % alignment) > 0)
            adjustment += alignment;
    }

    return adjustment;
}

template <class TAuxMemoryManager>
inline typename PoolMemoryManager<TAuxMemoryManager>::Page* PoolMemoryManager<TAuxMemoryManager>::AllocateMemoryPool(size_t capacity, Page* prev)
{
    // Allocate a new memory pool page
    uint8_t* buffer = (uint8_t*)_auxiliary.malloc(sizeof(Page) + capacity + alignof(std::max_align_t));
    Page* page = (Page*)buffer;
    if (page != nullptr)
    {
        // Prepare and return a new memory pool page
        page->buffer = buffer + sizeof(Page);
        page->prev = prev;
        page->next = nullptr;
        if (prev != nullptr)
            prev->next = page;
        return page;
    }

    // Out of memory...
    return nullptr;
}

template <class TAuxMemoryManager>
inline void PoolMemoryManager<TAuxMemoryManager>::ClearMemoryPool()
{
    if (!_external)
    {
        // Clear all memory pool pages
        while (_current != nullptr)
        {
            Page* prev = _current->prev;
            Page* next = _current->next;
            _auxiliary.free(_current, sizeof(Page) + _page + alignof(std::max_align_t));
            if (prev != nullptr)
            {
                prev->next = next;
                _current = prev;
            }
            else if (next != nullptr)
            {
                next->prev = prev;
                _current = next;
            }
            else
                _current = nullptr;
        }

        // Reset the free block pointer
        _free_block = nullptr;
    }
}

} // namespace CppCommon
