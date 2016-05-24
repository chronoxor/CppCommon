/*!
    \file named_event_auto_reset.cpp
    \brief Named auto-reset event synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 24.05.2016
    \copyright MIT License
*/

#include "threads/named_event_auto_reset.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"
#include "system/shared_type.h"

#include <algorithm>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef max
#undef min
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#endif

namespace CppCommon {

class NamedEventAutoReset::Impl
{
public:
    Impl(const std::string& name, bool signaled) : _name(name)
#if defined(unix) || defined(__unix) || defined(__unix__)
        , _shared(name)
#endif
    {
#if defined(_WIN32) || defined(_WIN64)
        _event = CreateEventA(nullptr, FALSE, signaled ? TRUE : FALSE, name.c_str());
        if (_event == nullptr)
            throwex SystemException("Failed to create or open a named auto-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        // Only the owner should initializate a named auto-reset event
        if (_shared.owner())
        {
            pthread_mutexattr_t mutex_attribute;
            int result = pthread_mutexattr_init(&mutex_attribute);
            if (result != 0)
                throwex SystemException("Failed to initialize a mutex attribute for the named auto-reset event!", result);
            result = pthread_mutexattr_setpshared(&mutex_attribute, PTHREAD_PROCESS_SHARED);
            if (result != 0)
                throwex SystemException("Failed to set a mutex process shared attribute for the named auto-reset event!", result);
            result = pthread_mutex_init(&_shared->mutex, &mutex_attribute);
            if (result != 0)
                throwex SystemException("Failed to initialize a mutex for the named auto-reset event!", result);
            result = pthread_mutexattr_destroy(&mutex_attribute);
            if (result != 0)
                throwex SystemException("Failed to destroy a mutex attribute for the named auto-reset event!", result);

            pthread_condattr_t cond_attribute;
            result = pthread_condattr_init(&cond_attribute);
            if (result != 0)
                throwex SystemException("Failed to initialize a conditional variable attribute for the named auto-reset event!", result);
            result = pthread_condattr_setpshared(&cond_attribute, PTHREAD_PROCESS_SHARED);
            if (result != 0)
                throwex SystemException("Failed to set a conditional variable process shared attribute for the named auto-reset event!", result);
            result = pthread_cond_init(&_shared->cond, &cond_attribute);
            if (result != 0)
                throwex SystemException("Failed to initialize a conditional variable for the named auto-reset event!", result);
            result = pthread_condattr_destroy(&cond_attribute);
            if (result != 0)
                throwex SystemException("Failed to destroy a conditional variable attribute for the named auto-reset event!", result);

            _shared->signaled = signaled ? 1 : 0;
        }
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_event))
            fatality("Failed to close a named auto-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        // Only the owner should destroy a named auto-reset event
        if (_shared.owner())
        {
            int result = pthread_mutex_destroy(&_shared->mutex);
            if (result != 0)
                fatality("Failed to destroy a mutex for the named auto-reset event!", result);
            result = pthread_cond_destroy(&_shared->cond);
            if (result != 0)
                fatality("Failed to destroy a conditional variable for the named auto-reset event!", result);
        }
#endif
    }

    const std::string& name() const
    {
        return _name;
    }

    void Signal()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!SetEvent(_event))
            throwex SystemException("Failed to signal a named auto-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_lock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the named auto-reset event!", result);
        ++_shared->signaled;
        result = pthread_mutex_unlock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex for the named auto-reset event!", result);
        result = pthread_cond_signal(&_shared->cond);
        if (result != 0)
            throwex SystemException("Failed to signal a named auto-reset event!", result);
#endif
    }

    bool TryWait()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_event, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named auto-reset event!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_lock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the named auto-reset event!", result);
        bool signaled = (_shared->signaled-- > 0);
        _shared->signaled = (_shared->signaled < 0) ? 0 : _shared->signaled;
        result = pthread_mutex_unlock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex for the named auto-reset event!", result);
        return signaled;
#endif
    }

    bool TryWaitFor(int64_t nanoseconds)
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_event, (DWORD)std::max(1ll, nanoseconds / 1000000000));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named auto-reset event for the given timeout!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        struct timespec timeout;
        timeout.tv_sec = nanoseconds / 1000000000;
        timeout.tv_nsec = nanoseconds % 1000000000;
        int result = pthread_mutex_lock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the named auto-reset event!", result);
        bool signaled = true;
        while (!_shared->signaled)
        {
            result = pthread_cond_timedwait(&_shared->cond, &_shared->mutex, &timeout);
            if ((result != 0) && (result != ETIMEDOUT))
                throwex SystemException("Failed to timeout waiting a conditional variable for the named auto-reset event!", result);
            if (result == ETIMEDOUT)
                signaled = (_shared->signaled > 0);
        }
        _shared->signaled = (_shared->signaled > 0) ? (_shared->signaled - 1) : 0;
        result = pthread_mutex_unlock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex for the named auto-reset event!", result);
        return signaled;
#endif
    }

    void Wait()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_event, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a named auto-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_lock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the named auto-reset event!", result);
        while (!_shared->signaled)
        {
            result = pthread_cond_wait(&_shared->cond, &_shared->mutex);
            if ((result != 0) && (result != ETIMEDOUT))
                throwex SystemException("Failed to waiting a conditional variable for the named auto-reset event!", result);
        }
        _shared->signaled = (_shared->signaled > 0) ? (_shared->signaled - 1) : 0;
        result = pthread_mutex_unlock(&_shared->mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex for the named auto-reset event!", result);
#endif
    }

private:
    std::string _name;
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _event;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    // Shared auto-reset event structure
    struct EventHeader
    {
        pthread_mutex_t mutex;
        pthread_cond_t cond;
        int signaled;
    };

    // Shared auto-reset event structure wrapper
    SharedType<EventHeader> _shared;
#endif
};

NamedEventAutoReset::NamedEventAutoReset(const std::string& name, bool signaled) : _pimpl(new Impl(name, signaled))
{
}

NamedEventAutoReset::~NamedEventAutoReset()
{
}

const std::string& NamedEventAutoReset::name() const
{
    return _pimpl->name();
}

void NamedEventAutoReset::Signal()
{
    _pimpl->Signal();
}

bool NamedEventAutoReset::TryWait()
{
    return _pimpl->TryWait();
}

bool NamedEventAutoReset::TryWaitFor(int64_t nanoseconds)
{
    return _pimpl->TryWaitFor(nanoseconds);
}

void NamedEventAutoReset::Wait()
{
    _pimpl->Wait();
}

} // namespace CppCommon
