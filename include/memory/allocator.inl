/*!
    \file allocator.inl
    \brief Memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4127) // C4127: conditional expression is constant
#endif

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
    pointer result = (pointer)_manager.malloc(num * sizeof(T), alignof(T));
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

template <typename T, class TMemoryManager, bool nothrow>
template <typename U, class... Args>
inline void Allocator<T, TMemoryManager, nothrow>::construct(U* ptr, Args&&... args)
{
    assert((ptr != nullptr) && "Constructed element must be valid!");

    // Construct the element
    if (ptr != nullptr)
        new ((void*)ptr) U(std::forward<Args>(args)...);
}

template <typename T, class TMemoryManager, bool nothrow>
template <typename U>
inline void Allocator<T, TMemoryManager, nothrow>::destroy(U* ptr)
{
    assert((ptr != nullptr) && "Destroyed element must be valid!");

    // Destroy the element
    if (ptr != nullptr)
        ptr->~U();
}

template <typename T, class TMemoryManager, bool nothrow>
template <class... Args>
inline T* Allocator<T, TMemoryManager, nothrow>::Create(Args&&... args)
{
    // Allocate memory for the element
    void* ptr = _manager.malloc(sizeof(T));

    // Construct the element
    if (ptr != nullptr)
        new (ptr) T(std::forward<Args>(args)...);

    return (T*)ptr;
}

template <typename T, class TMemoryManager, bool nothrow>
inline void Allocator<T, TMemoryManager, nothrow>::Release(T* ptr)
{
    assert((ptr != nullptr) && "Released element must be valid!");

    // Release the element
    if (ptr != nullptr)
    {
        // Destroy the element
        ptr->~T();

        // Free element memory
        _manager.free(ptr, sizeof(T));
    }
}

template <typename T, class TMemoryManager, bool nothrow>
template <class... Args>
inline T* Allocator<T, TMemoryManager, nothrow>::CreateArray(size_t length, Args&&... args)
{
    assert((length > 0) && "Array length must be greater than zero!");

    size_t header = sizeof(size_t) / sizeof(T);
    if ((sizeof(size_t) % sizeof(T)) > 0)
        header += 1;

    // Allocate extra space to store array length in the bytes before the array
    T* ptr = ((T*)_manager.malloc(sizeof(T) * (length + header), alignof(T))) + header;

    // Store the array length value
    *(((size_t*)ptr) - 1) = length;

    // Construct array elements
    for (size_t i = 0; i < length; ++i)
        new (&ptr[i]) T(std::forward<Args>(args)...);

    return ptr;
}

template <typename T, class TMemoryManager, bool nothrow>
inline void Allocator<T, TMemoryManager, nothrow>::ReleaseArray(T* ptr)
{
    assert((ptr != nullptr) && "Released array must be valid!");

    // Release the array
    if (ptr != nullptr)
    {
        // Get the length of the array
        size_t length = *(((size_t*)ptr) - 1);

        // Destroy array elements
        for(size_t i = 0; i < length; ++i)
            ptr[i].~T();

        // Calculate how much extra memory was allocated to store the length before the array
        size_t header = sizeof(size_t) / sizeof(T);
        if ((sizeof(size_t) % sizeof(T)) > 0)
            header += 1;

        // Free array memory
        _manager.free(ptr - header, sizeof(T) * (length + header));
    }
}

inline void* DefaultMemoryManager::malloc(size_t size, size_t alignment)
{
    assert((size > 0) && "Allocated block size must be greater than zero!");
    assert(Memory::IsValidAlignment(alignment) && "Alignment must be valid!");

    void* result = std::malloc(size);
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
        std::free(ptr);

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

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
