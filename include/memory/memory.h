/*!
    \file memory.h
    \brief Memory management definition
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_MEMORY_H
#define CPPCOMMON_MEMORY_MEMORY_H

#include <cassert>
#include <cstdint>
#include <cstring>

namespace CppCommon {

//! Memory management static class
/*!
    Provides memory management functionality such as total and free RAM available.

    Thread-safe.
*/
class Memory
{
public:
    Memory() = delete;
    Memory(const Memory&) = delete;
    Memory(Memory&&) = delete;
    ~Memory() = delete;

    Memory& operator=(const Memory&) = delete;
    Memory& operator=(Memory&&) = delete;

    //! Total RAM in bytes
    static int64_t RamTotal();
    //! Free RAM in bytes
    static int64_t RamFree();

    //! Is the given pointer aligned?
    /*!
        \param ptr - Pointer
        \param align - Must be a pointer of two (default is alignof(T))
        \return 'true' if the given pointer is aligned, 'false' if the given pointer is not aligned
    */
    template <typename T>
    static bool IsAligned(const T* ptr, size_t align = alignof(T)) noexcept;

    //! Align pointer (upwards or downwards)
    /*!
        \param ptr - Pointer
        \param align - Must be a pointer of two (default is alignof(T))
        \param upwards - Upwards flag (default is true)
        \return Aligned pointer
    */
    template <typename T>
    static T* Align(const T* ptr, size_t align = alignof(T), bool upwards = true) noexcept;
};

/*! \example memory_memory.cpp Memory management example */

} // namespace CppCommon

#include "memory.inl"

#endif // CPPCOMMON_MEMORY_MEMORY_H
