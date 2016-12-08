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
        Close();
    }

    void* reader() const noexcept
    {
        return (void*)(size_t)_pipe[0];
    }

    void* writer() const noexcept
    {
        return (void*)(size_t)_pipe[1];
    }

    bool IsPipeOpened() const noexcept
    {
        return IsPipeReadOpened() || IsPipeWriteOpened();
    }

    bool IsPipeReadOpened() const noexcept
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return _pipe[0] != -1;
#elif defined(_WIN32) || defined(_WIN64)
        return _pipe[0] != INVALID_HANDLE_VALUE;
#endif
    }

    bool IsPipeWriteOpened() const noexcept
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return _pipe[1] != -1;
#elif defined(_WIN32) || defined(_WIN64)
        return _pipe[1] != INVALID_HANDLE_VALUE;
#endif
    }

    size_t Read(void* buffer, size_t size)
    {
        if (!IsPipeReadOpened())
            throwex SystemException("Cannot read from the closed pipe!");
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
        if (!IsPipeWriteOpened())
            throwex SystemException("Cannot write into the closed pipe!");
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

    void CloseRead()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        if (IsPipeReadOpened())
        {
            int result = close(_pipe[0]);
            if (result != 0)
                fatality(SystemException("Cannot close the read pipe endpoint!"));
            _pipe[0] = -1;
        }
#elif defined(_WIN32) || defined(_WIN64)
        if (IsPipeReadOpened())
        {
            if (!CloseHandle(_pipe[0]))
                fatality(SystemException("Cannot close the read pipe endpoint!"));
            _pipe[0] = INVALID_HANDLE_VALUE;
        }
#endif
    }

    void CloseWrite()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        if (IsPipeWriteOpened())
        {
            int result = close(_pipe[1]);
            if (result != 0)
                fatality(SystemException("Cannot close the write pipe endpoint!"));
            _pipe[1] = -1;
        }
#elif defined(_WIN32) || defined(_WIN64)
        if (IsPipeWriteOpened())
        {
            if (!CloseHandle(_pipe[1]))
                fatality(SystemException("Cannot close the write pipe endpoint!"));
            _pipe[1] = INVALID_HANDLE_VALUE;
        }
#endif
    }

    void Close()
    {
        CloseRead();
        CloseWrite();
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    int _pipe[2];
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE _pipe[2];
#endif
};

//! @endcond

Pipe::Pipe() : _pimpl(std::make_unique<Impl>())
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

void* Pipe::reader() const noexcept
{
    return _pimpl->reader();
}

void* Pipe::writer() const noexcept
{
    return _pimpl->writer();
}

bool Pipe::IsPipeOpened() const noexcept
{
    return _pimpl->IsPipeOpened();
}

bool Pipe::IsPipeReadOpened() const noexcept
{
    return _pimpl->IsPipeReadOpened();
}

bool Pipe::IsPipeWriteOpened() const noexcept
{
    return _pimpl->IsPipeWriteOpened();
}

size_t Pipe::Read(void* buffer, size_t size)
{
    return _pimpl->Read(buffer, size);
}

size_t Pipe::Write(const void* buffer, size_t size)
{
    return _pimpl->Write(buffer, size);
}

void Pipe::CloseRead()
{
    return _pimpl->CloseRead();
}

void Pipe::CloseWrite()
{
    return _pimpl->CloseWrite();
}

void Pipe::Close()
{
    return _pimpl->Close();
}

} // namespace CppCommon
