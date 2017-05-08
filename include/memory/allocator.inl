/*!
    \file allocator.inl
    \brief Memory allocator wrapper inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T, typename U, class TMemoryManager, bool nothrow>
inline bool operator==(const Allocator<T, TMemoryManager, nothrow>& alloc1, const Allocator<U, TMemoryManager, nothrow>& alloc2) noexcept
{
    return true;
}

template <typename T, typename U, class TMemoryManager, bool nothrow>
inline bool operator!=(const Allocator<T, TMemoryManager, nothrow>& alloc1, const Allocator<U, TMemoryManager, nothrow>& alloc2) noexcept
{
    return false;
}

template <typename T, class TMemoryManager, bool nothrow>
inline T* Allocator<T, TMemoryManager, nothrow>::allocate(size_t num, const void* hint)
{
    pointer result = (pointer)_manager.malloc(num * sizeof(T), hint);
    if (result != nullptr)
        return result;

    // Not enough memory...
    if (nothrow)
        return nullptr;
    else
        throw std::bad_alloc();
}

template <typename T, class TMemoryManager, bool nothrow>
inline void Allocator<T, TMemoryManager, nothrow>::deallocate(T* ptr, size_t num)
{
    _manager.free(ptr, num * sizeof(T));
}

inline void* DefaultMemoryManager::malloc(size_t size, const void* hint)
{
    assert((size > 0) && "Allocated block size must be greater than zero!");

    void* result = ::malloc(size);
    if (result != nullptr)
    {
        // Update allocation statistics
        _allocated += size;
        ++_allocations;
    }
    return result;
}

inline void DefaultMemoryManager::free(void* ptr, size_t size)
{
    assert((ptr != nullptr) && "Deallocated block must be valid!");

    if (ptr != nullptr)
    {
        ::free(ptr);

        // Update allocation statistics
        _allocated -= size;
        --_allocations;
    }
}

inline void DefaultMemoryManager::reset()
{
    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");
}

} // namespace CppCommon
