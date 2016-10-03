/*!
    \file condition_variable.cpp
    \brief Condition variable synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 03.10.2016
    \copyright MIT License
*/

#include "threads/condition_variable.h"

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

//! @cond INTERNALS

class ConditionVariable::Impl
{
public:
    Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        result = pthread_cond_init(&_cond, nullptr);
        if (result != 0)
            throwex SystemException("Failed to initialize a condition variable!", result);
#elif defined(_WIN32) || defined(_WIN64)
        InitializeConditionVariable(&_cond);
#endif
    }

    ~Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        result = pthread_cond_destroy(&_cond);
        if (result != 0)
            fatality(SystemException("Failed to destroy a condition variable!", result));
#elif defined(_WIN32) || defined(_WIN64)
        // Condition variables do not need to be explicitly destroyed.
#endif
    }

    void NotifyOne()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        result = pthread_cond_signal(&_cond);
        if (result != 0)
            throwex SystemException("Failed to signal a condition variable!", result);
#elif defined(_WIN32) || defined(_WIN64)
        WakeConditionVariable(&_cond);
#endif
    }

    void NotifyAll()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        result = pthread_cond_broadcast(&_cond);
        if (result != 0)
            throwex SystemException("Failed to broadcast a condition variable!", result);
#elif defined(_WIN32) || defined(_WIN64)
        WakeAllConditionVariable(&_cond);
#endif
    }

    void Wait(CriticalSection& cs)
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        result = pthread_cond_wait(&_cond, (pthread_mutex_t*)cs.native());
        if (result != 0)
            throwex SystemException("Failed to waiting a condition variable!", result);
#elif defined(_WIN32) || defined(_WIN64)
        if (!SleepConditionVariableCS(&_cond, (CRITICAL_SECTION*)cs.native(), INFINITE))
            throwex SystemException("Failed to waiting a condition variable!");
#endif
    }

    bool TryWaitFor(CriticalSection& cs, const Timespan& timespan)
    {
        if (timespan < 0)
            return false;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        struct timespec timeout;
        timeout.tv_sec = timespan.seconds();
        timeout.tv_nsec = timespan.nanoseconds() % 1000000000;
        result = pthread_cond_timedwait(&_cond, (pthread_mutex_t*)cs.native(), &timeout);
        if ((result != 0) && (result != ETIMEDOUT))
            throwex SystemException("Failed to waiting a condition variable for the given timeout!", result);
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64)
        if (!SleepConditionVariableCS(&_cond, (CRITICAL_SECTION*)cs.native(), (DWORD)std::max(0ll, timespan.milliseconds())))
        {
            if (GetLastError() != ERROR_TIMEOUT)
                throwex SystemException("Failed to waiting a condition variable for the given timeout!");
            return false;
        }
        return true;
#endif
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    pthread_cond_t _cond;
#elif defined(_WIN32) || defined(_WIN64)
    CONDITION_VARIABLE _cond;
#endif
};

//! @endcond

ConditionVariable::ConditionVariable() : _pimpl(std::make_unique<Impl>())
{
}

ConditionVariable::ConditionVariable(ConditionVariable&& cv) noexcept : _pimpl(std::move(cv._pimpl))
{
}

ConditionVariable::~ConditionVariable()
{
}

ConditionVariable& ConditionVariable::operator=(ConditionVariable&& cv) noexcept
{
    _pimpl = std::move(cv._pimpl);
    return *this;
}

void ConditionVariable::NotifyOne()
{
    _pimpl->NotifyOne();
}

void ConditionVariable::NotifyAll()
{
    _pimpl->NotifyAll();
}

void ConditionVariable::Wait(CriticalSection& cs)
{
    _pimpl->Wait(cs);
}

bool ConditionVariable::TryWaitFor(CriticalSection& cs, const Timespan& timespan)
{
    return _pimpl->TryWaitFor(cs, timespan);
}

} // namespace CppCommon
