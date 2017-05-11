/*!
    \file allocator_null.inl
    \brief Null memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

inline void* NullMemoryManager::malloc(size_t size, size_t alignment)
{
    assert((size > 0) && "Allocated block size must be greater than zero!");
    assert(Memory::IsValidAlignment(alignment) && "Alignment must be valid!");

    // Not enough memory...
    return nullptr;
}

inline void NullMemoryManager::free(void* ptr, size_t size)
{
    assert((ptr != nullptr) && "Deallocated block must be valid!");

    if (ptr != nullptr)
    {
        // Update allocation statistics
        _allocated -= size;
        --_allocations;
    }
}

inline void NullMemoryManager::reset()
{
    assert((_allocated == 0) && "Memory leak detected! Allocated memory size must be zero!");
    assert((_allocations == 0) && "Memory leak detected! Count of active memory allocations must be zero!");
}

} // namespace CppCommon
