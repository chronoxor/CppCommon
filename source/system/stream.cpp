/*!
    \file stream.cpp
    \brief Standard input/output/error stream implementation
    \author Ivan Shynkarenka
    \date 23.07.2017
    \copyright MIT License
*/

#include "system/stream.h"

#include "errors/fatal.h"

#include <cassert>

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <stdio.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

//! @cond INTERNALS

class StdInput::Impl
{
public:
    Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        _stream = stdin;
        if (_stream < 0)
            throwex SystemException("Failed to get a valid standard input stream!");
#elif defined(_WIN32) || defined(_WIN64)
        _stream = GetStdHandle(STD_INPUT_HANDLE);
        if (_stream == INVALID_HANDLE_VALUE)
            throwex SystemException("Failed to get a valid standard input stream!");
#endif
    }

    ~Impl() {}

    void* stream() const noexcept
    {
        return _stream;
    }

    bool IsValid() const noexcept
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return (_stream >= 0);
#elif defined(_WIN32) || defined(_WIN64)
        return (_stream != INVALID_HANDLE_VALUE);
#endif
    }

    size_t Read(void* buffer, size_t size)
    {
        assert((buffer != nullptr) && "Pointer to the buffer should not be equal to 'nullptr'!");
        assert((size > 0) && "Buffer size should be greater than zero!");
        if ((buffer == nullptr) || (size == 0))
            return 0;

        assert(IsValid() && "Standard input stream is not valid!");
        if (!IsValid())
            throwex SystemException("Cannot read from the invalid standard input stream!");
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        size_t result = fread(buffer, 1, size, _stream);
        if (ferror(_stream) != 0)
            throwex SystemException("Cannot read from the standard input stream!");
        return result;
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result;
        if (!ReadFile(_stream, buffer, (DWORD)size, &result, nullptr))
            throwex SystemException("Cannot read from the standard input stream!");
        return (size_t)result;
#endif
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    FILE* _stream;
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE _stream;
#endif
};

class StdOutput::Impl
{
public:
    Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        _stream = stdout;
        if (_stream < 0)
            throwex SystemException("Failed to get a valid standard output stream!");
#elif defined(_WIN32) || defined(_WIN64)
        _stream = GetStdHandle(STD_OUTPUT_HANDLE);
        if (_stream == INVALID_HANDLE_VALUE)
            throwex SystemException("Failed to get a valid standard output stream!");
#endif
    }

    ~Impl() {}

    void* stream() const noexcept
    {
        return _stream;
    }

    bool IsValid() const noexcept
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return (_stream >= 0);
#elif defined(_WIN32) || defined(_WIN64)
        return (_stream != INVALID_HANDLE_VALUE);
#endif
    }

    size_t Write(const void* buffer, size_t size)
    {
        assert((buffer != nullptr) && "Pointer to the buffer should not be equal to 'nullptr'!");
        assert((size > 0) && "Buffer size should be greater than zero!");
        if ((buffer == nullptr) || (size == 0))
            return 0;

        assert(IsValid() && "Standard output stream is not valid!");
        if (!IsValid())
            throwex SystemException("Cannot write into the invalid standard output stream!");
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        size_t result = fwrite(buffer, 1, size, _stream);
        if (ferror(_stream) != 0)
            throwex SystemException("Cannot write into the standard output stream!");
        return result;
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result;
        if (!WriteFile(_stream, buffer, (DWORD)size, &result, nullptr))
            throwex SystemException("Cannot write into the standard output stream!");
        return (size_t)result;
#endif
    }

    void Flush()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = fflush(_stream);
        if (result != 0)
            throwex SystemException("Cannot flush the standard output stream!");
#elif defined(_WIN32) || defined(_WIN64)
        if (!FlushFileBuffers(_stream))
            throwex SystemException("Cannot flush the standard output stream!");
#endif
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    FILE* _stream;
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE _stream;
#endif
};

class StdError::Impl
{
public:
    Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        _stream = stderr;
        if (_stream < 0)
            throwex SystemException("Failed to get a valid standard error stream!");
#elif defined(_WIN32) || defined(_WIN64)
        _stream = GetStdHandle(STD_ERROR_HANDLE);
        if (_stream == INVALID_HANDLE_VALUE)
            throwex SystemException("Failed to get a valid standard error stream!");
#endif
    }

    ~Impl() {}

    void* stream() const noexcept
    {
        return _stream;
    }

    bool IsValid() const noexcept
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return (_stream >= 0);
#elif defined(_WIN32) || defined(_WIN64)
        return (_stream != INVALID_HANDLE_VALUE);
#endif
    }

    size_t Write(const void* buffer, size_t size)
    {
        assert((buffer != nullptr) && "Pointer to the buffer should not be equal to 'nullptr'!");
        assert((size > 0) && "Buffer size should be greater than zero!");
        if ((buffer == nullptr) || (size == 0))
            return 0;

        assert(IsValid() && "Standard error stream is not valid!");
        if (!IsValid())
            throwex SystemException("Cannot write into the invalid standard error stream!");
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        size_t result = fwrite(buffer, 1, size, _stream);
        if (ferror(_stream) != 0)
            throwex SystemException("Cannot write into the standard error stream!");
        return result;
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result;
        if (!WriteFile(_stream, buffer, (DWORD)size, &result, nullptr))
            throwex SystemException("Cannot write into the standard error stream!");
        return (size_t)result;
#endif
    }

    void Flush()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = fflush(_stream);
        if (result != 0)
            throwex SystemException("Cannot flush the standard error stream!");
#elif defined(_WIN32) || defined(_WIN64)
        if (!FlushFileBuffers(_stream))
            throwex SystemException("Cannot flush the standard error stream!");
#endif
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    FILE* _stream;
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE _stream;
#endif
};

//! @endcond

StdInput::StdInput() : _pimpl(std::make_unique<Impl>())
{
}

StdInput::StdInput(StdInput&& stream) noexcept : _pimpl(std::move(stream._pimpl))
{
}

StdInput::~StdInput()
{
}

StdInput& StdInput::operator=(StdInput&& stream) noexcept
{
    _pimpl = std::move(stream._pimpl);
    return *this;
}

void* StdInput::stream() const noexcept
{
    return _pimpl->stream();
}

bool StdInput::IsValid() const noexcept
{
    return _pimpl->IsValid();
}

size_t StdInput::Read(void* buffer, size_t size)
{
    return _pimpl->Read(buffer, size);
}

StdOutput::StdOutput() : _pimpl(std::make_unique<Impl>())
{
}

StdOutput::StdOutput(StdOutput&& stream) noexcept : _pimpl(std::move(stream._pimpl))
{
}

StdOutput::~StdOutput()
{
}

StdOutput& StdOutput::operator=(StdOutput&& stream) noexcept
{
    _pimpl = std::move(stream._pimpl);
    return *this;
}

void* StdOutput::stream() const noexcept
{
    return _pimpl->stream();
}

bool StdOutput::IsValid() const noexcept
{
    return _pimpl->IsValid();
}

size_t StdOutput::Write(const void* buffer, size_t size)
{
    return _pimpl->Write(buffer, size);
}

void StdOutput::Flush()
{
    return _pimpl->Flush();
}

StdError::StdError() : _pimpl(std::make_unique<Impl>())
{
}

StdError::StdError(StdError&& stream) noexcept : _pimpl(std::move(stream._pimpl))
{
}

StdError::~StdError()
{
}

StdError& StdError::operator=(StdError&& stream) noexcept
{
    _pimpl = std::move(stream._pimpl);
    return *this;
}

void* StdError::stream() const noexcept
{
    return _pimpl->stream();
}

bool StdError::IsValid() const noexcept
{
    return _pimpl->IsValid();
}

size_t StdError::Write(const void* buffer, size_t size)
{
    return _pimpl->Write(buffer, size);
}

void StdError::Flush()
{
    return _pimpl->Flush();
}

} // namespace CppCommon
