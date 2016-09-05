/*!
    \file named_mutex.cpp
    \brief Named mutex synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 15.04.2016
    \copyright MIT License
*/

#include "threads/named_mutex.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"

#include <algorithm>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef max
#undef min
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include "system/shared_type.h"
#include <pthread.h>
#endif

namespace CppCommon {

class NamedMutex::Impl
{
public:
    Impl(const std::string& name) : _name(name)
#if defined(unix) || defined(__unix) || defined(__unix__)
        , _shared(name)
#endif
    {
#if defined(_WIN32) || defined(_WIN64)
        _mutex = CreateMutexA(nullptr, FALSE, name.c_str());
        if (_mutex == nullptr)
            throwex SystemException("Failed to create or open a named mutex!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
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
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_mutex))
            fatality(SystemException("Failed to close a named mutex!"));
#elif defined(unix) || defined(__unix) || defined(__unix__)
        // Only the owner should destroy a named mutex
        if (_shared.owner())
        {
            int result = pthread_mutex_destroy(&_shared->mutex);
            if (result != 0)
                fatality(SystemException("Failed to destroy a named mutex!", result));
        }
#endif
    }

    const std::string& name() const
    {
        return _name;
    }

    bool TryLock()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_mutex, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named mutex!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_trylock(&_shared->mutex);
        if ((result != 0) && (result != EBUSY))
            throwex SystemException("Failed to try lock a named mutex!", result);
        return (result == 0);
#endif
    }

    bool TryLockFor(const Timespan& timespan)
    {
        if (timespan < 0)
            return TryLock();
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_mutex, (DWORD)std::max(1ll, timespan.milliseconds()));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named mutex for the given timeout!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        struct timespec timeout;
        timeout.tv_sec = timespan.seconds();
        timeout.tv_nsec = timespan.nanoseconds() % 1000000000;
        int result = pthread_mutex_timedlock(&_shared->mutex, &timeout);
        if ((result != 0) && (result != ETIMEDOUT))
            throwex SystemException("Failed to try lock a named mutex for the given timeout!", result);
        return (result == 0);
#endif
    }

    void Lock()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_mutex, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a named mutex!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_lock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a named mutex!", result);
#endif
    }

    void Unlock()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!ReleaseMutex(_mutex))
            throwex SystemException("Failed to unlock a named mutex!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_unlock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a named mutex!", result);
#endif
    }

private:
    std::string _name;
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _mutex;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    // Shared mutex structure
    struct MutexHeader
    {
        pthread_mutex_t mutex;
    };

    // Shared mutex structure wrapper
    SharedType<MutexHeader> _shared;
#endif
};

NamedMutex::NamedMutex(const std::string& name) : _pimpl(std::make_unique<Impl>(name))
{
}

NamedMutex::~NamedMutex()
{
}

const std::string& NamedMutex::name() const
{
    return _pimpl->name();
}

bool NamedMutex::TryLock()
{
    return _pimpl->TryLock();
}

bool NamedMutex::TryLockFor(const Timespan& timespan)
{
    return _pimpl->TryLockFor(timespan);
}

void NamedMutex::Lock()
{
    _pimpl->Lock();
}

void NamedMutex::Unlock()
{
    _pimpl->Unlock();
}

} // namespace CppCommon
