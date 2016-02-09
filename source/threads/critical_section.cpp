/*!
    \file critical_section.cpp
    \brief Critical section synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#include "threads/critical_section.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#endif

namespace CppCommon {

class CriticalSection::Impl
{
public:
    Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        InitializeCriticalSection(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)

#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        DeleteCriticalSection(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)

#endif
    }

    bool TryLock()
    {
#if defined(_WIN32) || defined(_WIN64)
        return (TryEnterCriticalSection(&_lock) != 0);
#elif defined(unix) || defined(__unix) || defined(__unix__)

#endif
    }

    void lock()
    {
#if defined(_WIN32) || defined(_WIN64)
        EnterCriticalSection(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)

#endif
    }

    void unlock()
    {
#if defined(_WIN32) || defined(_WIN64)
        LeaveCriticalSection(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)

#endif
    }

private:
#if defined(_WIN32) || defined(_WIN64)
    CRITICAL_SECTION _lock;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    pthread_mutex_t _lock;
#endif
};

CriticalSection::CriticalSection() : _pimpl(new Impl())
{
}

CriticalSection::~CriticalSection()
{
}

bool CriticalSection::TryLock()
{
    return _pimpl->TryLock();
}

void CriticalSection::lock()
{
    _pimpl->lock();
}

void CriticalSection::unlock()
{
    _pimpl->unlock();
}

} // namespace CppCommon
