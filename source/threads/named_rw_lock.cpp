/*!
    \file named_rw_lock.cpp
    \brief Named read/write lock synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 24.05.2016
    \copyright MIT License
*/

#include "threads/named_rw_lock.h"

#include "errors/fatal.h"
#include "system/shared_type.h"
#include "threads/thread.h"
#include "utility/validate_aligned_storage.h"

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
#include <fcntl.h>
#include <semaphore.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#undef Yield
#endif

namespace CppCommon {

//! @cond INTERNALS

//! Named read/write lock synchronization primitive implementation
/*!
    Resource lock value width: 32 bits.
    Lock owned (either exclusive or shared): L (1 bit).
    Exclusive waking: W (1 bit).
    Shared owners count: SC (10 bits).
    Shared waiters count: SW (10 bits).
    Exclusive waiters count: EW (10 bits).

    Acquire exclusive:
    {L=0,W=0,SC=0,SW,EW=0} -> {L=1,W=0,SC=0,SW,EW=0}
    {L=0,W=1,SC=0,SW,EW} or {L=1,W,SC,SW,EW} ->
        {L,W,SC,SW,EW+1},
        wait on event,
        {L=0,W=1,SC=0,SW,EW} -> {L=1,W=0,SC=0,SW,EW}

    Acquire shared:
    {L=0,W=0,SC=0,SW,EW=0} -> {L=1,W=0,SC=1,SW,EW=0}
    {L=1,W=0,SC>0,SW,EW=0} -> {L=1,W=0,SC+1,SW,EW=0}
    {L=1,W=0,SC=0,SW,EW=0} or {L,W=1,SC,SW,EW} or
        {L,W,SC,SW,EW>0} -> {L,W,SC,SW+1,EW},
        wait on event,
        retry.

    Release exclusive:
    {L=1,W=0,SC=0,SW,EW>0} ->
        {L=0,W=1,SC=0,SW,EW-1},
        release one exclusive waiter.
    {L=1,W=0,SC=0,SW,EW=0} ->
        {L=0,W=0,SC=0,SW=0,EW=0},
        release all shared waiters.

    Note that we never do a direct acquire when W=1
    (i.e. L=0 if W=1), so here we don't have to check
    the value of W.

    Release shared:
    {L=1,W=0,SC>1,SW,EW} -> {L=1,W=0,SC-1,SW,EW}
    {L=1,W=0,SC=1,SW,EW=0} -> {L=0,W=0,SC=0,SW,EW=0}
    {L=1,W=0,SC=1,SW,EW>0} ->
        {L=0,W=1,SC=0,SW,EW-1},
        release one exclusive waiter.

    Again, we don't need to check the value of W.

    Convert exclusive to shared:
    {L=1,W=0,SC=0,SW,EW} ->
        {L=1,W=0,SC=1,SW=0,EW},
        release all shared waiters.

    Convert shared to exclusive:
    {L=1,W=0,SC=1,SW,EW} ->
        {L=1,W=0,SC=0,SW,EW}
*/
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
    Impl(const std::string& name, uint32_t spin) : _value(name), _spin(spin)
    {
        // Owner of the read/write lock should initialize its value
        if (_value.owner())
            *_value = 0;
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
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
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        _exclusive_wake = CreateSemaphoreA(nullptr, 0, MAXLONG, (this->name() + "_exclusive").c_str());
        if (_exclusive_wake == nullptr)
            throwex SystemException("Failed to create or open an exclusive wake semaphore for the named read/write lock!");
        _shared_wake = CreateSemaphoreA(nullptr, 0, MAXLONG, (this->name() + "_shared").c_str());
        if (_shared_wake == nullptr)
            throwex SystemException("Failed to create or open a shared wake semaphore for the named read/write lock!");
#endif
    }

    ~Impl()
    {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = sem_close(_exclusive_wake);
        if (result != 0)
            fatality(SystemException("Failed to close an exclusive wake semaphore for the named read/write lock"));
        result = sem_close(_shared_wake);
        if (result != 0)
            fatality(SystemException("Failed to close an shared wake semaphore for the named read/write lock"));
        // Unlink the named semaphores (owner only)
        if (_value.owner())
        {
            result = sem_unlink((name() + "_exclusive").c_str());
            if (result != 0)
                fatality(SystemException("Failed to unlink an exclusive wake semaphore for the named read/write lock!"));
            result = sem_unlink((name() + "_shared").c_str());
            if (result != 0)
                fatality(SystemException("Failed to unlink an shared wake semaphore for the named read/write lock!"));
        }
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        if (!CloseHandle(_exclusive_wake))
            fatality(SystemException("Failed to close an exclusive wake semaphore for the named read/write lock!"));
        if (!CloseHandle(_shared_wake))
            fatality(SystemException("Failed to close a shared wake semaphore for the named read/write lock!"));
#endif
    }

    const std::string& name() const
    {
        return _value.name();
    }

    bool TryLockRead()
    {
        uint32_t value = (uint32_t)*_value;

        if (value & LOCK_EXCLUSIVE_MASK)
            return false;

        if (!(value & LOCK_OWNED))
        {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
            return __sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_OWNED + LOCK_SHARED_OWNERS_INC);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
            return (InterlockedCompareExchange(_value.ptr(), value + LOCK_OWNED + LOCK_SHARED_OWNERS_INC, value) == value);
#endif
        }
        else if ((value >> LOCK_SHARED_OWNERS_SHIFT) & LOCK_SHARED_OWNERS_MASK)
        {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
            return __sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_SHARED_OWNERS_INC);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
            return (InterlockedCompareExchange(_value.ptr(), value + LOCK_SHARED_OWNERS_INC, value) == value);
#endif
        }
        else
            return false;
    }

    bool TryLockWrite()
    {
        uint32_t value = (uint32_t)*_value;

        if (value & (LOCK_OWNED | LOCK_EXCLUSIVE_WAKING))
            return false;

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        return __sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_OWNED);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        return (InterlockedCompareExchange(_value.ptr(), value + LOCK_OWNED, value) == value);
#endif
    }

    bool TryConvertWriteToRead()
    {
        while (true)
        {
            uint32_t value = (uint32_t)*_value;

            // Can't convert if there are other shared owners
            if (((value >> LOCK_SHARED_OWNERS_SHIFT) & LOCK_SHARED_OWNERS_MASK) != 1)
                return false;

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
            if (__sync_bool_compare_and_swap(_value.ptr(), value, value - LOCK_SHARED_OWNERS_INC))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
            if (InterlockedCompareExchange(_value.ptr(), value - LOCK_SHARED_OWNERS_INC, value) == value)
#endif
                return true;
        }
    }

    void LockRead()
    {
        uint32_t iteration = 0;

        while (true)
        {
            uint32_t value = (uint32_t)*_value;

            // Case 1: lock not owned AND no exclusive waiter is waking up AND
            // there are no shared owners AND there are no exclusive waiters
            if (!(value & (LOCK_OWNED | (LOCK_SHARED_OWNERS_MASK << LOCK_SHARED_OWNERS_SHIFT) | LOCK_EXCLUSIVE_MASK)))
            {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_OWNED + LOCK_SHARED_OWNERS_INC))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                if (InterlockedCompareExchange(_value.ptr(), value + LOCK_OWNED + LOCK_SHARED_OWNERS_INC, value) == value)
#endif
                    break;
            }
            // Case 2: lock is owned AND no exclusive waiter is waking up AND
            // there are shared owners AND there are no exclusive waiters
            else if ((value & LOCK_OWNED) && ((value >> LOCK_SHARED_OWNERS_SHIFT) & LOCK_SHARED_OWNERS_MASK) > 0 && !(value & LOCK_EXCLUSIVE_MASK))
            {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_SHARED_OWNERS_INC))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                if (InterlockedCompareExchange(_value.ptr(), value + LOCK_SHARED_OWNERS_INC, value) == value)
#endif
                    break;
            }
            // Other cases
            else if (iteration++ >= _spin)
            {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_SHARED_WAITERS_INC))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                if (InterlockedCompareExchange(_value.ptr(), value + LOCK_SHARED_WAITERS_INC, value) == value)
#endif
                {
                    // Go to sleep
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                    int result = sem_wait(_shared_wake);
                    if (result != 0)
                        throwex SystemException("Failed to wait for a shared wake semaphore for the named read/write lock!");
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                    DWORD result = WaitForSingleObject(_shared_wake, INFINITE);
                    if (result != WAIT_OBJECT_0)
                        throwex SystemException("Failed to wait for a shared wake semaphore for the named read/write lock!");
#endif
                    // Go back and try again
                    continue;
                }
            }

            // Yield to other threads
            Thread::Yield();
        }
    }

    void LockWrite()
    {
        uint32_t iteration = 0;

        while (true)
        {
            uint32_t value = (uint32_t)*_value;

            // Case 1: lock not owned AND an exclusive waiter is not waking up.
            // Here we don't have to check if there are exclusive waiters, because
            // if there are the lock would be owned, and we are checking that anyway.
            if (!(value & (LOCK_OWNED | LOCK_EXCLUSIVE_WAKING)))
            {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_OWNED))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                if (InterlockedCompareExchange(_value.ptr(), value + LOCK_OWNED, value) == value)
#endif
                    break;
            }
            // Case 2: lock owned OR lock not owned and an exclusive waiter is waking up.
            // The second case means an exclusive waiter has just been woken up and is
            // going to acquire the lock. We have to go to sleep to make sure we don't
            // steal the lock.
            else if (iteration++ >= _spin)
            {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_EXCLUSIVE_WAITERS_INC))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                if (InterlockedCompareExchange(_value.ptr(), value + LOCK_EXCLUSIVE_WAITERS_INC, value) == value)
#endif
                {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                    int result = sem_wait(_exclusive_wake);
                    if (result != 0)
                        throwex SystemException("Failed to wait for an exclusive wake semaphore for the named read/write lock!");
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                    DWORD result = WaitForSingleObject(_exclusive_wake, INFINITE);
                    if (result != WAIT_OBJECT_0)
                        throwex SystemException("Failed to wait for an exclusive wake semaphore for the named read/write lock!");
#endif
                    // Acquire the lock.
                    // At this point *no one* should be able to steal the lock from us.
                    do
                    {
                        value = (uint32_t)*_value;
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                    } while (!__sync_bool_compare_and_swap(_value.ptr(), value, value + LOCK_OWNED - LOCK_EXCLUSIVE_WAKING));
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                    } while (InterlockedCompareExchange(_value.ptr(), value + LOCK_OWNED - LOCK_EXCLUSIVE_WAKING, value) != value);
#endif
                    break;
                }
            }

            // Yield to other threads
            Thread::Yield();
        }
    }

    void UnlockRead()
    {
        while (true)
        {
            uint32_t value = (uint32_t)*_value;

            // Case 1: there are multiple shared owners
            if (((value >> LOCK_SHARED_OWNERS_SHIFT) & LOCK_SHARED_OWNERS_MASK) > 1)
            {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value - LOCK_SHARED_OWNERS_INC))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                if (InterlockedCompareExchange(_value.ptr(), value - LOCK_SHARED_OWNERS_INC, value) == value)
#endif
                    break;
            }
            // Case 2: we are the last shared owner AND there are exclusive waiters
            else if ((value >> LOCK_EXCLUSIVE_WAITERS_SHIFT) & LOCK_EXCLUSIVE_WAITERS_MASK)
            {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value - LOCK_OWNED + LOCK_EXCLUSIVE_WAKING - LOCK_SHARED_OWNERS_INC - LOCK_EXCLUSIVE_WAITERS_INC))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                if (InterlockedCompareExchange(_value.ptr(), value - LOCK_OWNED + LOCK_EXCLUSIVE_WAKING - LOCK_SHARED_OWNERS_INC - LOCK_EXCLUSIVE_WAITERS_INC, value) == value)
#endif
                {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                    int result = sem_post(_exclusive_wake);
                    if (result != 0)
                        throwex SystemException("Failed to release an exclusive wake semaphore for the named read/write lock!");
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                    if (!ReleaseSemaphore(_exclusive_wake, 1, nullptr))
                        throwex SystemException("Failed to release an exclusive wake semaphore for the named read/write lock!");
#endif
                    break;
                }
            }
            // Case 3: we are the last shared owner AND there are no exclusive waiters
            else
            {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value - LOCK_OWNED - LOCK_SHARED_OWNERS_INC))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                if (InterlockedCompareExchange(_value.ptr(), value - LOCK_OWNED - LOCK_SHARED_OWNERS_INC, value) == value)
#endif
                    break;
            }

            // Yield to other threads
            Thread::Yield();
        }
    }

    void UnlockWrite()
    {
        while (true)
        {
            uint32_t value = (uint32_t)*_value;

            // Case 1: if we have exclusive waiters, release one
            if ((value >> LOCK_EXCLUSIVE_WAITERS_SHIFT) & LOCK_EXCLUSIVE_WAITERS_MASK)
            {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value - LOCK_OWNED + LOCK_EXCLUSIVE_WAKING - LOCK_EXCLUSIVE_WAITERS_INC))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                if (InterlockedCompareExchange(_value.ptr(), value - LOCK_OWNED + LOCK_EXCLUSIVE_WAKING - LOCK_EXCLUSIVE_WAITERS_INC, value) == value)
#endif
                {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                    int result = sem_post(_exclusive_wake);
                    if (result != 0)
                        throwex SystemException("Failed to release an exclusive wake semaphore for the named read/write lock!");
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                    if (!ReleaseSemaphore(_exclusive_wake, 1, nullptr))
                        throwex SystemException("Failed to release an exclusive wake semaphore for the named read/write lock!");
#endif
                    break;
                }
            }
            // Case 2: if we have shared waiters, release all of them
            else
            {
                uint32_t shared_waiters = (value >> LOCK_SHARED_WAITERS_SHIFT) & LOCK_SHARED_WAITERS_MASK;

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                if (__sync_bool_compare_and_swap(_value.ptr(), value, value & ~(LOCK_OWNED | (LOCK_SHARED_WAITERS_MASK << LOCK_SHARED_WAITERS_SHIFT))))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                if (InterlockedCompareExchange(_value.ptr(), value & ~(LOCK_OWNED | (LOCK_SHARED_WAITERS_MASK << LOCK_SHARED_WAITERS_SHIFT)), value) == value)
#endif
                {
                    if (shared_waiters > 0)
                    {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                        while (shared_waiters-- > 0)
                        {
                            int result = sem_post(_shared_wake);
                            if (result != 0)
                                throwex SystemException("Failed to release a shared wake semaphore for the named read/write lock!");
                        }
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                        if (!ReleaseSemaphore(_shared_wake, shared_waiters, nullptr))
                            throwex SystemException("Failed to release a shared wake semaphore for the named read/write lock!");
#endif
                    }
                    break;
                }
            }

            // Yield to other threads
            Thread::Yield();
        }
    }

    void ConvertWriteToRead()
    {
        while (true)
        {
            uint32_t value = (uint32_t)*_value;

            uint32_t shared_waiters = (value >> LOCK_SHARED_WAITERS_SHIFT) & LOCK_SHARED_WAITERS_MASK;

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
            if (__sync_bool_compare_and_swap(_value.ptr(), value, (value + LOCK_SHARED_OWNERS_INC) & ~(LOCK_SHARED_WAITERS_MASK << LOCK_SHARED_WAITERS_SHIFT)))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
            if (InterlockedCompareExchange(_value.ptr(), (value + LOCK_SHARED_OWNERS_INC) & ~(LOCK_SHARED_WAITERS_MASK << LOCK_SHARED_WAITERS_SHIFT), value) == value)
#endif
            {
                if (shared_waiters > 0)
                {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                    while (shared_waiters-- > 0)
                    {
                        int result = sem_post(_shared_wake);
                        if (result != 0)
                            throwex SystemException("Failed to release a shared wake semaphore for the named read/write lock!");
                    }
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                    if (!ReleaseSemaphore(_shared_wake, shared_waiters, nullptr))
                        throwex SystemException("Failed to release a shared wake semaphore for the named read/write lock!");
#endif
                }

                break;
            }
        }
    }

private:
    SharedType<volatile uint64_t> _value;
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
    sem_t* _exclusive_wake;
    sem_t* _shared_wake;
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    HANDLE _exclusive_wake;
    HANDLE _shared_wake;
#endif
    uint32_t _spin;
};

//! @endcond

NamedRWLock::NamedRWLock(const std::string& name)
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "NamedRWLock::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "NamedRWLock::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(name, 4000);
}

NamedRWLock::~NamedRWLock()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

const std::string& NamedRWLock::name() const { return impl().name(); }

bool NamedRWLock::TryLockRead() { return impl().TryLockRead(); }
bool NamedRWLock::TryLockWrite() { return impl().TryLockWrite(); }
bool NamedRWLock::TryConvertWriteToRead() { return impl().TryConvertWriteToRead(); }

bool NamedRWLock::TryLockReadFor(const Timespan& timespan)
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

bool NamedRWLock::TryLockWriteFor(const Timespan& timespan)
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

bool NamedRWLock::TryConvertWriteToReadFor(const Timespan& timespan)
{
    // Calculate a finish timestamp
    Timestamp finish = NanoTimestamp() + timespan;

    // Try to convert write lock to read lock at least one time
    if (TryConvertWriteToRead())
        return true;
    else
    {
        // Try convert write lock to read lock or yield for the given timespan
        while (NanoTimestamp() < finish)
        {
            if (TryConvertWriteToRead())
                return true;
            else
                Thread::Yield();
        }

        // Failed to convert write lock to read lock
        return false;
    }
}

void NamedRWLock::LockRead() { impl().LockRead(); }
void NamedRWLock::LockWrite() { impl().LockWrite(); }
void NamedRWLock::UnlockRead() { impl().UnlockRead(); }
void NamedRWLock::UnlockWrite() { impl().UnlockWrite(); }
void NamedRWLock::ConvertWriteToRead() { impl().ConvertWriteToRead(); }

} // namespace CppCommon
