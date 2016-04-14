/*!
    \file event_manual_reset.cpp
    \brief Manual-reset event synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 14.04.2016
    \copyright MIT License
*/

#include "threads/event_manual_reset.h"

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

class EventManualReset::Impl
{
public:
    Impl(bool signaled)
    {
#if defined(_WIN32) || defined(_WIN64)
        _event = CreateEvent(nullptr, TRUE, signaled ? TRUE : FALSE, nullptr);
        if (_event == nullptr)
            throwex SystemException("Failed to create a manual-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_init(&_mutex, nullptr);
        if (result != 0)
            throwex SystemException(result, "Failed to initialize a mutex for the manual-reset event!");
        result = pthread_cond_init(&_cond, nullptr);
        if (result != 0)
            throwex SystemException(result, "Failed to initialize a conditional variable for the manual-reset event!");
        _signaled = signaled;
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_event))
            fatality("Failed to close a manual-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_destroy(&_mutex);
        if (result != 0)
            fatality("Failed to destroy a mutex for the manual-reset event!", result);
        result = pthread_cond_destroy(&_cond);
        if (result != 0)
            fatality("Failed to destroy a conditional variable for the manual-reset event!", result);
#endif
    }

    void Reset()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!ResetEvent(_event))
            throwex SystemException("Failed to reset a manual-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException(result, "Failed to lock a mutex for the manual-reset event!");
        _signaled = false;
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException(result, "Failed to unlock a mutex for the manual-reset event!");
#endif
    }

    void Signal()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!SetEvent(_event))
            throwex SystemException("Failed to signal a manual-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException(result, "Failed to lock a mutex for the manual-reset event!");
        _signaled = true;
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException(result, "Failed to unlock a mutex for the manual-reset event!");
        result = pthread_cond_broadcast(&_cond);
        if (result != 0)
            throwex SystemException(result, "Failed to signal an manual-reset event!");
#endif
    }

    bool TryWait()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_event, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a manual-reset event!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException(result, "Failed to lock a mutex for the manual-reset event!");
        bool signaled = _signaled;
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException(result, "Failed to unlock a mutex for the manual-reset event!");
        return signaled;
#endif
    }

    bool TryWaitFor(int64_t nanoseconds)
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_event, (DWORD)std::max(1ll, nanoseconds / 1000000000));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a manual-reset event for the given timeout!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        struct timespec timeout;
        timeout.tv_sec = nanoseconds / 1000000000;
        timeout.tv_nsec = nanoseconds % 1000000000;
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException(result, "Failed to lock a mutex for the manual-reset event!");
        bool signaled = true;
        while (!_signaled)
        {
            result = pthread_cond_timedwait(&_cond, &_mutex, &timeout);
            if ((result != 0) && (result != ETIMEDOUT))
                throwex SystemException(result, "Failed to timeout waiting a conditional variable for the manual-reset event!");
            if (result == ETIMEDOUT)
                signaled = _signaled;
        }
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException(result, "Failed to unlock a mutex for the manual-reset event!");
        return signaled;
#endif
    }

    void Wait()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_event, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a manual-reset event!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException(result, "Failed to lock a mutex for the manual-reset event!");
        while (!_signaled)
        {
            result = pthread_cond_wait(&_cond, &_mutex);
            if ((result != 0) && (result != ETIMEDOUT))
                throwex SystemException(result, "Failed to waiting a conditional variable for the manual-reset event!");
        }
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException(result, "Failed to unlock a mutex for the manual-reset event!");
#endif
    }

private:
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _event;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
    bool _signaled;
#endif
};

EventManualReset::EventManualReset(bool signaled) : _pimpl(new Impl(signaled))
{
}

EventManualReset::~EventManualReset()
{
}

void EventManualReset::Reset()
{
    _pimpl->Reset();
}

void EventManualReset::Signal()
{
    _pimpl->Signal();
}

bool EventManualReset::TryWait()
{
    return _pimpl->TryWait();
}

bool EventManualReset::TryWaitFor(int64_t nanoseconds)
{
    return _pimpl->TryWaitFor(nanoseconds);
}

void EventManualReset::Wait()
{
    _pimpl->Wait();
}

} // namespace CppCommon
