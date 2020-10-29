/*!
    \file file_lock.cpp
    \brief File-lock synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 01.09.2016
    \copyright MIT License
*/

#include "threads/file_lock.h"

#include "errors/fatal.h"
#include "threads/thread.h"
#include "utility/validate_aligned_storage.h"

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#undef Yield
#endif

namespace CppCommon {

//! @cond INTERNALS

// In case we are on a system with glibc version earlier than 2.20
#ifndef F_OFD_GETLK
#define F_OFD_GETLK  36
#define F_OFD_SETLK  37
#define F_OFD_SETLKW 38
#endif

class FileLock::Impl
{
public:
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
    Impl() : _file(0) {}
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    Impl() : _file(nullptr) {}
#endif

    ~Impl()
    {
        try
        {
            Reset();
        }
        catch (const SystemException& ex)
        {
            fatality(SystemException(ex.string()));
        }
    }

    const Path& path() const noexcept { return _path; }

    void Assign(const Path& path)
    {
        // Reset the previous file-lock
        Reset();

        _path = path;
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        _file = open(_path.string().c_str(), O_CREAT | O_RDWR, 0644);
        if (_file < 0)
            throwex FileSystemException("Cannot create or open file-lock file!").Attach(path);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        // Retries in CreateFile, see http://support.microsoft.com/kb/316609
        const std::wstring wpath = _path.wstring();
        const int attempts = 1000;
        const int sleep = 100;
        for (int attempt = 0; attempt < attempts; ++attempt)
        {
            _file = CreateFileW(wpath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_FLAG_DELETE_ON_CLOSE, nullptr);
            if (_file == INVALID_HANDLE_VALUE)
            {
                Sleep(sleep);
                continue;
            }
            return;
        }
        throwex FileSystemException("Cannot create or open file-lock file!").Attach(path);
#endif
    }

    void Reset()
    {
        if (!_file)
            return;

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = close(_file);
        if (result != 0)
            fatality(FileSystemException("Cannot close the file-lock descriptor!").Attach(_path));
        _file = 0;

        // Remove the file-lock file
        unlink(_path.string().c_str());
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        if (!CloseHandle(_file))
            fatality(FileSystemException("Cannot close the file-lock handle!").Attach(_path));
        _file = nullptr;
#endif
    }

    bool TryLockRead()
    {
#if defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        lock.l_pid = 0;
        int result = fcntl(_file, F_OFD_SETLK, &lock);
        if (result == -1)
        {
           if  (errno == EAGAIN)
               return false;
           else
               throwex FileSystemException("Failed to try lock for read!").Attach(_path);
        }
        else
            return true;
#elif (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = flock(_file, LOCK_SH | LOCK_NB);
        if (result != 0)
        {
           if  (errno == EWOULDBLOCK)
               return false;
           else
               throwex FileSystemException("Failed to try lock for read!").Attach(_path);
        }
        else
            return true;
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        return LockFileEx(_file, LOCKFILE_FAIL_IMMEDIATELY, 0, MAXDWORD, MAXDWORD, &overlapped) != 0;
#endif
    }

    bool TryLockWrite()
    {
#if defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        lock.l_pid = 0;
        int result = fcntl(_file, F_OFD_SETLK, &lock);
        if (result == -1)
        {
           if  (errno == EAGAIN)
               return false;
           else
               throwex FileSystemException("Failed to try lock for write!").Attach(_path);
        }
        else
            return true;
#elif (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = flock(_file, LOCK_EX | LOCK_NB);
        if (result != 0)
        {
           if  (errno == EWOULDBLOCK)
               return false;
           else
               throwex FileSystemException("Failed to try lock for write!").Attach(_path);
        }
        else
            return true;
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        return LockFileEx(_file, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, MAXDWORD, MAXDWORD, &overlapped) != 0;
#endif
    }

    void LockRead()
    {
#if defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        lock.l_pid = 0;
        int result = fcntl(_file, F_OFD_SETLKW, &lock);
        if (result == -1)
            throwex FileSystemException("Failed to lock for read!").Attach(_path);
#elif (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = flock(_file, LOCK_SH);
        if (result != 0)
            throwex FileSystemException("Failed to lock for read!").Attach(_path);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        if (!LockFileEx(_file, 0, 0, MAXDWORD, MAXDWORD, &overlapped))
            throwex FileSystemException("Failed to lock for read!").Attach(_path);
#endif
    }

    void LockWrite()
    {
#if defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        lock.l_pid = 0;
        int result = fcntl(_file, F_OFD_SETLKW, &lock);
        if (result == -1)
            throwex FileSystemException("Failed to lock for write!").Attach(_path);
#elif (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = flock(_file, LOCK_EX);
        if (result != 0)
            throwex FileSystemException("Failed to lock for write!").Attach(_path);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        if (!LockFileEx(_file, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped))
            throwex FileSystemException("Failed to lock for write!").Attach(_path);
#endif
    }

    void UnlockRead()
    {
#if defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_UNLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        lock.l_pid = 0;
        int result = fcntl(_file, F_OFD_SETLK, &lock);
        if (result != 0)
            throwex FileSystemException("Failed to unlock the read lock!").Attach(_path);
#elif (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = flock(_file, LOCK_UN);
        if (result != 0)
            throwex FileSystemException("Failed to unlock the read lock!").Attach(_path);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        if (!UnlockFileEx(_file, 0, MAXDWORD, MAXDWORD, &overlapped))
            throwex FileSystemException("Failed to unlock the read lock!").Attach(_path);
#endif
    }

    void UnlockWrite()
    {
#if defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_UNLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        lock.l_pid = 0;
        int result = fcntl(_file, F_OFD_SETLK, &lock);
        if (result != 0)
            throwex FileSystemException("Failed to unlock the write lock!").Attach(_path);
#elif (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = flock(_file, LOCK_UN);
        if (result != 0)
            throwex FileSystemException("Failed to unlock the write lock!").Attach(_path);
#elif defined(_WIN32) || defined(_WIN64)  || defined(__CYGWIN__)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        if (!UnlockFileEx(_file, 0, MAXDWORD, MAXDWORD, &overlapped))
            throwex FileSystemException("Failed to unlock the write lock!").Attach(_path);
#endif
    }

private:
    Path _path;
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
    int _file;
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    HANDLE _file;
#endif
};

//! @endcond

FileLock::FileLock()
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "FileLock::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "FileLock::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl();
}

FileLock::FileLock(const Path& path) : FileLock()
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "FileLock::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "FileLock::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl();

    Assign(path);
}

FileLock::~FileLock()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

FileLock& FileLock::operator=(const Path& path)
{
    Assign(path);
    return *this;
}

const Path& FileLock::path() const noexcept { return impl().path(); }

void FileLock::Assign(const Path& path) { impl().Assign(path); }
void FileLock::Reset() { impl().Reset(); }

bool FileLock::TryLockRead() { return impl().TryLockRead(); }
bool FileLock::TryLockWrite() { return impl().TryLockWrite(); }

bool FileLock::TryLockReadFor(const Timespan& timespan)
{
    // Calculate a finish timestamp
    Timestamp finish = NanoTimestamp() + timespan;

    // Try to acquire read lock at least one time
    if (TryLockRead())
        return true;
    else
    {
        // Try lock or yield for the given timespan
        while (NanoTimestamp() < finish)
        {
            if (TryLockRead())
                return true;
            else
                Thread::Yield();
        }

        // Failed to acquire read lock
        return false;
    }
}

bool FileLock::TryLockWriteFor(const Timespan& timespan)
{
    // Calculate a finish timestamp
    Timestamp finish = NanoTimestamp() + timespan;

    // Try to acquire write lock at least one time
    if (TryLockWrite())
        return true;
    else
    {
        // Try lock or yield for the given timespan
        while (NanoTimestamp() < finish)
        {
            if (TryLockWrite())
                return true;
            else
                Thread::Yield();
        }

        // Failed to acquire write lock
        return false;
    }
}

void FileLock::LockRead() { impl().LockRead(); }
void FileLock::LockWrite() { impl().LockWrite(); }
void FileLock::UnlockRead() { impl().UnlockRead(); }
void FileLock::UnlockWrite() { impl().UnlockWrite(); }

} // namespace CppCommon
