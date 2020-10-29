/*!
    \file stream.cpp
    \brief Standard input/output/error stream implementation
    \author Ivan Shynkarenka
    \date 23.07.2017
    \copyright MIT License
*/

#include "system/stream.h"

#include "errors/fatal.h"
#include "utility/validate_aligned_storage.h"

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
        if (_stream == nullptr)
            throwex SystemException("Failed to get a valid standard input stream!");
#elif defined(_WIN32) || defined(_WIN64)
        _stream = GetStdHandle(STD_INPUT_HANDLE);
        if (_stream == INVALID_HANDLE_VALUE)
            throwex SystemException("Failed to get a valid standard input stream!");
#endif
    }

    ~Impl() = default;

    void* stream() const noexcept
    {
        return _stream;
    }

    bool IsValid() const noexcept
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return (_stream != nullptr);
#elif defined(_WIN32) || defined(_WIN64)
        return (_stream != INVALID_HANDLE_VALUE);
#endif
    }

    size_t Read(void* buffer, size_t size)
    {
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
        if (_stream == nullptr)
            throwex SystemException("Failed to get a valid standard output stream!");
#elif defined(_WIN32) || defined(_WIN64)
        _stream = GetStdHandle(STD_OUTPUT_HANDLE);
        if (_stream == INVALID_HANDLE_VALUE)
            throwex SystemException("Failed to get a valid standard output stream!");
#endif
    }

    ~Impl() = default;

    void* stream() const noexcept
    {
        return _stream;
    }

    bool IsValid() const noexcept
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return (_stream != nullptr);
#elif defined(_WIN32) || defined(_WIN64)
        return (_stream != INVALID_HANDLE_VALUE);
#endif
    }

    size_t Write(const void* buffer, size_t size)
    {
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
        if (_stream == nullptr)
            throwex SystemException("Failed to get a valid standard error stream!");
#elif defined(_WIN32) || defined(_WIN64)
        _stream = GetStdHandle(STD_ERROR_HANDLE);
        if (_stream == INVALID_HANDLE_VALUE)
            throwex SystemException("Failed to get a valid standard error stream!");
#endif
    }

    ~Impl() = default;

    void* stream() const noexcept
    {
        return _stream;
    }

    bool IsValid() const noexcept
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return (_stream != nullptr);
#elif defined(_WIN32) || defined(_WIN64)
        return (_stream != INVALID_HANDLE_VALUE);
#endif
    }

    size_t Write(const void* buffer, size_t size)
    {
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

StdInput::StdInput()
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "StdInput::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "StdInput::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl();
}

StdInput::~StdInput()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

void* StdInput::stream() const noexcept { return impl().stream(); }
bool StdInput::IsValid() const noexcept { return impl().IsValid(); }
size_t StdInput::Read(void* buffer, size_t size) { return impl().Read(buffer, size); }

void StdInput::swap(StdInput& stream) noexcept
{
    using std::swap;
    swap(_storage, stream._storage);
}

StdOutput::StdOutput()
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "StdOutput::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "StdOutput::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl();
}

StdOutput::~StdOutput()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

void* StdOutput::stream() const noexcept { return impl().stream(); }
bool StdOutput::IsValid() const noexcept { return impl().IsValid(); }
size_t StdOutput::Write(const void* buffer, size_t size) { return impl().Write(buffer, size); }
void StdOutput::Flush() { return impl().Flush(); }

void StdOutput::swap(StdOutput& stream) noexcept
{
    using std::swap;
    swap(_storage, stream._storage);
}

StdError::StdError()
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "StdError::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "StdError::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl();
}

StdError::~StdError()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

void* StdError::stream() const noexcept { return impl().stream(); }
bool StdError::IsValid() const noexcept { return impl().IsValid(); }
size_t StdError::Write(const void* buffer, size_t size) { return impl().Write(buffer, size); }
void StdError::Flush() { return impl().Flush(); }

void StdError::swap(StdError& stream) noexcept
{
    using std::swap;
    swap(_storage, stream._storage);
}

} // namespace CppCommon
