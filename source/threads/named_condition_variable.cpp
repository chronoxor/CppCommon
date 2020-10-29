/*!
    \file named_condition_variable.cpp
    \brief Named condition variable synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 04.10.2016
    \copyright MIT License
*/

#include "threads/named_condition_variable.h"

#include "errors/fatal.h"
#include "utility/validate_aligned_storage.h"

#include <algorithm>

#if (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__APPLE__) && !defined(__CYGWIN__)
#include "system/shared_type.h"
#include <pthread.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include "system/shared_type.h"
#include <windows.h>
#undef max
#undef min
#endif

namespace CppCommon {

//! @cond INTERNALS

class NamedConditionVariable::Impl
{
public:
    Impl(const std::string& name) : _name(name)
#if (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__APPLE__) && !defined(__CYGWIN__)
        , _shared(name)
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        , _shared(name)
#endif
    {
#if defined(__APPLE__)
        throwex SystemException("Named condition variable is not supported!");
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        // Only the owner should initializate a named condition variable
        if (_shared.owner())
        {
            pthread_mutexattr_t mutex_attribute;
            int result = pthread_mutexattr_init(&mutex_attribute);
            if (result != 0)
                throwex SystemException("Failed to initialize a mutex attribute for the named condition variable!", result);
            result = pthread_mutexattr_setpshared(&mutex_attribute, PTHREAD_PROCESS_SHARED);
            if (result != 0)
                throwex SystemException("Failed to set a mutex process shared attribute for the named condition variable!", result);
            result = pthread_mutex_init(&_shared->mutex, &mutex_attribute);
            if (result != 0)
                throwex SystemException("Failed to initialize a mutex for the named condition variable!", result);
            result = pthread_mutexattr_destroy(&mutex_attribute);
            if (result != 0)
                throwex SystemException("Failed to destroy a mutex attribute for the named condition variable!", result);

            pthread_condattr_t cond_attribute;
            result = pthread_condattr_init(&cond_attribute);
            if (result != 0)
                throwex SystemException("Failed to initialize a conditional variable attribute for the named condition variable!", result);
            result = pthread_condattr_setpshared(&cond_attribute, PTHREAD_PROCESS_SHARED);
            if (result != 0)
                throwex SystemException("Failed to set a conditional variable process shared attribute for the named condition variable!", result);
            result = pthread_cond_init(&_shared->cond, &cond_attribute);
            if (result != 0)
                throwex SystemException("Failed to initialize a conditional variable for the named condition variable!", result);
            result = pthread_condattr_destroy(&cond_attribute);
            if (result != 0)
                throwex SystemException("Failed to destroy a conditional variable attribute for the named condition variable!", result);
        }
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        // Owner of the condition variable should initialize its value
        if (_shared.owner())
            *_shared = 0;
        _mutex = CreateMutexA(nullptr, FALSE, (name + "-mutex").c_str());
        if (_mutex == nullptr)
            throwex SystemException("Failed to create or open a named mutex for the named condition variable!");
        _semaphore = CreateSemaphoreA(nullptr, 0, std::numeric_limits<LONG>::max(), (name + "-semaphore").c_str());
        if (_semaphore == nullptr)
            throwex SystemException("Failed to create or open a named semaphore for the named condition variable!");
#endif
    }

    ~Impl()
    {
#if defined(__APPLE__)
        fatality(SystemException("Named condition variable is not supported!"));
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        // Only the owner should destroy a named condition variable
        if (_shared.owner())
        {
            int result = pthread_mutex_destroy(&_shared->mutex);
            if (result != 0)
                fatality(SystemException("Failed to destroy a mutex for the named condition variable!", result));
            result = pthread_cond_destroy(&_shared->cond);
            if (result != 0)
                fatality(SystemException("Failed to destroy a conditional variable for the named condition variable!", result));
        }
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        if (!CloseHandle(_mutex))
            fatality(SystemException("Failed to close a named mutex for the named condition variable!"));
        if (!CloseHandle(_semaphore))
            fatality(SystemException("Failed to close a named semaphore for the named condition variable!"));
#endif
    }

    const std::string& name() const
    {
        return _name;
    }

    void NotifyOne()
    {
#if defined(__APPLE__)
        throwex SystemException("Named condition variable is not supported!");
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        int result = pthread_cond_signal(&_shared->cond);
        if (result != 0)
            throwex SystemException("Failed to signal a named condition variable!", result);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        // Lock the named mutex
        DWORD result = WaitForSingleObject(_mutex, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a named mutex for the named condition variable!");
        // Decrement shared waiters count
        --(*_shared);
        // Signal one waiter
        if (!ReleaseSemaphore(_semaphore, 1, nullptr))
            throwex SystemException("Failed to release one semaphore waiter for the named condition variable!");
        // Unlock the named mutex
        if (!ReleaseMutex(_mutex))
            throwex SystemException("Failed to unlock a named mutex for the named condition variable!");
#endif
    }

    void NotifyAll()
    {
#if defined(__APPLE__)
        throwex SystemException("Named condition variable is not supported!");
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        int result = pthread_cond_broadcast(&_shared->cond);
        if (result != 0)
            throwex SystemException("Failed to broadcast a named condition variable!", result);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        // Lock the named mutex
        DWORD result = WaitForSingleObject(_mutex, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a named mutex for the named condition variable!");
        // Signal all waiters
        if (!ReleaseSemaphore(_semaphore, *_shared, nullptr))
            throwex SystemException("Failed to release all semaphore waiters for the named condition variable!");
        // Clear all shared waiters
        *_shared = 0;
        // Unlock the named mutex
        if (!ReleaseMutex(_mutex))
            throwex SystemException("Failed to unlock a named mutex for the named condition variable!");
#endif
    }

    void Wait()
    {
#if defined(__APPLE__)
        throwex SystemException("Named condition variable is not supported!");
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        int result = pthread_cond_wait(&_shared->cond, &_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to waiting a named condition variable!", result);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        // Lock the named mutex
        DWORD result = WaitForSingleObject(_mutex, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a named mutex for the named condition variable!");
        // Increment shared waiters count
        ++(*_shared);
        // Unlock the named mutex
        if (!ReleaseMutex(_mutex))
            throwex SystemException("Failed to unlock a named mutex for the named condition variable!");

        // Wait for the named condition variable
        result = WaitForSingleObject(_semaphore, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to wait a named condition variable!");
#endif
    }

    bool TryWaitFor(const Timespan& timespan)
    {
        if (timespan < 0)
            return false;
#if defined(__APPLE__)
        throwex SystemException("Named condition variable is not supported!");
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        struct timespec timeout;
        timeout.tv_sec = timespan.seconds();
        timeout.tv_nsec = timespan.nanoseconds() % 1000000000;
        int result = pthread_cond_timedwait(&_shared->cond, &_shared->mutex, &timeout);
        if ((result != 0) && (result != ETIMEDOUT))
            throwex SystemException("Failed to waiting a named condition variable for the given timeout!", result);
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        // Lock the named mutex
        DWORD result = WaitForSingleObject(_mutex, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a named mutex for the named condition variable!");
        // Increment shared waiters count
        ++(*_shared);
        // Unlock the named mutex
        if (!ReleaseMutex(_mutex))
            throwex SystemException("Failed to unlock a named mutex for the named condition variable!");

        // Wait for the named condition variable
        result = WaitForSingleObject(_semaphore, std::max((DWORD)0, (DWORD)timespan.milliseconds()));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named condition variable for the given timeout!");
        return (result == WAIT_OBJECT_0);
#endif
    }

private:
    std::string _name;
#if (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__APPLE__) && !defined(__CYGWIN__)
    // Shared condition variable structure
    struct CondVar
    {
        pthread_mutex_t mutex;
        pthread_cond_t cond;
    };

    // Shared condition variable structure wrapper
    SharedType<CondVar> _shared;
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    HANDLE _mutex;
    HANDLE _semaphore;
    SharedType<LONG> _shared;
#endif
};

//! @endcond

NamedConditionVariable::NamedConditionVariable(const std::string& name)
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "NamedConditionVariable::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "NamedConditionVariable::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(name);
}

NamedConditionVariable::~NamedConditionVariable()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

const std::string& NamedConditionVariable::name() const { return impl().name(); }

void NamedConditionVariable::NotifyOne() { impl().NotifyOne(); }
void NamedConditionVariable::NotifyAll() { impl().NotifyAll(); }

void NamedConditionVariable::Wait() { impl().Wait(); }

bool NamedConditionVariable::TryWaitFor(const Timespan& timespan) { return impl().TryWaitFor(timespan); }

} // namespace CppCommon
