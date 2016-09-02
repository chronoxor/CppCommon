/*!
    \file file_lock.cpp
    \brief File-lock synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 01.09.2016
    \copyright MIT License
*/

#include "threads/file_lock.h"

#include "errors/fatal.h"
#include "filesystem/exceptions.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace CppCommon {

class FileLock::Impl
{
public:
    Impl(const Path& path) : _path(path)
    {
#if defined(_WIN32) || defined(_WIN64)
        std::wstring wpath = _path.to_wstring();

        // Retries in CreateFile, see http://support.microsoft.com/kb/316609
        const int attempts = 5;
        const int sleep = 250;
        for (int attempt = 0; attempt < attempts; ++attempt)
        {
            _file = CreateFileW(wpath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, CREATE_NEW, FILE_ATTRIBUTE_HIDDEN, nullptr);
            if (_file == INVALID_HANDLE_VALUE)
            {
                if (GetLastError() == ERROR_SHARING_VIOLATION)
                {
                    Sleep(sleep);
                    continue;
                }
                else
                {
                    _file = CreateFileW(wpath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_HIDDEN, nullptr);
                    if (_file == INVALID_HANDLE_VALUE)
                    {
                        if (GetLastError() == ERROR_SHARING_VIOLATION)
                        {
                            Sleep(sleep);
                            continue;
                        }
                        else
                            break;
                    }
                    else
                    {
                        _owner = false;
                        return;
                    }
                }
            }
            else
            {
                _owner = true;
                return;
            }
        }
        throwex FileSystemException("Cannot create or open file-lock file!").Attach(_path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        _file = open(_path.native().c_str(), O_CREAT | O_EXCL | O_RDWR, 0644);
        if (_file < 0)
        {
            if (errno == EEXIST)
            {
                _file = open(_path.native().c_str(), O_CREAT | O_RDWR, 0644);
                if (_file >= 0)
                {
                    _owner = false;
                    return;
                }
            }
        }
        else
        {
            _owner = true;
            return;
        }
        throwex FileSystemException("Cannot create or open file-lock! file").Attach(_path);
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_file))
            fatality(FileSystemException("Cannot close the file-lock handle!").Attach(_path));

        // Remove the file-lock file (owner only)
        if (_owner)
        {
            if (!DeleteFileW(_path.to_wstring().c_str()))
                fatality(FileSystemException("Cannot delete the file-lock file!").Attach(_path));
        }
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = close(_file);
        if (result != 0)
            fatality(FileSystemException("Cannot close the file-lock descriptor!").Attach(_path));

        // Remove the file-lock file (owner only)
        if (_owner)
        {
            result = unlink(_path.native().c_str());
            if (result != 0)
                fatality(FileSystemException("Cannot unlink the file-lock file!").Attach(_path));
        }
#endif
    }

    const Path& path() const noexcept { return _path; }

    bool TryLockRead()
    {
#if defined(_WIN32) || defined(_WIN64)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        return LockFileEx(_file, LOCKFILE_FAIL_IMMEDIATELY, 0, MAXDWORD, MAXDWORD, &overlapped) ? true : false;
#elif defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
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
#elif defined(unix) || defined(__unix) || defined(__unix__)
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
#endif
    }

    bool TryLockWrite()
    {
#if defined(_WIN32) || defined(_WIN64)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        return LockFileEx(_file, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, MAXDWORD, MAXDWORD, &overlapped) ? true : false;
#elif defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
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
#elif defined(unix) || defined(__unix) || defined(__unix__)
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
#endif
    }

    void LockRead()
    {
#if defined(_WIN32) || defined(_WIN64)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        if (!LockFileEx(_file, 0, 0, MAXDWORD, MAXDWORD, &overlapped))
            throwex FileSystemException("Failed to lock for read!").Attach(_path);
#elif defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        int result = fcntl(_file, F_OFD_SETLKW, &lock);
        if (result == -1)
            throwex FileSystemException("Failed to lock for read!").Attach(_path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = flock(_file, LOCK_SH);
        if (result != 0)
            throwex FileSystemException("Failed to lock for read!").Attach(_path);
#endif
    }

    void LockWrite()
    {
#if defined(_WIN32) || defined(_WIN64)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        if (!LockFileEx(_file, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped))
            throwex FileSystemException("Failed to lock for write!").Attach(_path);
#elif defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        int result = fcntl(_file, F_OFD_SETLKW, &lock);
        if (result == -1)
            throwex FileSystemException("Failed to lock for write!").Attach(_path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = flock(_file, LOCK_EX);
        if (result != 0)
            throwex FileSystemException("Failed to lock for write!").Attach(_path);
#endif
    }

    void UnlockRead()
    {
#if defined(_WIN32) || defined(_WIN64)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        if (!UnlockFileEx(_file, 0, MAXDWORD, MAXDWORD, &overlapped))
            throwex FileSystemException("Failed to unlock the read lock!").Attach(_path);
#elif defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_UNLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        int result = fcntl(_file, F_OFD_SETLK, &lock);
        if (result != 0)
            throwex FileSystemException("Failed to unlock the read lock!").Attach(_path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = flock(_file, LOCK_UN);
        if (result != 0)
            throwex FileSystemException("Failed to unlock the read lock!").Attach(_path);
#endif
    }

    void UnlockWrite()
    {
#if defined(_WIN32) || defined(_WIN64)
        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(OVERLAPPED));
        if (!UnlockFileEx(_file, 0, MAXDWORD, MAXDWORD, &overlapped))
            throwex FileSystemException("Failed to unlock the write lock!").Attach(_path);
#elif defined(linux) || defined(__linux) || defined(__linux__)
        struct flock lock;
        lock.l_type = F_UNLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        int result = fcntl(_file, F_OFD_SETLK, &lock);
        if (result != 0)
            throwex FileSystemException("Failed to unlock the write lock!").Attach(_path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = flock(_file, LOCK_UN);
        if (result != 0)
            throwex FileSystemException("Failed to unlock the write lock!").Attach(_path);
#endif
    }

private:
    Path _path;
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _file;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int _file;
#endif
    bool _owner;
};

FileLock::FileLock(const Path& path) : _pimpl(new Impl(path))
{
}

FileLock::~FileLock()
{
}

const Path& FileLock::path() const noexcept
{
    return _pimpl->path();
}

bool FileLock::TryLockRead()
{
    return _pimpl->TryLockRead();
}

bool FileLock::TryLockWrite()
{
    return _pimpl->TryLockWrite();
}

void FileLock::LockRead()
{
    _pimpl->LockRead();
}

void FileLock::LockWrite()
{
    _pimpl->LockWrite();
}

void FileLock::UnlockRead()
{
    _pimpl->UnlockRead();
}

void FileLock::UnlockWrite()
{
    _pimpl->UnlockWrite();
}

} // namespace CppCommon
