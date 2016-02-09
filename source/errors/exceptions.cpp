/*!
    \file exceptions.cpp
    \brief Exceptions implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#include "errors/exceptions.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <errno.h>
#endif

namespace CppCommon {

#if defined(_WIN32) || defined(_WIN64)
SystemException::SystemException(const std::string& message) : SystemException(GetLastError(), message) {}
#elif defined(unix) || defined(__unix) || defined(__unix__)
SystemException::SystemException(const std::string& message) : SystemException(errno, message) {}
#endif

SystemException::SystemException(int error, const std::string& message) : Exception(message)
{
    _message.append("\nSystem error: " + std::to_string(error));
#if defined(_WIN32) || defined(_WIN64)
    LPSTR buffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0, nullptr);
    if (buffer != nullptr)
    {
        _message.append("\nSystem message: " + std::string(buffer, size));
        LocalFree(buffer);
    }
#elif defined(unix) || defined(__unix) || defined(__unix__)
    const int size = 1024;
    char buffer[size];
    int result = strerror_r(error, buffer, size);
    if (result == EINVAL) 
        _message.append("\nSystem message: System error is not a valid error number!");
    else if (result == ERANGE) 
        _message.append("\nSystem message: Insufficient storage was supplied to contain the error description string!");
    else
        _message.append("\nSystem message: " + std::string(buffer));
#endif
}

} // namespace CppCommon
