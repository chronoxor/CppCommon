/*!
    \file allocator.inl
    \brief Memory allocator wrapper inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T, class TMemoryManager>
inline Allocator<T, TMemoryManager>::~Allocator() noexcept
{
    assert((_allocations == 0) && "Memory leak detected! Allocations count must be zero!");
    assert((_allocated == 0) && "Memory leak detected! Allocated bytes must be zero!");
}

template <typename T, class TMemoryManager>
inline T* Allocator<T, TMemoryManager>::allocate(size_t num, const void* hint)
{
    pointer result = (pointer)_manager.allocate(num, hint);
    if (result != nullptr)
    {
        ++_allocations;
        _allocated += num * sizeof(T);
    }
    return result;
}

template <typename T, class TMemoryManager>
inline void Allocator<T, TMemoryManager>::deallocate(T* ptr, size_t num)
{
    _manager.deallocate(ptr, num);
    if (ptr != nullptr)
    {
        --_allocations;
        _allocated -= num * sizeof(T);
    }
}

template <bool nothrow>
inline void* DefaultMemoryManager<nothrow>::allocate(size_t num, const void* hint)
{
    assert((num > 0) && "Allocated block size must be greater than zero!");

    void* result = malloc(num);
    if (result != nullptr)
        return result;

    if (nothrow)
        return nullptr;
    else
        throw std::bad_alloc();
}

template <bool nothrow>
void DefaultMemoryManager<nothrow>::deallocate(void* ptr, size_t num)
{
    assert((ptr != nullptr) && "Deallocated block must be valid!");

    free(ptr);
}

} // namespace CppCommon
