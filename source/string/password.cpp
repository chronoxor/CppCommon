/*!
    \file password.cpp
    \brief Password string implementation
    \author Ivan Shynkarenka
    \date 04.06.2019
    \copyright MIT License
*/

#include "string/password.h"

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdlib.h>
#include <string.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

void ZeroPasswordMemory(void* buffer, size_t size)
{
#ifdef __STDC_LIB_EXT1__
    memset_s(buffer, size, 0, size);
#elif defined(_WIN32) || defined(_WIN64)
    SecureZeroMemory(buffer, size);
#else
    volatile char* ptr = (volatile char*)buffer;
    while (size--)
        *ptr++ = 0;
#endif
}

} // namespace CppCommon
