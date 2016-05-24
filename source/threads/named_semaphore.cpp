/*!
    \file named_semaphore.cpp
    \brief Named semaphore synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 14.04.2016
    \copyright MIT License
*/

#include "threads/named_semaphore.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"

#include <algorithm>
#include <cassert>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef max
#undef min
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <fcntl.h>
#include <semaphore.h>
#endif

namespace CppCommon {

class NamedSemaphore::Impl
{
public:
    Impl(const std::string& name, int resources) : _name(name), _resources(resources)
    {
        assert((resources > 0) && "Named semaphore resources counter must be greater than zero!");

#if defined(_WIN32) || defined(_WIN64)
        _semaphore = CreateSemaphoreA(nullptr, resources, resources, name.c_str());
        if (_semaphore == nullptr)
            throwex SystemException("Failed to create or open a named semaphore!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        _owner = true;
        // Try to create a named binary semaphore
        _semaphore = sem_open(name.c_str(), (O_CREAT | O_EXCL), 0666, resources);
        if (_semaphore == SEM_FAILED)
        {
            // Try to open a named binary semaphore
            _semaphore = sem_open(name.c_str(), O_CREAT, 0666, resources);
            if (_semaphore == SEM_FAILED)
                throwex SystemException("Failed to initialize a named semaphore!");
            else
                _owner = false;
        }
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_semaphore))
            fatality("Failed to close a named semaphore!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_close(_semaphore);
        if (result != 0)
            fatality("Failed to close a named semaphore!");
        // Unlink the named semaphore (owner only)
        if (_owner)
        {
            result = sem_unlink(_name.c_str());
            if (result != 0)
                fatality("Failed to unlink a named semaphore!");
        }
#endif
    }

    const std::string& name() const
    {
        return _name;
    }

    int resources() const noexcept
    {
        return _resources;
    }

    bool TryLock()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_semaphore, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named semaphore!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_trywait(_semaphore);
        if ((result != 0) && (errno != EAGAIN))
            throwex SystemException("Failed to try lock a named semaphore!");
        return (result == 0);
#endif
    }

    bool TryLockFor(int64_t nanoseconds)
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_semaphore, (DWORD)std::max(1ll, nanoseconds / 1000000000));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named semaphore for the given timeout!");
        return (result == WAIT_OBJECT_0);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        struct timespec timeout;
        timeout.tv_sec = nanoseconds / 1000000000;
        timeout.tv_nsec = nanoseconds % 1000000000;
        int result = sem_timedwait(_semaphore, &timeout);
        if ((result != 0) && (errno != ETIMEDOUT))
            throwex SystemException("Failed to try lock a named semaphore for the given timeout!");
        return (result == 0);
#endif
    }

    void Lock()
    {
#if defined(_WIN32) || defined(_WIN64)
        DWORD result = WaitForSingleObject(_semaphore, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a named semaphore!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_wait(_semaphore);
        if (result != 0)
            throwex SystemException("Failed to lock a named semaphore!");
#endif
    }

    void Unlock()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!ReleaseSemaphore(_semaphore, 1, nullptr))
            throwex SystemException("Failed to unlock a named semaphore!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = sem_post(_semaphore);
        if (result != 0)
            throwex SystemException("Failed to unlock a named semaphore!");
#endif
    }

private:
    std::string _name;
    int _resources;
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _semaphore;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    sem_t* _semaphore;
    bool _owner;
#endif
};

NamedSemaphore::NamedSemaphore(const std::string& name, int resources) : _pimpl(new Impl(name, resources))
{
}

NamedSemaphore::~NamedSemaphore()
{
}

const std::string& NamedSemaphore::name() const
{
    return _pimpl->name();
}

int NamedSemaphore::resources() const noexcept
{
    return _pimpl->resources();
}

bool NamedSemaphore::TryLock()
{
    return _pimpl->TryLock();
}

bool NamedSemaphore::TryLockFor(int64_t nanoseconds)
{
    return _pimpl->TryLockFor(nanoseconds);
}

void NamedSemaphore::Lock()
{
    _pimpl->Lock();
}

void NamedSemaphore::Unlock()
{
    _pimpl->Unlock();
}

} // namespace CppCommon
