/*!
    \file allocator.inl
    \brief Memory allocator wrapper inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T, class TMemoryManager, bool nothrow>
inline T* Allocator<T, TMemoryManager, nothrow>::allocate(size_t num, const void* hint)
{
    pointer result = (pointer)_manager.allocate(num, hint);
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
    _manager.deallocate(ptr, num);
}

inline void* DefaultMemoryManager::allocate(size_t num, const void* hint)
{
    assert((num > 0) && "Allocated block size must be greater than zero!");

    return malloc(num);
}

void DefaultMemoryManager::deallocate(void* ptr, size_t num)
{
    assert((ptr != nullptr) && "Deallocated block must be valid!");

    free(ptr);
}

} // namespace CppCommon
