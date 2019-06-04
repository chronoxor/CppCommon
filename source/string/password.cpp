/*!
    \file password.cpp
    \brief Password string implementation
    \author Ivan Shynkarenka
    \date 04.06.2019
    \copyright MIT License
*/

#include "string/password.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
#include <string.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

void ZeroPasswordMemory(void* buffer, size_t size)
{
#if defined(__APPLE__)
    volatile char* ptr = (volatile char*)buffer;
    while (size--) *ptr++ = 0;
#elif defined(linux) || defined(__linux) || defined(__linux__)
    memset(buffer, 0, size);
    asm volatile("" ::: "memory");
#elif defined(_WIN32) || defined(_WIN64)
    SecureZeroMemory(buffer, size);
#else
    #error Unsupported platform
#endif
}

} // namespace CppCommon
