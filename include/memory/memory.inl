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
inline bool Memory::IsAligned(const T* address, size_t alignment) noexcept
{
    assert((address != nullptr) && "Address must be valid!");
    assert(IsValidAlignment(alignment) && "Alignment must be valid!");

    uintptr_t ptr = (uintptr_t)address;
    return (ptr & (alignment - 1)) == 0;
}

template <typename T>
inline T* Memory::Align(const T* address, size_t alignment, bool upwards) noexcept
{
    assert((address != nullptr) && "Address must be valid!");
    assert(IsValidAlignment(alignment) && "Alignment must be valid!");

    uintptr_t ptr = (uintptr_t)address;

    if (upwards)
        return (T*)((ptr + (alignment - 1)) & -((int)alignment));
    else
        return (T*)(ptr & -((int)alignment));
}

} // namespace CppCommon
