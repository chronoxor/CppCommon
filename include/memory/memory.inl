/*!
    \file memory.inl
    \brief Memory management inline implementation
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#include <cstdint>

namespace CppCommon {

inline bool Memory::IsValidAlignment(size_t alignment) noexcept
{
    return ((alignment > 0) && ((alignment & (alignment - 1)) == 0));
}

template <typename T>
inline bool Memory::IsAligned(const T* ptr, size_t alignment) noexcept
{
    assert((ptr != nullptr) && "Pointer must be valid!");
    assert(IsValidAlignment(alignment) && "Alignment must be valid!");

    uintptr_t address = (uintptr_t)ptr;
    return (address & (alignment - 1)) == 0;
}

template <typename T>
inline T* Memory::Align(const T* ptr, size_t alignment, bool upwards) noexcept
{
    assert((ptr != nullptr) && "Pointer must be valid!");
    assert(IsValidAlignment(alignment) && "Alignment must be valid!");

    uintptr_t address = (uintptr_t)ptr;

    if (upwards)
        return (T*)((address + (alignment - 1)) & -((int)alignment));
    else
        return (T*)(address & -((int)alignment));
}

} // namespace CppCommon
