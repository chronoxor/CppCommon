/*!
    \file memory.inl
    \brief Memory management inline implementation
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#include <cstdint>

namespace CppCommon {

template <typename T>
inline T* Memory::Align(const T* ptr, size_t align, bool upwards)
{
    assert((ptr != nullptr) && "Pointer must be valid!");
    assert((align > 0) && "Align must be greater than zero!");
    assert(((align & (align - 1)) == 0) && "Align must be a power of two!");

    uintptr_t address = (uintptr_t)ptr;

    if (upwards)
        return (T*)((address + (align - 1)) & -((int)align));
    else
        return (T*)(address & -((int)align));
}

} // namespace CppCommon
