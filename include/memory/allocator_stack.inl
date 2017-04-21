/*!
    \file allocator_stack.inl
    \brief Stack memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <size_t N, std::size_t alignment>
inline StackMemoryManager<N, alignment>::StackMemoryManager()
    : _capacity(N),
      _size(0)
{
}

template <size_t N, std::size_t alignment>
inline void* StackMemoryManager<N, alignment>::malloc(size_t size, const void* hint)
{
    assert((size > 0) && "Allocated block size must be greater than zero!");

    uint8_t* buffer = _buffer + _size;
    uint8_t* aligned = Memory::Align(buffer, alignment);

    // Check if there is enough free space to allocate the block
    if ((size + (aligned - buffer)) <= (_capacity - _size))
    {
        _size += size;
        return aligned;
    }

    // Not enough memory...
    return nullptr;
}

template <size_t N, std::size_t alignment>
inline void StackMemoryManager<N, alignment>::free(void* ptr, size_t size)
{
    assert((ptr != nullptr) && "Deallocated block must be valid!");
}

template <size_t N, std::size_t alignment>
inline void StackMemoryManager<N, alignment>::reset()
{
    _size = 0;
}

} // namespace CppCommon
