/*!
    \file password.cpp
    \brief Password string implementation
    \author Ivan Shynkarenka
    \date 04.06.2019
    \copyright MIT License
*/

#include "string/password.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

void ZeroPasswordMemory(void* buffer, size_t size)
{
#if defined(_WIN32) || defined(_WIN64)
    SecureZeroMemory(buffer, size);
#else
    volatile char* ptr = (volatile char*)buffer;
    while (size--)
        *ptr++ = 0;
#endif
}

} // namespace CppCommon
