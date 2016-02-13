/*!
    \file system_error.cpp
    \brief System error wrapper implementation
    \author Ivan Shynkarenka
    \date 10.02.2016
    \copyright MIT License
*/

#include "errors/system_error.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <errno.h>
#endif

namespace CppCommon {

int SystemError::GetLast()
{
#if defined(_WIN32) || defined(_WIN64)
    return GetLastError();
#elif defined(unix) || defined(__unix) || defined(__unix__)
    return errno;
#endif
}

void SystemError::SetLast(int error)
{
#if defined(_WIN32) || defined(_WIN64)
    return SetLastError(error);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    errno = error;
#endif
}

void SystemError::ClearLast()
{
    SetLast(0);
}

std::string SystemError::Convert(int error)
{
    const int capacity = 1024;
    char buffer[capacity];
#if defined(_WIN32) || defined(_WIN64)
    DWORD size = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, capacity, nullptr);
    return std::string(buffer, size);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    char* result = strerror_r(error, buffer, capacity);
    if (result == nullptr)
        return std::string("Cannot convert the given system error code to the system message - " + std::to_string(error));
    else
        return std::string(result);
#endif
}

} // namespace CppCommon
