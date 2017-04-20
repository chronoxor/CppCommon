/*!
    \file allocator_arena.inl
    \brief Arena memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <bool nothrow, std::size_t alignment>
inline ArenaMemoryManager<nothrow, alignment>::ArenaMemoryManager(size_t capacity)
    : _external(false),
      _buffer(nullptr),
      _capacity(0),
      _size(0)
{
    reset(capacity);
}

template <bool nothrow, std::size_t alignment>
inline ArenaMemoryManager<nothrow, alignment>::ArenaMemoryManager(uint8_t* buffer, size_t size)
    : _external(true),
      _buffer(nullptr),
      _capacity(0),
      _size(0)
{
    reset(buffer, size);
}

template <bool nothrow, std::size_t alignment>
inline ArenaMemoryManager<nothrow, alignment>::~ArenaMemoryManager()
{
    if (!_external && (_buffer != nullptr))
        free(_buffer);
}

template <bool nothrow, std::size_t alignment>
inline void* ArenaMemoryManager<nothrow, alignment>::allocate(size_t num, const void* hint)
{
    assert((num > 0) && "Allocated block size must be greater than zero!");

    uint8_t* buffer = _buffer + _size;
    uint8_t* aligned = Memory::Align(buffer, alignment);

    // Check if there is enough free space to allocate the block
    if ((num + (aligned - buffer)) <= (_capacity - _size))
    {
        _size += num;
        return aligned;
    }

    // Not enough free memory...
    if (nothrow)
        return nullptr;
    else
        throw std::bad_alloc();
}

template <bool nothrow, std::size_t alignment>
inline void ArenaMemoryManager<nothrow, alignment>::deallocate(void* ptr, size_t num)
{
    assert((ptr != nullptr) && "Deallocated block must be valid!");
}

template <bool nothrow, std::size_t alignment>
inline void ArenaMemoryManager<nothrow, alignment>::reset()
{
    _size = 0;
}

template <bool nothrow, std::size_t alignment>
inline void ArenaMemoryManager<nothrow, alignment>::reset(size_t capacity)
{
    assert((capacity > 0) && "Arena capacity must be greater than zero!");

    if (!_external && (_buffer != nullptr))
        free(_buffer);

    _external = false;
    _buffer = (uint8_t*)malloc(capacity);
    _capacity = capacity;
    _size = 0;
}

template <bool nothrow, std::size_t alignment>
inline void ArenaMemoryManager<nothrow, alignment>::reset(uint8_t* buffer, size_t size)
{
    assert((buffer != nullptr) && "Arena buffer must be valid!");
    assert((size > 0) && "Arena buffer size must be greater than zero!");

    if (!_external && (_buffer != nullptr))
        free(_buffer);

    _external = true;
    _buffer = buffer;
    _capacity = size;
    _size = 0;
}

} // namespace CppCommon
