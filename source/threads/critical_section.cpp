/*!
    \file critical_section.cpp
    \brief Critical section synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#include "threads/critical_section.h"

#include "threads/thread.h"
#include "utility/validate_aligned_storage.h"

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include "errors/fatal.h"
#include <pthread.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef Yield
#endif

namespace CppCommon {

//! @cond INTERNALS

class CriticalSection::Impl
{
public:
    Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        pthread_mutexattr_t mutex_attribute;
        int result = pthread_mutexattr_init(&mutex_attribute);
        if (result != 0)
            throwex SystemException("Failed to initialize a mutex attribute!", result);
        result = pthread_mutexattr_settype(&mutex_attribute, PTHREAD_MUTEX_RECURSIVE);
        if (result != 0)
            throwex SystemException("Failed to set a mutex recursive attribute!", result);
        result = pthread_mutex_init(&_lock, &mutex_attribute);
        if (result != 0)
            throwex SystemException("Failed to initialize a mutex!", result);
        result = pthread_mutexattr_destroy(&mutex_attribute);
        if (result != 0)
            throwex SystemException("Failed to destroy a mutex attribute!", result);
#elif defined(_WIN32) || defined(_WIN64)
        InitializeCriticalSection(&_lock);
#endif
    }

    ~Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_destroy(&_lock);
        if (result != 0)
            fatality(SystemException("Failed to destroy a mutex!", result));
#elif defined(_WIN32) || defined(_WIN64)
        DeleteCriticalSection(&_lock);
#endif
    }

    void* native() noexcept
    {
        return &_lock;
    }

    bool TryLock()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_trylock(&_lock);
        if ((result != 0) && (result != EAGAIN) && (result != EBUSY) && (result != EDEADLK))
            throwex SystemException("Failed to try lock a mutex!", result);
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64)
        return (TryEnterCriticalSection(&_lock) != 0);
#endif
    }

    void Lock()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_lock(&_lock);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex!", result);
#elif defined(_WIN32) || defined(_WIN64)
        EnterCriticalSection(&_lock);
#endif
    }

    void Unlock()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_unlock(&_lock);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex!", result);
#elif defined(_WIN32) || defined(_WIN64)
        LeaveCriticalSection(&_lock);
#endif
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    pthread_mutex_t _lock;
#elif defined(_WIN32) || defined(_WIN64)
    CRITICAL_SECTION _lock;
#endif
};

//! @endcond

CriticalSection::CriticalSection()
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "CriticalSection::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "CriticalSection::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl();
}

CriticalSection::~CriticalSection()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

void* CriticalSection::native() noexcept { return impl().native(); }

bool CriticalSection::TryLock() { return impl().TryLock(); }

bool CriticalSection::TryLockFor(const Timespan& timespan)
{
    // Calculate a finish timestamp
    Timestamp finish = NanoTimestamp() + timespan;

    // Try to acquire critical section at least one time
    if (TryLock())
        return true;
    else
    {
        // Try lock or yield for the given timespan
        while (NanoTimestamp() < finish)
        {
            if (TryLock())
                return true;
            else
                Thread::Yield();
        }

        // Failed to acquire critical section
        return false;
    }
}

void CriticalSection::Lock() { impl().Lock(); }
void CriticalSection::Unlock() { impl().Unlock(); }

} // namespace CppCommon
