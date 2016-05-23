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
    Impl(const std::string& name, bool signaled)
    {
#if defined(_WIN32) || defined(_WIN64)
        _event = CreateEventA(nullptr, FALSE, signaled ? TRUE : FALSE, name.c_str());
        if (_event == nullptr)
            throwex SystemException("Failed to create a named auto-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_init(&_mutex, nullptr);
        if (result != 0)
            throwex SystemException("Failed to initialize a mutex for the named auto-reset event!", result);
        result = pthread_cond_init(&_cond, nullptr);
        if (result != 0)
            throwex SystemException("Failed to initialize a conditional variable for the named auto-reset event!", result);
        _signaled = signaled ? 1 : 0;
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_event))
            fatality("Failed to close a named auto-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_destroy(&_mutex);
        if (result != 0)
            fatality("Failed to destroy a mutex for the named auto-reset event!", result);
        result = pthread_cond_destroy(&_cond);
        if (result != 0)
            fatality("Failed to destroy a conditional variable for the named auto-reset event!", result);
#endif
    }

    void Signal()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!SetEvent(_event))
            throwex SystemException("Failed to signal a named auto-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the named auto-reset event!", result);
        ++_signaled;
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex for the named auto-reset event!", result);
        result = pthread_cond_signal(&_cond);
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
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the named auto-reset event!", result);
        bool signaled = (_signaled-- > 0);
        _signaled = (_signaled < 0) ? 0 : _signaled;
        result = pthread_mutex_unlock(&_mutex);
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
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the named auto-reset event!", result);
        bool signaled = true;
        while (!_signaled)
        {
            result = pthread_cond_timedwait(&_cond, &_mutex, &timeout);
            if ((result != 0) && (result != ETIMEDOUT))
                throwex SystemException("Failed to timeout waiting a conditional variable for the named auto-reset event!", result);
            if (result == ETIMEDOUT)
                signaled = (_signaled > 0);
        }
        _signaled = (_signaled > 0) ? (_signaled - 1) : 0;
        result = pthread_mutex_unlock(&_mutex);
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
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the named auto-reset event!", result);
        while (!_signaled)
        {
            result = pthread_cond_wait(&_cond, &_mutex);
            if ((result != 0) && (result != ETIMEDOUT))
                throwex SystemException("Failed to waiting a conditional variable for the named auto-reset event!", result);
        }
        _signaled = (_signaled > 0) ? (_signaled - 1) : 0;
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex for the named auto-reset event!", result);
#endif
    }

private:
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _event;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
    int _signaled;
#endif
};

NamedEventAutoReset::NamedEventAutoReset(const std::string& name, bool signaled) : _pimpl(new Impl(name, signaled)), _name(name)
{
}

NamedEventAutoReset::~NamedEventAutoReset()
{
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
