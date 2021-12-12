/*!
    \file system_error.cpp
    \brief System error implementation
    \author Ivan Shynkarenka
    \date 10.02.2016
    \copyright MIT License
*/

#include "errors/system_error.h"

#include "string/encoding.h"

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include "string/format.h"
#include <errno.h>
#include <string.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

int SystemError::GetLast() noexcept
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    return errno;
#elif defined(_WIN32) || defined(_WIN64)
    return GetLastError();
#endif
}

void SystemError::SetLast(int error) noexcept
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    errno = error;
#elif defined(_WIN32) || defined(_WIN64)
    return SetLastError(error);
#endif
}

void SystemError::ClearLast() noexcept
{
    SetLast(0);
}

std::string SystemError::Description(int error)
{
    const int capacity = 1024;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    char buffer[capacity];
#if defined(__APPLE__) || defined(__CYGWIN__)
    int result = strerror_r(error, buffer, capacity);
    if (result != 0)
#else
    char* result = strerror_r(error, buffer, capacity);
    if (result == nullptr)
#endif
        return format("Cannot convert the given system error code to the system message - {}", error);
    else
        return std::string(buffer);
#elif defined(_WIN32) || defined(_WIN64)
    WCHAR buffer[capacity];
    DWORD size = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, capacity, nullptr);
    return Encoding::ToUTF8((size >= 2) ? std::wstring(buffer, (size - 2)) : std::wstring(L"Unknown system error!"));
#endif
}

} // namespace CppCommon
