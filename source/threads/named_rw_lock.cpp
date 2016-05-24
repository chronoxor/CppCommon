/*!
    \file named_rw_lock.cpp
    \brief Named read/write lock synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 24.05.2016
    \copyright MIT License
*/

#include "threads/named_rw_lock.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"
#include "system/shared_type.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#endif

namespace CppCommon {

class NamedRWLock::Impl
{
public:
    Impl(const std::string& name)
    {
#if defined(_WIN32) || defined(_WIN64)
        InitializeSRWLock(&_rwlock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_init(&_rwlock, nullptr);
        if (result != 0)
            throwex SystemException("Failed to initialize a read/write lock!", result);
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        // SRW locks do not need to be explicitly destroyed.
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_destroy(&_rwlock);
        if (result != 0)
            fatality("Failed to destroy a read/write lock!", result);
#endif
    }

    bool TryLockRead()
    {
#if defined(_WIN32) || defined(_WIN64)
        return (TryAcquireSRWLockShared(&_rwlock) != 0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_tryrdlock(&_rwlock);
        if ((result != 0) && (result != EBUSY))
            throwex SystemException("Failed to try lock a read/write lock!", result);
        return (result == 0);
#endif
    }

    bool TryLockWrite()
    {
#if defined(_WIN32) || defined(_WIN64)
        return (TryAcquireSRWLockExclusive(&_rwlock) != 0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_trywrlock(&_rwlock);
        if ((result != 0) && (result != EBUSY))
            throwex SystemException("Failed to try lock a read/write lock!", result);
        return (result == 0);
#endif
    }

    void LockRead()
    {
#if defined(_WIN32) || defined(_WIN64)
        AcquireSRWLockShared(&_rwlock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_rdlock(&_rwlock);
        if (result != 0)
            throwex SystemException("Failed to lock a read/write lock!", result);
#endif
    }

    void LockWrite()
    {
#if defined(_WIN32) || defined(_WIN64)
        AcquireSRWLockExclusive(&_rwlock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_wrlock(&_rwlock);
        if (result != 0)
            throwex SystemException("Failed to lock a read/write lock!", result);
#endif
    }

    void UnlockRead()
    {
#if defined(_WIN32) || defined(_WIN64)
        ReleaseSRWLockShared(&_rwlock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_unlock(&_rwlock);
        if (result != 0)
            throwex SystemException("Failed to unlock a read/write lock!", result);
#endif
    }

    void UnlockWrite()
    {
#if defined(_WIN32) || defined(_WIN64)
        ReleaseSRWLockExclusive(&_rwlock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_rwlock_unlock(&_rwlock);
        if (result != 0)
            throwex SystemException("Failed to unlock a read/write lock!", result);
#endif
    }

private:
    SharedType<unsigned> _value;
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _exclusive_wake;
    HANDLE _shared_wake;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    sem_t* _exclusive_wake;
    sem_t* _shared_wake;
#endif
};

NamedRWLock::NamedRWLock(const std::string& name) : _pimpl(new Impl(name)), _name(name)
{
}

NamedRWLock::~NamedRWLock()
{
}

bool NamedRWLock::TryLockRead()
{
    return _pimpl->TryLockRead();
}

bool NamedRWLock::TryLockWrite()
{
    return _pimpl->TryLockWrite();
}

void NamedRWLock::LockRead()
{
    _pimpl->LockRead();
}

void NamedRWLock::LockWrite()
{
    _pimpl->LockWrite();
}

void NamedRWLock::UnlockRead()
{
    _pimpl->UnlockRead();
}

void NamedRWLock::UnlockWrite()
{
    _pimpl->UnlockWrite();
}

} // namespace CppCommon
