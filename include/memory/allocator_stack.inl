/*!
    \file allocator_stack.inl
    \brief Stack memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <size_t N>
inline StackMemoryManager<N>::StackMemoryManager()
    : _allocated(0),
      _allocations(0),
      _capacity(N),
      _size(0)
{
}

template <size_t N>
inline void* StackMemoryManager<N>::malloc(size_t size, size_t alignment)
{
    assert((size > 0) && "Allocated block size must be greater than zero!");
    assert(Memory::IsValidAlignment(alignment) && "Alignment must be valid!");

    // Allocate memory from the stack buffer
    uint8_t* buffer = _buffer + _size;
    uint8_t* aligned = Memory::Align(buffer, alignment);

    // Check if there is enough free space to allocate the block
    if ((size + (aligned - buffer)) <= (_capacity - _size))
    {
        // Memory allocated
        _size += size;

        // Update allocation statistics
        _allocated += size;
        ++_allocations;

        return aligned;
    }

    // Not enough memory...
    return nullptr;
}

template <size_t N>
inline void StackMemoryManager<N>::free(void* ptr, size_t size)
{
    assert((ptr != nullptr) && "Deallocated block must be valid!");

    if (ptr != nullptr)
    {
        // Update allocation statistics
        _allocated -= size;
        --_allocations;
    }
}

template <size_t N>
inline void StackMemoryManager<N>::reset()
{
    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");

    _size = 0;
}

} // namespace CppCommon
