/*!
    \file critical_section.cpp
    \brief Critical section synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#include "threads/critical_section.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"

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
        pthread_mutexattr_t attribute;
        int result = pthread_mutexattr_init(&attribute);
        if (result != 0)
            throwex SystemException(result, "Failed to initialize a mutex attribute!");
        result = pthread_mutexattr_settype(&attribute, PTHREAD_MUTEX_RECURSIVE);
        if (result != 0)
            throwex SystemException(result, "Failed to set a mutex recursive attribute!");
        result = pthread_mutex_init(&_lock, &attribute);
        if (result != 0)
            throwex SystemException(result, "Failed to initialize a mutex!");
        result = pthread_mutexattr_destroy(&attribute);
        if (result != 0)
            throwex SystemException(result, "Failed to destroy a mutex attribute!");
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        DeleteCriticalSection(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_destroy(&_lock);
        if (result != 0)
            fatality("Failed to destroy a mutex!", result);
#endif
    }

    bool TryLock()
    {
#if defined(_WIN32) || defined(_WIN64)
        return (TryEnterCriticalSection(&_lock) != 0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_trylock(&_lock);
        if ((result != 0) && (result != EBUSY))
            throwex SystemException(result, "Failed to try lock a mutex!");
        return (result == 0);
#endif
    }

    void Lock()
    {
#if defined(_WIN32) || defined(_WIN64)
        EnterCriticalSection(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_lock(&_lock);
        if (result != 0)
            throwex SystemException(result, "Failed to lock a mutex!");
#endif
    }

    void Unlock()
    {
#if defined(_WIN32) || defined(_WIN64)
        LeaveCriticalSection(&_lock);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_unlock(&_lock);
        if (result != 0)
            throwex SystemException(result, "Failed to unlock a mutex!");
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

void CriticalSection::Lock()
{
    _pimpl->Lock();
}

void CriticalSection::Unlock()
{
    _pimpl->Unlock();
}

} // namespace CppCommon
