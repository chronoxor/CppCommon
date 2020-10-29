/*!
    \file semaphore.cpp
    \brief Semaphore synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 12.04.2016
    \copyright MIT License
*/

#include "threads/semaphore.h"

#include "errors/fatal.h"
#include "utility/validate_aligned_storage.h"

#include <algorithm>
#include <cassert>

#if defined(__APPLE__)
#include <dispatch/dispatch.h>
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <fcntl.h>
#include <semaphore.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef max
#undef min
#endif

namespace CppCommon {

//! @cond INTERNALS

class Semaphore::Impl
{
public:
    explicit Impl(int resources) : _resources(resources)
    {
        assert((resources > 0) && "Semaphore resources counter must be greater than zero!");

#if defined(__APPLE__)
        _semaphore = dispatch_semaphore_create(resources);
        if (_semaphore == nullptr)
            throwex SystemException("Failed to initialize a semaphore!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_init(&_semaphore, 0, resources);
        if (result != 0)
            throwex SystemException("Failed to initialize a semaphore!");
#elif defined(_WIN32) || defined(_WIN64)
        _semaphore = CreateSemaphore(nullptr, resources, resources, nullptr);
        if (_semaphore == nullptr)
            throwex SystemException("Failed to create a semaphore!");
#endif
    }

    ~Impl()
    {
#if defined(__APPLE__)
        if (_semaphore != nullptr)
            dispatch_release(_semaphore);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_destroy(&_semaphore);
        if (result != 0)
            fatality(SystemException("Failed to destroy a semaphore!"));
#elif defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_semaphore))
            fatality(SystemException("Failed to close a semaphore!"));
#endif
    }

    int resources() const noexcept
    {
        return _resources;
    }

    bool TryLock()
    {
#if defined(__APPLE__)
        return (dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_NOW) == 0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_trywait(&_semaphore);
        if ((result != 0) && (errno != EAGAIN))
            throwex SystemException("Failed to try lock a semaphore!");
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_semaphore, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a semaphore!");
        return (result == WAIT_OBJECT_0);
#endif
    }

    bool TryLockFor(const Timespan& timespan)
    {
        if (timespan < 0)
            return TryLock();
#if defined(__APPLE__)
        dispatch_time_t timeout = dispatch_time(DISPATCH_TIME_NOW, timespan.nanoseconds());
        return (dispatch_semaphore_wait(_semaphore, timeout) == 0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        struct timespec timeout;
        timeout.tv_sec = timespan.seconds();
        timeout.tv_nsec = timespan.nanoseconds() % 1000000000;
        int result = sem_timedwait(&_semaphore, &timeout);
        if ((result != 0) && (errno != ETIMEDOUT))
            throwex SystemException("Failed to try lock a semaphore for the given timeout!");
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_semaphore, std::max((DWORD)1, (DWORD)timespan.milliseconds()));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a semaphore for the given timeout!");
        return (result == WAIT_OBJECT_0);
#endif
    }

    void Lock()
    {
#if defined(__APPLE__)
        dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_wait(&_semaphore);
        if (result != 0)
            throwex SystemException("Failed to lock a semaphore!");
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_semaphore, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a semaphore!");
#endif
    }

    void Unlock()
    {
#if defined(__APPLE__)
        dispatch_semaphore_signal(_semaphore);
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = sem_post(&_semaphore);
        if (result != 0)
            throwex SystemException("Failed to unlock a semaphore!");
#elif defined(_WIN32) || defined(_WIN64)
        if (!ReleaseSemaphore(_semaphore, 1, nullptr))
            throwex SystemException("Failed to unlock a semaphore!");
#endif
    }

private:
    int _resources;
#if defined(__APPLE__)
    dispatch_semaphore_t _semaphore;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    sem_t _semaphore;
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE _semaphore;
#endif
};

//! @endcond

Semaphore::Semaphore(int resources)
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "Semaphore::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "Semaphore::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(resources);
}

Semaphore::~Semaphore()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

int Semaphore::resources() const noexcept { return impl().resources(); }

bool Semaphore::TryLock() { return impl().TryLock(); }
bool Semaphore::TryLockFor(const Timespan& timespan) { return impl().TryLockFor(timespan); }

void Semaphore::Lock() { impl().Lock(); }
void Semaphore::Unlock() { impl().Unlock(); }

} // namespace CppCommon
