/*!
    \file mutex.cpp
    \brief Mutex synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 04.04.2016
    \copyright MIT License
*/

#include "threads/mutex.h"

#include "errors/fatal.h"
#include "utility/validate_aligned_storage.h"

#include <algorithm>

#if defined(__APPLE__) || defined(__CYGWIN__)
#include "threads/thread.h"
#endif
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <pthread.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef max
#undef min
#endif

namespace CppCommon {

//! @cond INTERNALS

class Mutex::Impl
{
public:
    Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_init(&_mutex, nullptr);
        if (result != 0)
            throwex SystemException("Failed to initialize a mutex!", result);
#elif defined(_WIN32) || defined(_WIN64)
        _mutex = CreateMutex(nullptr, FALSE, nullptr);
        if (_mutex == nullptr)
            throwex SystemException("Failed to create a mutex!");
#endif
    }

    ~Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_destroy(&_mutex);
        if (result != 0)
            fatality(SystemException("Failed to destroy a mutex!", result));
#elif defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_mutex))
            fatality(SystemException("Failed to close a mutex!"));
#endif
    }

    bool TryLock()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_trylock(&_mutex);
        if ((result != 0) && (result != EAGAIN) && (result != EBUSY) && (result != EDEADLK))
            throwex SystemException("Failed to try lock a mutex!", result);
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_mutex, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a mutex!");
        return (result == WAIT_OBJECT_0);
#endif
    }

    bool TryLockFor(const Timespan& timespan)
    {
        if (timespan < 0)
            return TryLock();
#if defined(__APPLE__) || defined(__CYGWIN__)
        // Calculate a finish timestamp
        Timestamp finish = NanoTimestamp() + timespan;

        // Try to acquire lock at least one time
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

            // Failed to acquire lock
            return false;
        }
#elif defined(unix) || defined(__unix) || defined(__unix__)
        struct timespec timeout;
        timeout.tv_sec = timespan.seconds();
        timeout.tv_nsec = timespan.nanoseconds() % 1000000000;
        int result = pthread_mutex_timedlock(&_mutex, &timeout);
        if ((result != 0) && (result != ETIMEDOUT))
            throwex SystemException("Failed to try lock a mutex for the given timeout!", result);
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_mutex, std::max((DWORD)1, (DWORD)timespan.milliseconds()));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a mutex for the given timeout!");
        return (result == WAIT_OBJECT_0);
#endif
    }

    void Lock()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex!", result);
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_mutex, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a mutex!");
#endif
    }

    void Unlock()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex!", result);
#elif defined(_WIN32) || defined(_WIN64)
        if (!ReleaseMutex(_mutex))
            throwex SystemException("Failed to unlock a mutex!");
#endif
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    pthread_mutex_t _mutex;
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE _mutex;
#endif
};

//! @endcond

Mutex::Mutex()
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "Mutex::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "Mutex::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl();
}

Mutex::~Mutex()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

bool Mutex::TryLock() { return impl().TryLock(); }
bool Mutex::TryLockFor(const Timespan& timespan) { return impl().TryLockFor(timespan); }

void Mutex::Lock() { impl().Lock(); }
void Mutex::Unlock() { impl().Unlock(); }

} // namespace CppCommon
