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
#include "threads/thread.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef Yield
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <fcntl.h>
#include <semaphore.h>
#endif

namespace CppCommon {

class NamedRWLock::Impl
{
    const uint32_t LOCK_OWNED = 0x1;
    const uint32_t LOCK_EXCLUSIVE_WAKING = 0x2;

    const uint32_t LOCK_SHARED_OWNERS_SHIFT = 2;
    const uint32_t LOCK_SHARED_OWNERS_MASK = 0x3ff;
    const uint32_t LOCK_SHARED_OWNERS_INC = 0x4;

    const uint32_t LOCK_SHARED_WAITERS_SHIFT = 12;
    const uint32_t LOCK_SHARED_WAITERS_MASK = 0x3ff;
    const uint32_t LOCK_SHARED_WAITERS_INC = 0x1000;

    const uint32_t LOCK_EXCLUSIVE_WAITERS_SHIFT = 22;
    const uint32_t LOCK_EXCLUSIVE_WAITERS_MASK = 0x3ff;
    const uint32_t LOCK_EXCLUSIVE_WAITERS_INC = 0x400000;

    const uint32_t LOCK_EXCLUSIVE_MASK = (LOCK_EXCLUSIVE_WAKING | (LOCK_EXCLUSIVE_WAITERS_MASK << LOCK_EXCLUSIVE_WAITERS_SHIFT));

public:
    Impl(const std::string& name) : _value(name)
    {
        // Owner of the read/write lock should initialize its value
        if (_value.owner())
            *_value = 0;
#if defined(_WIN32) || defined(_WIN64)
        _exclusive_wake = CreateSemaphoreA(nullptr, 0, MAXLONG, (this->name() + "_exclusive").c_str());
        if (_exclusive_wake == nullptr)
            throwex SystemException("Failed to create or open an exclusive wake semaphore for the named read/write lock!");
        _shared_wake = CreateSemaphoreA(nullptr, 0, MAXLONG, (this->name() + "_shared").c_str());
        if (_shared_wake == nullptr)
            throwex SystemException("Failed to create or open a shared wake semaphore for the named read/write lock!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        if (_value.owner())
        {
            _exclusive_wake = sem_open((this->name() + "_exclusive").c_str(), (O_CREAT | O_EXCL), 0666, 0);
            if (_exclusive_wake == SEM_FAILED)
                throwex SystemException("Failed to create an exclusive wake semaphore for the named read/write lock!");
            _shared_wake = sem_open((this->name() + "_shared").c_str(), (O_CREAT | O_EXCL), 0666, 0);
            if (_shared_wake == SEM_FAILED)
                throwex SystemException("Failed to create an shared wake semaphore for the named read/write lock!");
        }
        else
        {
            _exclusive_wake = sem_open((this->name() + "_exclusive").c_str(), O_CREAT, 0666, 0);
            if (_exclusive_wake == SEM_FAILED)
                throwex SystemException("Failed to open an exclusive wake semaphore for the named read/write lock!");
            _shared_wake = sem_open((this->name() + "_shared").c_str(), O_CREAT, 0666, 0);
            if (_shared_wake == SEM_FAILED)
                throwex SystemException("Failed to open an shared wake semaphore for the named read/write lock!");
        }
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_exclusive_wake))
            fatality("Failed to close an exclusive wake semaphore for the named read/write lock!");
        if (!CloseHandle(_shared_wake))
            fatality("Failed to close a shared wake semaphore for the named read/write lock!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_close(_exclusive_wake);
        if (result != 0)
            fatality("Failed to close an exclusive wake semaphore for the named read/write lock");
        result = sem_close(_shared_wake);
        if (result != 0)
            fatality("Failed to close an shared wake semaphore for the named read/write lock");
        // Unlink the named semaphores (owner only)
        if (_value.owner())
        {
            result = sem_unlink((name() + "_exclusive").c_str());
            if (result != 0)
                fatality("Failed to unlink an exclusive wake semaphore for the named read/write lock!");
            result = sem_unlink((name() + "_shared").c_str());
            if (result != 0)
                fatality("Failed to unlink an shared wake semaphore for the named read/write lock!");
        }
#endif
    }

    const std::string& name() const
    {
        return _value.name();
    }

    bool TryLockRead()
    {
        uint32_t value = *_value;

        if (value & LOCK_EXCLUSIVE_MASK)
            return false;

        if (!(value & LOCK_OWNED))
        {
#if defined(_WIN32) || defined(_WIN64)
            return InterlockedCompareExchange(_value.ptr(), value + LOCK_OWNED + LOCK_SHARED_OWNERS_INC, value) == value;
#elif defined(unix) || defined(__unix) || defined(__unix__)
            return __sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_OWNED + LOCK_SHARED_OWNERS_INC);
#endif
        }
        else if ((value >> LOCK_SHARED_OWNERS_SHIFT) & LOCK_SHARED_OWNERS_MASK)
        {
#if defined(_WIN32) || defined(_WIN64)
            return InterlockedCompareExchange(_value.ptr(), value + LOCK_SHARED_OWNERS_INC, value) == value;
#elif defined(unix) || defined(__unix) || defined(__unix__)
            return __sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_SHARED_OWNERS_INC);
#endif
        }
        else
            return false;
    }

    bool TryLockWrite()
    {
        uint32_t value = *_value;

        if (value & (LOCK_OWNED | LOCK_EXCLUSIVE_WAKING))
            return false;

#if defined(_WIN32) || defined(_WIN64)
        return InterlockedCompareExchange(_value.ptr(), value + LOCK_OWNED, value) == value;
#elif defined(unix) || defined(__unix) || defined(__unix__)
        return __sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_OWNED);
#endif
    }

    void LockRead()
    {

    }

    void LockWrite()
    {

    }

    void UnlockRead()
    {
        uint32_t value;

        while (true)
        {
            value = *_value;

            if (((value >> LOCK_SHARED_OWNERS_SHIFT) & LOCK_SHARED_OWNERS_MASK) > 1)
            {
#if defined(_WIN32) || defined(_WIN64)
                if (InterlockedCompareExchange(_value.ptr(), value - LOCK_SHARED_OWNERS_INC, value) == value)
#elif defined(unix) || defined(__unix) || defined(__unix__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value - LOCK_SHARED_OWNERS_INC))
#endif
                    break;
            }
            else if ((value >> LOCK_EXCLUSIVE_WAITERS_SHIFT) & LOCK_EXCLUSIVE_WAITERS_MASK)
            {
#if defined(_WIN32) || defined(_WIN64)
                if (InterlockedCompareExchange(_value.ptr(), value - LOCK_OWNED + LOCK_EXCLUSIVE_WAKING - LOCK_SHARED_OWNERS_INC - LOCK_EXCLUSIVE_WAITERS_INC, value) == value)
#elif defined(unix) || defined(__unix) || defined(__unix__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value - LOCK_OWNED + LOCK_EXCLUSIVE_WAKING - LOCK_SHARED_OWNERS_INC - LOCK_EXCLUSIVE_WAITERS_INC))
#endif
                {
#if defined(_WIN32) || defined(_WIN64)
                    if (!ReleaseSemaphore(_exclusive_wake, 1, nullptr))
                        throwex SystemException("Failed to release an exclusive wake semaphore for the named read/write lock!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
                    int result = sem_post(_exclusive_wake);
                    if (result != 0)
                        throwex SystemException("Failed to release an exclusive wake semaphore for the named read/write lock!");
#endif
                    break;
                }
            }
            else
            {
#if defined(_WIN32) || defined(_WIN64)
                if (InterlockedCompareExchange(_value.ptr(), value - LOCK_OWNED - LOCK_SHARED_OWNERS_INC, value) == value)
#elif defined(unix) || defined(__unix) || defined(__unix__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value - LOCK_OWNED - LOCK_SHARED_OWNERS_INC))
#endif
                    break;
            }

            Thread::Yield();
        }
    }

    void UnlockWrite()
    {
        uint32_t value;

        while (true)
        {
            value = *_value;

            if ((value >> LOCK_EXCLUSIVE_WAITERS_SHIFT) & LOCK_EXCLUSIVE_WAITERS_MASK)
            {
#if defined(_WIN32) || defined(_WIN64)
                if (InterlockedCompareExchange(_value.ptr(), value - LOCK_OWNED + LOCK_EXCLUSIVE_WAKING - LOCK_EXCLUSIVE_WAITERS_INC, value) == value)
#elif defined(unix) || defined(__unix) || defined(__unix__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value - LOCK_OWNED + LOCK_EXCLUSIVE_WAKING - LOCK_EXCLUSIVE_WAITERS_INC))
#endif
                {
#if defined(_WIN32) || defined(_WIN64)
                    if (!ReleaseSemaphore(_exclusive_wake, 1, nullptr))
                        throwex SystemException("Failed to release an exclusive wake semaphore for the named read/write lock!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
                    int result = sem_post(_exclusive_wake);
                    if (result != 0)
                        throwex SystemException("Failed to release an exclusive wake semaphore for the named read/write lock!");
#endif
                    break;
                }
            }
            else
            {
                uint32_t shared_waiters = (value >> LOCK_SHARED_WAITERS_SHIFT) & LOCK_SHARED_WAITERS_MASK;

#if defined(_WIN32) || defined(_WIN64)
                if (InterlockedCompareExchange(_value.ptr(), value & ~(LOCK_OWNED | (LOCK_SHARED_WAITERS_MASK << LOCK_SHARED_WAITERS_SHIFT)), value) == value)
#elif defined(unix) || defined(__unix) || defined(__unix__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value & ~(LOCK_OWNED | (LOCK_SHARED_WAITERS_MASK << LOCK_SHARED_WAITERS_SHIFT))))
#endif
                {
                    if (shared_waiters > 0)
                    {
#if defined(_WIN32) || defined(_WIN64)
                        if (!ReleaseSemaphore(_shared_wake, shared_waiters, nullptr))
                            throwex SystemException("Failed to release a shared wake semaphore for the named read/write lock!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
                        while (shared_waiters-- > 0)
                        {
                            int result = sem_post(_shared_wake);
                            if (result != 0)
                                throwex SystemException("Failed to release a shared wake semaphore for the named read/write lock!");
                        }
#endif
                    }
                    break;
                }
            }

            Thread::Yield();
        }
    }

private:
    SharedType<volatile uint32_t> _value;
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _exclusive_wake;
    HANDLE _shared_wake;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    sem_t* _exclusive_wake;
    sem_t* _shared_wake;
#endif
};

NamedRWLock::NamedRWLock(const std::string& name) : _pimpl(new Impl(name))
{
}

NamedRWLock::~NamedRWLock()
{
}

const std::string& NamedRWLock::name() const
{
    return _pimpl->name();
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
