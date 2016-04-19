/*!
    \file named_mutex.cpp
    \brief Named mutex synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 15.04.2016
    \copyright MIT License
*/

#include "threads/named_mutex.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"

#include <algorithm>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef max
#undef min
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#endif

namespace CppCommon {

class NamedMutex::Impl
{
public:
    Impl(const std::string& name)
    {
#if defined(_WIN32) || defined(_WIN64)
        _mutex = CreateMutexA(nullptr, FALSE, name.c_str());
        if (_mutex == nullptr)
            throwex SystemException("Failed to create a named mutex!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        _semaphore = sem_open(name.c_str(), O_CREAT, 0666, 1);
        if (_semaphore == SEM_FAILED)
            throwex SystemException(result, "Failed to initialize a named binary semaphore!");
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_mutex))
            fatality("Failed to close a named mutex!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_close(&_semaphore);
        if (result != 0)
            fatality("Failed to close a named binary semaphore!", result);
#endif
    }

    bool TryLock()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_mutex, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named mutex!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_trywait(&_semaphore);
        if ((result != 0) && (result != EAGAIN))
            throwex SystemException(result, "Failed to try lock a named binary semaphore!");
        return (result == 0);
#endif
    }

    bool TryLockFor(int64_t nanoseconds)
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_mutex, (DWORD)std::max(1ll, nanoseconds / 1000000000));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named mutex for the given timeout!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        struct timespec timeout;
        timeout.tv_sec = nanoseconds / 1000000000;
        timeout.tv_nsec = nanoseconds % 1000000000;
        int result = sem_timedwait(&_semaphore, &timeout);
        if ((result != 0) && (result != ETIMEDOUT))
            throwex SystemException(result, "Failed to try lock a named binary semaphore for the given timeout!");
        return (result == 0);
#endif
    }

    void Lock()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_mutex, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a named mutex!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_wait(&_semaphore);
        if (result != 0)
            throwex SystemException(result, "Failed to lock a named binary semaphore!");
#endif
    }

    void Unlock()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!ReleaseMutex(_mutex))
            throwex SystemException("Failed to unlock a named mutex!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_post(&_semaphore);
        if (result != 0)
            throwex SystemException(result, "Failed to unlock a named binary semaphore!");
#endif
    }

private:
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _mutex;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    sem_t _semaphore;
#endif
};

NamedMutex::NamedMutex(const std::string& name) : _pimpl(new Impl(name)), _name(name)
{
}

NamedMutex::~NamedMutex()
{
}

bool NamedMutex::TryLock()
{
    return _pimpl->TryLock();
}

bool NamedMutex::TryLockFor(int64_t nanoseconds)
{
    return _pimpl->TryLockFor(nanoseconds);
}

void NamedMutex::Lock()
{
    _pimpl->Lock();
}

void NamedMutex::Unlock()
{
    _pimpl->Unlock();
}

} // namespace CppCommon
