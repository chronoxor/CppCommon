/*!
    \file allocator_arena.inl
    \brief Arena memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <class TAuxMemoryManager, std::size_t alignment>
inline ArenaMemoryManager<TAuxMemoryManager, alignment>::ArenaMemoryManager(TAuxMemoryManager& auxiliary, size_t capacity)
    : _auxiliary(auxiliary),
      _current(nullptr),
      _reserved(0),
      _external(false),
      _buffer(nullptr),
      _capacity(0),
      _size(0)
{
    reset(capacity);
}

template <class TAuxMemoryManager, std::size_t alignment>
inline ArenaMemoryManager<TAuxMemoryManager, alignment>::ArenaMemoryManager(TAuxMemoryManager& auxiliary, uint8_t* buffer, size_t size)
    : _auxiliary(auxiliary),
      _current(nullptr),
      _reserved(0),
      _external(true),
      _buffer(nullptr),
      _capacity(0),
      _size(0)
{
    reset(buffer, size);
}

template <class TAuxMemoryManager, std::size_t alignment>
inline ArenaMemoryManager<TAuxMemoryManager, alignment>::~ArenaMemoryManager()
{
    clear();
}

template <class TAuxMemoryManager, std::size_t alignment>
inline void* ArenaMemoryManager<TAuxMemoryManager, alignment>::malloc(size_t size, const void* hint)
{
    assert((size > 0) && "Allocated block size must be greater than zero!");

    if (_external)
    {
        uint8_t* buffer = _buffer + _size;
        uint8_t* aligned = Memory::Align(buffer, alignment);
        size_t aligned_size = size + (aligned - buffer);

        // Check if there is enough free space to allocate the block
        if (aligned_size <= (_capacity - _size))
        {
            _size += aligned_size;
            return aligned;
        }

        // Not enough memory... use auxiliary memory manager
        return _auxiliary.malloc(size, hint);
    }
    else
    {
        if (_current != nullptr)
        {
            uint8_t* buffer = _current->buffer + _current->size;
            uint8_t* aligned = Memory::Align(buffer, alignment);
            size_t aligned_size = size + (aligned - buffer);

            // Check if there is enough free space to allocate the block
            if (aligned_size <= (_current->capacity - _current->size))
            {
                _current->size += aligned_size;
                return aligned;
            }
        }

        // Increase the required reserved memory size
        size_t aligned_size = Memory::Align(size, alignment);
        size_t next_reserved = 2 * _reserved;
        while (next_reserved < aligned_size)
            next_reserved *= 2;

        // Allocate a new arena chunk
        Chunk* current = AllocateArena(_current, next_reserved);
        if (current != nullptr)
        {
            // Increase the required reserved memory size
            _reserved = next_reserved;

            // Update the current arena chunk
            _current = current;
            _current->size += aligned_size;

            // Return an aligned memory buffer from the current arena chunk
            return Memory::Align(_current->buffer, alignment);
        }

        // Not enough memory...
        return nullptr;
    }
}

template <class TAuxMemoryManager, std::size_t alignment>
inline void ArenaMemoryManager<TAuxMemoryManager, alignment>::free(void* ptr, size_t size)
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

}

template <class TAuxMemoryManager, std::size_t alignment>
inline void ArenaMemoryManager<TAuxMemoryManager, alignment>::reset()
{
    // Expand internal arena buffer to fit auxiliary allocated storage
    if (!_external)
        reset(_reserved);

    _size = 0;
}

template <class TAuxMemoryManager, std::size_t alignment>
inline void ArenaMemoryManager<TAuxMemoryManager, alignment>::reset(size_t capacity)
{
    assert((capacity > 0) && "Arena capacity must be greater than zero!");

    // Clear previous allocations
    clear();

    // Allocate arena chunk
    Chunk* current = AllocateArena(_current, capacity);
    if (current != nullptr)
        _current = current;

    // Set initial reserved memory size
    _reserved = capacity;
}

template <class TAuxMemoryManager, std::size_t alignment>
inline void ArenaMemoryManager<TAuxMemoryManager, alignment>::reset(uint8_t* buffer, size_t size)
{
    assert((buffer != nullptr) && "Arena buffer must be valid!");
    assert((size > 0) && "Arena buffer size must be greater than zero!");

    // Clear previous allocations
    clear();

    _external = true;
    _buffer = buffer;
    _capacity = size;
    _size = 0;
}

template <class TAuxMemoryManager, std::size_t alignment>
inline void ArenaMemoryManager<TAuxMemoryManager, alignment>::clear()
{
    // Clear arena
    ClearArena();

    // Clear external buffer
    _external = false;
    _buffer = nullptr;
    _capacity = 0;
    _size = 0;

    // Reset reserved memory size
    _reserved = 0;
}

template <class TAuxMemoryManager, std::size_t alignment>
inline typename ArenaMemoryManager<TAuxMemoryManager, alignment>::Chunk* ArenaMemoryManager<TAuxMemoryManager, alignment>::AllocateArena(Chunk* prev, size_t capacity)
{
    // Calculate required alignment
    size_t align = std::max(alignment, alignof(Chunk));

    // Calculate header size
    size_t header = Memory::Align(sizeof(Chunk), align);

    // Allocate a new arena chunk
    uint8_t* buffer = (uint8_t*)_auxiliary.malloc(header + capacity, prev);
    Chunk* chunk = (Chunk*)buffer;
    if (chunk != nullptr)
    {
        // Prepare and return a new arena chunk
        chunk->buffer = buffer + header;
        chunk->capacity = capacity;
        chunk->size = 0;
        chunk->prev = prev;
        return chunk;
    }

    // Out of memory...
    return nullptr;
}

template <class TAuxMemoryManager, std::size_t alignment>
inline void ArenaMemoryManager<TAuxMemoryManager, alignment>::ClearArena()
{
    if (!_external)
    {
        // Calculate required alignment
        size_t align = std::max(alignment, alignof(Chunk));

        // Calculate header size
        size_t header = Memory::Align(sizeof(Chunk), align);

        Chunk* current = _current;
        while (current != nullptr)
        {
            Chunk* prev = current->prev;
            _auxiliary.free(current, header + current->capacity);
            current = prev;
        }
        _current = nullptr;
    }
}

} // namespace CppCommon
