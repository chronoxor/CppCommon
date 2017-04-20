/*!
    \file allocator_hybrid.inl
    \brief Hybrid memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <class TAuxMemoryManager, bool nothrow, std::size_t alignment>
inline HybridMemoryManager<TAuxMemoryManager, nothrow, alignment>::HybridMemoryManager(TAuxMemoryManager& auxiliary, size_t capacity)
    : _auxiliary(auxiliary),
      _auxiliary_allocated(0),
      _external(false),
      _buffer(nullptr),
      _capacity(0),
      _size(0)
{
    reset(capacity);
}

template <class TAuxMemoryManager, bool nothrow, std::size_t alignment>
inline HybridMemoryManager<TAuxMemoryManager, nothrow, alignment>::HybridMemoryManager(TAuxMemoryManager& auxiliary, uint8_t* buffer, size_t size)
    : _auxiliary(auxiliary),
      _auxiliary_allocated(0),
      _external(true),
      _buffer(nullptr),
      _capacity(0),
      _size(0)
{
    reset(buffer, size);
}

template <class TAuxMemoryManager, bool nothrow, std::size_t alignment>
inline HybridMemoryManager<TAuxMemoryManager, nothrow, alignment>::~HybridMemoryManager()
{
    if (!_external && (_buffer != nullptr))
        _auxiliary.deallocate(_buffer, _capacity);
}

template <class TAuxMemoryManager, bool nothrow, std::size_t alignment>
inline void* HybridMemoryManager<TAuxMemoryManager, nothrow, alignment>::allocate(size_t num, const void* hint)
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

    // Not enough free memory... use auxiliary memory manager
    void* result = _auxiliary.allocate(num, hint);
    _auxiliary_allocated += num;
    return result;
}

template <class TAuxMemoryManager, bool nothrow, std::size_t alignment>
inline void HybridMemoryManager<TAuxMemoryManager, nothrow, alignment>::deallocate(void* ptr, size_t num)
{
    assert((ptr != nullptr) && "Deallocated block must be valid!");

    // Free memory block in auxiliary memory manager
    if ((ptr < _buffer) || (ptr >= (_buffer + _size)))
        _auxiliary.deallocate(ptr, num);
}

template <class TAuxMemoryManager, bool nothrow, std::size_t alignment>
inline void HybridMemoryManager<TAuxMemoryManager, nothrow, alignment>::reset()
{
    // Expand internal arena buffer to fit auxiliary allocated storage
    if (!_external && (_auxiliary_allocated > 0))
        reset(_capacity + _auxiliary_allocated);

    _auxiliary_allocated = 0;
    _size = 0;
}

template <class TAuxMemoryManager, bool nothrow, std::size_t alignment>
inline void HybridMemoryManager<TAuxMemoryManager, nothrow, alignment>::reset(size_t capacity)
{
    assert((capacity > 0) && "Arena capacity must be greater than zero!");

    if (!_external && (_buffer != nullptr))
        _auxiliary.deallocate(_buffer, _capacity);

    _auxiliary_allocated = 0;
    _external = false;
    _buffer = (uint8_t*)_auxiliary.allocate(capacity);
    _capacity = capacity;
    _size = 0;
}

template <class TAuxMemoryManager, bool nothrow, std::size_t alignment>
inline void HybridMemoryManager<TAuxMemoryManager, nothrow, alignment>::reset(uint8_t* buffer, size_t size)
{
    assert((buffer != nullptr) && "Arena buffer must be valid!");
    assert((size > 0) && "Arena buffer size must be greater than zero!");

    if (!_external && (_buffer != nullptr))
        _auxiliary.deallocate(_buffer, _capacity);

    _auxiliary_allocated = 0;
    _external = true;
    _buffer = buffer;
    _capacity = size;
    _size = 0;
}

} // namespace CppCommon
