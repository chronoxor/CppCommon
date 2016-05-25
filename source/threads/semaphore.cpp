/*!
    \file semaphore.cpp
    \brief Semaphore synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 12.04.2016
    \copyright MIT License
*/

#include "threads/semaphore.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"

#include <algorithm>
#include <cassert>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef max
#undef min
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <fcntl.h>
#include <semaphore.h>
#endif

namespace CppCommon {

class Semaphore::Impl
{
public:
    Impl(int resources)
    {
        assert((resources > 0) && "Semaphore resources counter must be greater than zero!");

#if defined(_WIN32) || defined(_WIN64)
        _semaphore = CreateSemaphore(nullptr, resources, resources, nullptr);
        if (_semaphore == nullptr)
            throwex SystemException("Failed to create a semaphore!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_init(&_semaphore, 0, resources);
        if (result != 0)
            throwex SystemException("Failed to initialize a semaphore!");
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_semaphore))
            fatality("Failed to close a semaphore!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_destroy(&_semaphore);
        if (result != 0)
            fatality("Failed to destroy a semaphore!");
#endif
    }

    bool TryLock()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_semaphore, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a semaphore!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_trywait(&_semaphore);
        if ((result != 0) && (errno != EAGAIN))
            throwex SystemException("Failed to try lock a semaphore!");
        return (result == 0);
#endif
    }

    bool TryLockFor(int64_t nanoseconds)
    {
        if (nanoseconds < 0)
            return TryLock();
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_semaphore, (DWORD)std::max(1ll, nanoseconds / 1000000000));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a semaphore for the given timeout!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        struct timespec timeout;
        timeout.tv_sec = nanoseconds / 1000000000;
        timeout.tv_nsec = nanoseconds % 1000000000;
        int result = sem_timedwait(&_semaphore, &timeout);
        if ((result != 0) && (errno != ETIMEDOUT))
            throwex SystemException("Failed to try lock a semaphore for the given timeout!");
        return (result == 0);
#endif
    }

    void Lock()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_semaphore, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a semaphore!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_wait(&_semaphore);
        if (result != 0)
            throwex SystemException("Failed to lock a semaphore!");
#endif
    }

    void Unlock()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!ReleaseSemaphore(_semaphore, 1, nullptr))
            throwex SystemException("Failed to unlock a semaphore!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_post(&_semaphore);
        if (result != 0)
            throwex SystemException("Failed to unlock a semaphore!");
#endif
    }

private:
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _semaphore;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    sem_t _semaphore;
#endif
};

Semaphore::Semaphore(int resources) : _pimpl(new Impl(resources)), _resources(resources)
{
}

Semaphore::~Semaphore()
{
}

bool Semaphore::TryLock()
{
    return _pimpl->TryLock();
}

bool Semaphore::TryLockFor(int64_t nanoseconds)
{
    return _pimpl->TryLockFor(nanoseconds);
}

void Semaphore::Lock()
{
    _pimpl->Lock();
}

void Semaphore::Unlock()
{
    _pimpl->Unlock();
}

} // namespace CppCommon
