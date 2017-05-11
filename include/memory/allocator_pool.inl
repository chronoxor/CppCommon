/*!
    \file allocator_pool.inl
    \brief Memory pool allocator inline implementation
    \author Ivan Shynkarenka
    \date 08.05.2017
    \copyright MIT License
*/

namespace CppCommon {

template <class TAuxMemoryManager>
inline PoolMemoryManager<TAuxMemoryManager>::PoolMemoryManager(TAuxMemoryManager& auxiliary, size_t chunk, size_t chunks)
    : _allocated(0),
      _allocations(0),
      _auxiliary(auxiliary),
      _external(false),
      _chunk(0),
      _chunks(0),
      _current(nullptr),
      _free_block(nullptr)
{
    reset(chunk, chunks);
}

template <class TAuxMemoryManager>
inline PoolMemoryManager<TAuxMemoryManager>::PoolMemoryManager(TAuxMemoryManager& auxiliary, uint8_t* buffer, size_t capacity)
    : _allocated(0),
      _allocations(0),
      _auxiliary(auxiliary),
      _external(false),
      _chunk(0),
      _chunks(0),
      _current(nullptr),
      _free_block(nullptr)
{
    reset(buffer, capacity);
}

template <class TAuxMemoryManager>
inline void* PoolMemoryManager<TAuxMemoryManager>::malloc(size_t size, size_t alignment)
{

}

template <class TAuxMemoryManager>
inline void PoolMemoryManager<TAuxMemoryManager>::free(void* ptr, size_t size)
{

}

template <class TAuxMemoryManager>
inline void PoolMemoryManager<TAuxMemoryManager>::reset()
{
    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    if (_current != nullptr)
    {
        // Reset the current memory pool chunk
        while (_current->prev != nullptr)
            _current = _current->prev;

        // Reset the free block pointer
        _free_block = _current->buffer;
        _free_block->size = chunk;
        _free_block->next = nullptr;
    }
}

template <class TAuxMemoryManager>
inline void PoolMemoryManager<TAuxMemoryManager>::reset(size_t chunk, size_t chunks)
{
    assert((chunk >= (sizeof(Chunk) + sizeof(FreeBlock))) && "Memory pool chunk must be big enough to fit at least one allocation block!");

    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    // Clear previous allocations
    clear();

    // Initialize the memory pool
    _external = false;
    _chunk = chunk;
    _chunks = (chunks > 0) ? chunks : std::numeric_limits<size_t>::max();

    // Allocate a new memory pool chunk
    Chunk* current = AllocateMemoryPool(chunk, _current);
    if (current != nullptr)
    {
        _current = current;

        // Initialize the free block pointer
        _free_block = _current->buffer;
        _free_block->size = chunk;
        _free_block->next = nullptr;

        --_chunks;
    }
}

template <class TAuxMemoryManager>
inline void PoolMemoryManager<TAuxMemoryManager>::reset(uint8_t* buffer, size_t capacity)
{
    assert((buffer != nullptr) && "Memory pool buffer must be valid!");
    assert((capacity >= (sizeof(Chunk) + sizeof(FreeBlock))) && "Memory pool buffer capacity must be big enough to fit at least one allocation block!");

    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    // Clear previous allocations
    clear();

    // Initialize the external memory pool
    _external = true;
    _chunk = capacity;
    _chunks = 1;
    _current = buffer;
    _current->buffer = buffer + sizeof(Chunk);
    _current->prev = nullptr;
    _current->next = nullptr;

    // Initialize the free block pointer
    _free_block = _current->buffer;
    _free_block->size = capacity - sizeof(Chunk);
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
    _chunk = 0;
    _chunks = 0;
    _current = nullptr;

    // Reset the free block pointer
    _free_block = nullptr;
}

template <class TAuxMemoryManager>
inline Chunk* PoolMemoryManager<TAuxMemoryManager>::AllocateMemoryPool(size_t capacity, Chunk* prev)
{
    // Allocate a new memory pool chunk
    uint8_t* buffer = (uint8_t*)_auxiliary.malloc(sizeof(Chunk) + capacity + alignof(std::max_align_t));
    Chunk* chunk = (Chunk*)buffer;
    if (chunk != nullptr)
    {
        // Prepare and return a new memory pool chunk
        chunk->buffer = buffer + sizeof(Chunk);
        chunk->prev = prev;
        chunk->next = nullptr;
        if (prev != nullptr)
            prev->next = chunk;
        return chunk;
    }

    // Out of memory...
    return nullptr;
}

template <class TAuxMemoryManager>
inline void PoolMemoryManager<TAuxMemoryManager>::ClearMemoryPool()
{
    if (!_external)
    {
        // Clear all memory pool chunks
        while (_current != nullptr)
        {
            Chunk* prev = _current->prev;
            _auxiliary.free(_current, sizeof(Chunk) + _chunk + alignof(std::max_align_t));
            _current = prev;
        }

        // Reset the free block pointer
        _free_block = nullptr;
    }
}

} // namespace CppCommon
