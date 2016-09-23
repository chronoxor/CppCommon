/*!
    \file rw_lock.cpp
    \brief Read/Write lock synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 08.04.2016
    \copyright MIT License
*/

#if (__MINGW32__)
#define _WIN32_WINNT 0x601
#endif

#include "threads/rw_lock.h"

#include "threads/thread.h"

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include "errors/exceptions.h"
#include "errors/fatal.h"
#include <pthread.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef Yield
#endif

namespace CppCommon {

class RWLock::Impl
{
public:
    Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_rwlock_init(&_rwlock, nullptr);
        if (result != 0)
            throwex SystemException("Failed to initialize a read/write lock!", result);
#elif defined(_WIN32) || defined(_WIN64)
        InitializeSRWLock(&_rwlock);
#endif
    }

    ~Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_rwlock_destroy(&_rwlock);
        if (result != 0)
            fatality(SystemException("Failed to destroy a read/write lock!", result));
#elif defined(_WIN32) || defined(_WIN64)
        // SRW locks do not need to be explicitly destroyed.
#endif
    }

    bool TryLockRead()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_rwlock_tryrdlock(&_rwlock);
        if ((result != 0) && (result != EBUSY))
            throwex SystemException("Failed to try lock for read!", result);
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64)
        return (TryAcquireSRWLockShared(&_rwlock) != 0);
#endif
    }

    bool TryLockWrite()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_rwlock_trywrlock(&_rwlock);
        if ((result != 0) && (result != EBUSY))
            throwex SystemException("Failed to try lock for write!", result);
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64)
        return (TryAcquireSRWLockExclusive(&_rwlock) != 0);
#endif
    }

    void LockRead()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_rwlock_rdlock(&_rwlock);
        if (result != 0)
            throwex SystemException("Failed to lock for read!", result);
#elif defined(_WIN32) || defined(_WIN64)
        AcquireSRWLockShared(&_rwlock);
#endif
    }

    void LockWrite()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_rwlock_wrlock(&_rwlock);
        if (result != 0)
            throwex SystemException("Failed to lock for write!", result);
#elif defined(_WIN32) || defined(_WIN64)
        AcquireSRWLockExclusive(&_rwlock);
#endif
    }

    void UnlockRead()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_rwlock_unlock(&_rwlock);
        if (result != 0)
            throwex SystemException("Failed to unlock read lock!", result);
#elif defined(_WIN32) || defined(_WIN64)
        ReleaseSRWLockShared(&_rwlock);
#endif
    }

    void UnlockWrite()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_rwlock_unlock(&_rwlock);
        if (result != 0)
            throwex SystemException("Failed to unlock write lock!", result);
#elif defined(_WIN32) || defined(_WIN64)
        ReleaseSRWLockExclusive(&_rwlock);
#endif
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    pthread_rwlock_t _rwlock;
#elif defined(_WIN32) || defined(_WIN64)
    SRWLOCK _rwlock;
#endif
};

RWLock::RWLock() : _pimpl(std::make_unique<Impl>())
{
}

RWLock::RWLock(RWLock&& lock) noexcept : _pimpl(std::move(lock._pimpl))
{
}

RWLock::~RWLock()
{
}

RWLock& RWLock::operator=(RWLock&& lock) noexcept
{
    _pimpl = std::move(lock._pimpl);
    return *this;
}

bool RWLock::TryLockRead()
{
    return _pimpl->TryLockRead();
}

bool RWLock::TryLockWrite()
{
    return _pimpl->TryLockWrite();
}

bool RWLock::TryLockReadFor(const Timespan& timespan)
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

bool RWLock::TryLockWriteFor(const Timespan& timespan)
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

void RWLock::LockRead()
{
    _pimpl->LockRead();
}

void RWLock::LockWrite()
{
    _pimpl->LockWrite();
}

void RWLock::UnlockRead()
{
    _pimpl->UnlockRead();
}

void RWLock::UnlockWrite()
{
    _pimpl->UnlockWrite();
}

} // namespace CppCommon
