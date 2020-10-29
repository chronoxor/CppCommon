/*!
    \file named_mutex.cpp
    \brief Named mutex synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 15.04.2016
    \copyright MIT License
*/

#include "threads/named_mutex.h"

#include "errors/fatal.h"
#include "utility/validate_aligned_storage.h"

#include <algorithm>

#if defined(__APPLE__)
#include "threads/thread.h"
#endif
#if (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__APPLE__) && !defined(__CYGWIN__)
#include "system/shared_type.h"
#include <pthread.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#undef max
#undef min
#endif

namespace CppCommon {

//! @cond INTERNALS

class NamedMutex::Impl
{
public:
    Impl(const std::string& name) : _name(name)
#if (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__APPLE__) && !defined(__CYGWIN__)
        , _shared(name)
#endif
    {
#if defined(__APPLE__)
        throwex SystemException("Named mutex is not supported!");
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        // Only the owner should initializate a named mutex
        if (_shared.owner())
        {
            pthread_mutexattr_t mutex_attribute;
            int result = pthread_mutexattr_init(&mutex_attribute);
            if (result != 0)
                throwex SystemException("Failed to initialize a named mutex attribute!", result);
            result = pthread_mutexattr_setpshared(&mutex_attribute, PTHREAD_PROCESS_SHARED);
            if (result != 0)
                throwex SystemException("Failed to set a named mutex process shared attribute!", result);
            result = pthread_mutex_init(&_shared->mutex, &mutex_attribute);
            if (result != 0)
                throwex SystemException("Failed to initialize a named mutex!", result);
            result = pthread_mutexattr_destroy(&mutex_attribute);
            if (result != 0)
                throwex SystemException("Failed to destroy a named mutex attribute!", result);
        }
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        _mutex = CreateMutexA(nullptr, FALSE, name.c_str());
        if (_mutex == nullptr)
            throwex SystemException("Failed to create or open a named mutex!");
#endif
    }

    ~Impl()
    {
#if defined(__APPLE__)
        fatality(SystemException("Named mutex is not supported!"));
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        // Only the owner should destroy a named mutex
        if (_shared.owner())
        {
            int result = pthread_mutex_destroy(&_shared->mutex);
            if (result != 0)
                fatality(SystemException("Failed to destroy a named mutex!", result));
        }
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        if (!CloseHandle(_mutex))
            fatality(SystemException("Failed to close a named mutex!"));
#endif
    }

    const std::string& name() const
    {
        return _name;
    }

    bool TryLock()
    {
#if defined(__APPLE__)
        throwex SystemException("Named mutex is not supported!");
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        int result = pthread_mutex_trylock(&_shared->mutex);
        if ((result != 0) && (result != EAGAIN) && (result != EBUSY) && (result != EDEADLK))
            throwex SystemException("Failed to try lock a named mutex!", result);
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        DWORD result = WaitForSingleObject(_mutex, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named mutex!");
        return (result == WAIT_OBJECT_0);
#endif
    }

    bool TryLockFor(const Timespan& timespan)
    {
        if (timespan < 0)
            return TryLock();
#if defined(__APPLE__)
        throwex SystemException("Named mutex is not supported!");
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        struct timespec timeout;
        timeout.tv_sec = timespan.seconds();
        timeout.tv_nsec = timespan.nanoseconds() % 1000000000;
        int result = pthread_mutex_timedlock(&_shared->mutex, &timeout);
        if ((result != 0) && (result != ETIMEDOUT))
            throwex SystemException("Failed to try lock a named mutex for the given timeout!", result);
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        DWORD result = WaitForSingleObject(_mutex, std::max((DWORD)1, (DWORD)timespan.milliseconds()));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named mutex for the given timeout!");
        return (result == WAIT_OBJECT_0);
#endif
    }

    void Lock()
    {
#if defined(__APPLE__)
        throwex SystemException("Named mutex is not supported!");
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        int result = pthread_mutex_lock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a named mutex!", result);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        DWORD result = WaitForSingleObject(_mutex, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a named mutex!");
#endif
    }

    void Unlock()
    {
#if defined(__APPLE__)
        throwex SystemException("Named mutex is not supported!");
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        int result = pthread_mutex_unlock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a named mutex!", result);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        if (!ReleaseMutex(_mutex))
            throwex SystemException("Failed to unlock a named mutex!");
#endif
    }

private:
    std::string _name;
#if (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__APPLE__) && !defined(__CYGWIN__)
    // Shared mutex structure
    struct MutexHeader
    {
        pthread_mutex_t mutex;
    };

    // Shared mutex structure wrapper
    SharedType<MutexHeader> _shared;
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    HANDLE _mutex;
#endif
};

//! @endcond

NamedMutex::NamedMutex(const std::string& name)
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "NamedMutex::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "NamedMutex::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(name);
}

NamedMutex::~NamedMutex()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

const std::string& NamedMutex::name() const { return impl().name(); }

bool NamedMutex::TryLock() { return impl().TryLock(); }
bool NamedMutex::TryLockFor(const Timespan& timespan) { return impl().TryLockFor(timespan); }

void NamedMutex::Lock() { impl().Lock(); }
void NamedMutex::Unlock() { impl().Unlock(); }

} // namespace CppCommon
