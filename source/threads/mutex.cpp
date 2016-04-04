/*!
    \file mutex.cpp
    \brief Mutex synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 04.04.2016
    \copyright MIT License
*/

#include "threads/mutex.h"

#include "errors/exceptions.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#endif

namespace CppCommon {

class Mutex::Impl
{
public:
    Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        _mutex = CreateMutex(nullptr, FALSE, nullptr);
        if (_mutex == nullptr)
            throwex SystemException("Failed to create a mutex!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_init(&_lock, nullptr);
        if (result != 0)
            throwex SystemException(result, "Failed to initialize a mutex!");
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        CloseHandle(_mutex);
        // TODO: warning C4297: function assumed not to throw an exception but does
        //if (!CloseHandle(_mutex))
        //    throwex SystemException("Failed to close a mutex!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_destroy(&_lock);
        if (result != 0)
            throwex SystemException(result, "Failed to destroy a mutex!");
#endif
    }

    bool TryLock()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_mutex, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a mutex!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_trylock(&_lock);
        if ((result != 0) && (result != EBUSY))
            throwex SystemException(result, "Failed to try lock a mutex!");
        return (result == 0);
#endif
    }

    void lock()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_mutex, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a mutex!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_lock(&_lock);
        if (result != 0)
            throwex SystemException(result, "Failed to lock a mutex!");
#endif
    }

    void unlock()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!ReleaseMutex(_mutex))
            throwex SystemException("Failed to unlock a mutex!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_mutex_unlock(&_lock);
        if (result != 0)
            throwex SystemException(result, "Failed to unlock a mutex!");
#endif
    }

private:
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _mutex;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    pthread_mutex_t _mutex;
#endif
};

Mutex::Mutex() : _pimpl(new Impl())
{
}

Mutex::~Mutex()
{
}

bool Mutex::TryLock()
{
    return _pimpl->TryLock();
}

void Mutex::lock()
{
    _pimpl->lock();
}

void Mutex::unlock()
{
    _pimpl->unlock();
}

} // namespace CppCommon
