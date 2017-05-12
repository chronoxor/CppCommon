/*!
    \file allocator_arena.inl
    \brief Arena memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <class TAuxMemoryManager>
inline ArenaMemoryManager<TAuxMemoryManager>::ArenaMemoryManager(TAuxMemoryManager& auxiliary, size_t capacity)
    : _allocated(0),
      _allocations(0),
      _auxiliary(auxiliary),
      _current(nullptr),
      _reserved(0),
      _external(false),
      _buffer(nullptr),
      _capacity(0),
      _size(0)
{
    reset(capacity);
}

template <class TAuxMemoryManager>
inline ArenaMemoryManager<TAuxMemoryManager>::ArenaMemoryManager(TAuxMemoryManager& auxiliary, void* buffer, size_t capacity)
    : _allocated(0),
      _allocations(0),
      _auxiliary(auxiliary),
      _current(nullptr),
      _reserved(0),
      _external(true),
      _buffer(nullptr),
      _capacity(0),
      _size(0)
{
    reset(buffer, capacity);
}

template <class TAuxMemoryManager>
inline void* ArenaMemoryManager<TAuxMemoryManager>::malloc(size_t size, size_t alignment)
{
    assert((size > 0) && "Allocated block size must be greater than zero!");
    assert(Memory::IsValidAlignment(alignment) && "Alignment must be valid!");

    if (_external)
    {
        // Allocate memory from the external memory block
        uint8_t* buffer = _buffer + _size;
        uint8_t* aligned = Memory::Align(buffer, alignment);
        size_t aligned_size = size + (aligned - buffer);

        // Check if there is enough free space to allocate the block
        if (aligned_size <= (_capacity - _size))
        {
            // Memory allocated
            _size += aligned_size;

            // Update allocation statistics
            _allocated += size;
            ++_allocations;

            return aligned;
        }

        // Not enough memory... use auxiliary memory manager
        void* result = _auxiliary.malloc(size, alignment);
        if (result != nullptr)
        {
            // Update allocation statistics
            _allocated += size;
            ++_allocations;

            // Increase the required reserved memory size
            _reserved += size;
        }
        return result;
    }
    else
    {
        if (_current != nullptr)
        {
            // Allocate memory from the current arena chunk
            uint8_t* buffer = _current->buffer + _current->size;
            uint8_t* aligned = Memory::Align(buffer, alignment);
            size_t aligned_size = size + (aligned - buffer);

            // Check if there is enough free space to allocate the block
            if (aligned_size <= (_current->capacity - _current->size))
            {
                // Memory allocated
                _current->size += aligned_size;

                // Update allocation statistics
                _allocated += size;
                ++_allocations;

                return aligned;
            }
        }

        // Increase the required reserved memory size
        size_t next_reserved = 2 * _reserved;
        while (next_reserved < size)
            next_reserved *= 2;

        // Allocate a new arena chunk
        Chunk* current = AllocateArena(next_reserved, _current);
        if (current != nullptr)
        {
            // Update the current arena chunk
            _current = current;

            // Increase the required reserved memory size
            _reserved = next_reserved;

            // Allocate memory from the current arena chunk
            uint8_t* buffer = _current->buffer + _current->size;
            uint8_t* aligned = Memory::Align(buffer, alignment);
            size_t aligned_size = size + (aligned - buffer);

            // Memory allocated
            _current->size += aligned_size;

            // Update allocation statistics
            _allocated += size;
            ++_allocations;

            return aligned;
        }

        // Not enough memory...
        return nullptr;
    }
}

template <class TAuxMemoryManager>
inline void ArenaMemoryManager<TAuxMemoryManager>::free(void* ptr, size_t size)
{
    assert((ptr != nullptr) && "Deallocated block must be valid!");

    if (_external)
    {
        // Free memory block in auxiliary memory manager
        if ((ptr < _buffer) || (ptr >= (_buffer + _size)))
            _auxiliary.free(ptr, size);
    }
    else
    {
        // Free memory block in auxiliary memory manager
        if (_current == nullptr)
            _auxiliary.free(ptr, size);
    }

    // Update allocation statistics
    _allocated -= size;
    --_allocations;
}

template <class TAuxMemoryManager>
inline void ArenaMemoryManager<TAuxMemoryManager>::reset()
{
    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    // Expand internal arena buffer to fit auxiliary allocated storage
    if (!_external)
        reset(_reserved);

    _size = 0;
}

template <class TAuxMemoryManager>
inline void ArenaMemoryManager<TAuxMemoryManager>::reset(size_t capacity)
{
    assert((capacity > 0) && "Arena capacity must be greater than zero!");

    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    // Clear previous allocations
    clear();

    // Allocate a new arena chunk
    Chunk* current = AllocateArena(capacity, _current);
    if (current != nullptr)
        _current = current;

    // Set initial reserved memory size
    _reserved = capacity;
}

template <class TAuxMemoryManager>
inline void ArenaMemoryManager<TAuxMemoryManager>::reset(void* buffer, size_t capacity)
{
    assert((buffer != nullptr) && "Arena buffer must be valid!");
    assert((capacity > 0) && "Arena buffer capacity must be greater than zero!");

    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    // Clear previous allocations
    clear();

    // Initialize an external arena buffer
    _external = true;
    _buffer = (uint8_t*)buffer;
    _capacity = capacity;
    _size = 0;

    // Set initial reserved memory size
    _reserved = capacity;
}

template <class TAuxMemoryManager>
inline void ArenaMemoryManager<TAuxMemoryManager>::clear()
{
    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    // Clear arena
    ClearArena();

    // Clear external buffer
    _external = false;
    _buffer = nullptr;
    _capacity = 0;
    _size = 0;
}

template <class TAuxMemoryManager>
inline typename ArenaMemoryManager<TAuxMemoryManager>::Chunk* ArenaMemoryManager<TAuxMemoryManager>::AllocateArena(size_t capacity, Chunk* prev)
{
    // Allocate a new arena chunk
    uint8_t* buffer = (uint8_t*)_auxiliary.malloc(sizeof(Chunk) + capacity + alignof(std::max_align_t));
    Chunk* chunk = (Chunk*)buffer;
    if (chunk != nullptr)
    {
        // Prepare and return a new arena chunk
        chunk->buffer = buffer + sizeof(Chunk);
        chunk->capacity = capacity;
        chunk->size = 0;
        chunk->prev = prev;
        return chunk;
    }

    // Out of memory...
    return nullptr;
}

template <class TAuxMemoryManager>
inline void ArenaMemoryManager<TAuxMemoryManager>::ClearArena()
{
    if (!_external)
    {
        // Clear all arena chunks
        while (_current != nullptr)
        {
            Chunk* prev = _current->prev;
            _auxiliary.free(_current, sizeof(Chunk) + _current->capacity + alignof(std::max_align_t));
            _current = prev;
        }
    }
}

} // namespace CppCommon
