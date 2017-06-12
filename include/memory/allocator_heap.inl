/*!
    \file allocator_heap.inl
    \brief Heap memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 07.06.2017
    \copyright MIT License
*/

namespace CppCommon {

inline void* HeapMemoryManager::malloc(size_t size, size_t alignment)
{
    assert((size > 0) && "Allocated block size must be greater than zero!");
    assert(Memory::IsValidAlignment(alignment) && "Alignment must be valid!");

#if defined(_WIN32) || defined(_WIN64)
    void* result = HeapAlloc(GetProcessHeap(), 0, size);
#else
    void* result = std::malloc(size);
#endif
    if (result != nullptr)
    {
        // Update allocation statistics
        _allocated += size;
        ++_allocations;
    }
    return result;
}

inline void HeapMemoryManager::free(void* ptr, size_t size)
{
    assert((ptr != nullptr) && "Deallocated block must be valid!");

    if (ptr != nullptr)
    {
#if defined(_WIN32) || defined(_WIN64)
        HeapFree(GetProcessHeap(), 0, (LPVOID)ptr);
#else
        std::free(ptr);
#endif

        // Update allocation statistics
        _allocated -= size;
        --_allocations;
    }
}

inline void HeapMemoryManager::reset()
{
    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");
}

} // namespace CppCommon
