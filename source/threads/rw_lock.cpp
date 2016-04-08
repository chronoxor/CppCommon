/*!
    \file rw_lock.cpp
    \brief Read/Write lock synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 08.04.2016
    \copyright MIT License
*/

#include "threads/rw_lock.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#endif

namespace CppCommon {

class RWLock::Impl
{
public:
    Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        InitializeSRWLock(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_init(&_lock, nullptr);
        if (result != 0)
            throwex SystemException(result, "Failed to initialize a read/write lock!");
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        // SRW locks do not need to be explicitly destroyed.
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_destroy(&_lock);
        if (result != 0)
            fatality("Failed to destroy a read/write lock!", result);
#endif
    }

    bool TryLockRead()
    {
#if defined(_WIN32) || defined(_WIN64)
        return (TryAcquireSRWLockShared(&_lock) != 0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_tryrdlock(&_lock);
        if ((result != 0) && (result != EBUSY))
            throwex SystemException(result, "Failed to try lock a read/write lock!");
        return (result == 0);
#endif
    }

    bool TryLockWrite()
    {
#if defined(_WIN32) || defined(_WIN64)
        return (TryAcquireSRWLockExclusive(&_lock) != 0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_trywrlock(&_lock);
        if ((result != 0) && (result != EBUSY))
            throwex SystemException(result, "Failed to try lock a read/write lock!");
        return (result == 0);
#endif
    }

    void LockRead()
    {
#if defined(_WIN32) || defined(_WIN64)
        AcquireSRWLockShared(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_rdlock(&_lock);
        if (result != 0)
            throwex SystemException(result, "Failed to lock a read/write lock!");
#endif
    }

    void LockWrite()
    {
#if defined(_WIN32) || defined(_WIN64)
        AcquireSRWLockExclusive(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_wrlock(&_lock);
        if (result != 0)
            throwex SystemException(result, "Failed to lock a read/write lock!");
#endif
    }

    void UnlockRead()
    {
#if defined(_WIN32) || defined(_WIN64)
        ReleaseSRWLockShared(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_unlock(&_lock);
        if (result != 0)
            throwex SystemException(result, "Failed to unlock a read/write lock!");
#endif
    }

    void UnlockWrite()
    {
#if defined(_WIN32) || defined(_WIN64)
        ReleaseSRWLockExclusive(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_unlock(&_lock);
        if (result != 0)
            throwex SystemException(result, "Failed to unlock a read/write lock!");
#endif
    }

private:
#if defined(_WIN32) || defined(_WIN64)
    SRWLOCK _lock;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    pthread_mutex_t _lock;
#endif
};

RWLock::RWLock() : _pimpl(new Impl())
{
}

RWLock::~RWLock()
{
}

bool RWLock::TryLockRead()
{
    return _pimpl->TryLockRead();
}

bool RWLock::TryLockWrite()
{
    return _pimpl->TryLockWrite();
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
