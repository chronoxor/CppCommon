/*!
    \file rdts.cpp
    \brief Read Time Stamp Counter implementation
    \author Ivan Shynkarenka
    \date 22.01.2016
    \copyright MIT License
*/

#include "system/rdts.h"

#if defined(_MSC_VER)
#include <windows.h>
#endif

namespace CppCommon {

uint64_t rdts() noexcept
{
#if defined(_MSC_VER)
    return __rdtsc();
#elif defined(__i386__)
    uint64_t x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
#elif defined(__x86_64__)
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)lo) | (((uint64_t)hi) << 32);
#endif
}

} // namespace CppCommon
