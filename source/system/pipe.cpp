/*!
    \file pipe.cpp
    \brief Pipe implementation
    \author Ivan Shynkarenka
    \date 30.11.2016
    \copyright MIT License
*/

#include "system/pipe.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

//! @cond INTERNALS

class Pipe::Impl
{
public:
    Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pipe(_pipe);
        if (result != 0)
            throwex SystemException("Failed to create a new pipe!");
#elif defined(_WIN32) || defined(_WIN64)
        if (!CreatePipe(&_pipe[0], &_pipe[1], nullptr, 0))
            throwex SystemException("Failed to create a new pipe!");
#endif
    }

    ~Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result1 = close(_pipe[0]);
        int result2 = close(_pipe[1]);
        if ((result1 != 0) || (result2 != 0))
            fatality(SystemException("Cannot close the pipe!"));
#elif defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_pipe[0]) || !CloseHandle(_pipe[1]))
            fatality(SystemException("Cannot close the pipe!"));
#endif
    }

    size_t Read(void* buffer, size_t size)
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        ssize_t result = read(_pipe[0], buffer, size);
        if (result < 0)
            throwex SystemException("Cannot read from the pipe!");
        return (size_t)result;
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result;
        if (!ReadFile(_pipe[0], buffer, (DWORD)size, &result, nullptr))
            throwex SystemException("Cannot read from the pipe!");
        return (size_t)result;
#endif
    }

    size_t Write(const void* buffer, size_t size)
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        ssize_t result = write(_pipe[1], buffer, size);
        if (result < 0)
            throwex SystemException("Cannot write into the pipe!");
        return (size_t)result;
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result;
        if (!WriteFile(_pipe[1], buffer, (DWORD)size, &result, nullptr))
            throwex SystemException("Cannot write into the pipe!");
        return (size_t)result;
#endif
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    int _pipe[2];
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE _pipe[2];
#endif
};

//! @endcond

Pipe::Pipe() : _pimpl(new Impl())
{
}

Pipe::Pipe(Pipe&& pipe) noexcept : _pimpl(std::move(pipe._pimpl))
{
}

Pipe::~Pipe()
{
}

Pipe& Pipe::operator=(Pipe&& pipe) noexcept
{
    _pimpl = std::move(pipe._pimpl);
    return *this;
}

size_t Pipe::Read(void* buffer, size_t size)
{
    return _pimpl->Read(buffer, size);
}

size_t Pipe::Write(const void* buffer, size_t size)
{
    return _pimpl->Write(buffer, size);
}

} // namespace CppCommon
