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

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <pthread.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef max
#undef min
#endif

namespace CppCommon {

class EventManualReset::Impl
{
public:
    Impl(bool signaled)
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_init(&_mutex, nullptr);
        if (result != 0)
            throwex SystemException("Failed to initialize a mutex for the manual-reset event!", result);
        result = pthread_cond_init(&_cond, nullptr);
        if (result != 0)
            throwex SystemException("Failed to initialize a conditional variable for the manual-reset event!", result);
        _signaled = signaled;
#elif defined(_WIN32) || defined(_WIN64)
        _event = CreateEvent(nullptr, TRUE, signaled ? TRUE : FALSE, nullptr);
        if (_event == nullptr)
            throwex SystemException("Failed to create a manual-reset event!");
#endif
    }

    ~Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_destroy(&_mutex);
        if (result != 0)
            fatality(SystemException("Failed to destroy a mutex for the manual-reset event!", result));
        result = pthread_cond_destroy(&_cond);
        if (result != 0)
            fatality(SystemException("Failed to destroy a conditional variable for the manual-reset event!", result));
#elif defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_event))
            fatality(SystemException("Failed to close a manual-reset event!"));
#endif
    }

    void Reset()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the manual-reset event!", result);
        _signaled = false;
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex for the manual-reset event!", result);
#elif defined(_WIN32) || defined(_WIN64)
        if (!ResetEvent(_event))
            throwex SystemException("Failed to reset a manual-reset event!");
#endif
    }

    void Signal()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the manual-reset event!", result);
        _signaled = true;
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex for the manual-reset event!", result);
        result = pthread_cond_broadcast(&_cond);
        if (result != 0)
            throwex SystemException("Failed to signal an manual-reset event!", result);
#elif defined(_WIN32) || defined(_WIN64)
        if (!SetEvent(_event))
            throwex SystemException("Failed to signal a manual-reset event!");
#endif
    }

    bool TryWait()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the manual-reset event!", result);
        bool signaled = _signaled;
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex for the manual-reset event!", result);
        return signaled;
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_event, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a manual-reset event!");
        return (result == WAIT_OBJECT_0);
#endif
    }

    bool TryWaitFor(const Timespan& timespan)
    {
        if (timespan < 0)
            return TryWait();
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        struct timespec timeout;
        timeout.tv_sec = timespan.seconds();
        timeout.tv_nsec = timespan.nanoseconds() % 1000000000;
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the manual-reset event!", result);
        bool signaled = true;
        while (!_signaled)
        {
            result = pthread_cond_timedwait(&_cond, &_mutex, &timeout);
            if ((result != 0) && (result != ETIMEDOUT))
                throwex SystemException("Failed to timeout waiting a conditional variable for the manual-reset event!", result);
            if (result == ETIMEDOUT)
                signaled = _signaled;
        }
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex for the manual-reset event!", result);
        return signaled;
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_event, (DWORD)std::max(1ll, timespan.milliseconds()));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a manual-reset event for the given timeout!");
        return (result == WAIT_OBJECT_0);
#endif
    }

    void Wait()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to lock a mutex for the manual-reset event!", result);
        while (!_signaled)
        {
            result = pthread_cond_wait(&_cond, &_mutex);
            if ((result != 0) && (result != ETIMEDOUT))
                throwex SystemException("Failed to waiting a conditional variable for the manual-reset event!", result);
        }
        result = pthread_mutex_unlock(&_mutex);
        if (result != 0)
            throwex SystemException("Failed to unlock a mutex for the manual-reset event!", result);
#elif defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_event, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a manual-reset event!");
#endif
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
    bool _signaled;
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE _event;
#endif
};

EventManualReset::EventManualReset(bool signaled) : _pimpl(std::make_unique<Impl>(signaled))
{
}

EventManualReset::EventManualReset(EventManualReset&& event) noexcept : _pimpl(std::move(event._pimpl))
{
}

EventManualReset::~EventManualReset()
{
}

EventManualReset& EventManualReset::operator=(EventManualReset&& event) noexcept
{
    _pimpl = std::move(event._pimpl);
    return *this;
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

bool EventManualReset::TryWaitFor(const Timespan& timespan)
{
    return _pimpl->TryWaitFor(timespan);
}

void EventManualReset::Wait()
{
    _pimpl->Wait();
}

} // namespace CppCommon
