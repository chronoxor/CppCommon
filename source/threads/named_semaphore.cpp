/*!
    \file named_semaphore.cpp
    \brief Named semaphore synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 14.04.2016
    \copyright MIT License
*/

#include "threads/named_semaphore.h"

#include "errors/fatal.h"
#include "utility/validate_aligned_storage.h"

#include <algorithm>
#include <cassert>

#if defined(__APPLE__)
#include "threads/thread.h"
#endif
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
#include <fcntl.h>
#include <semaphore.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#undef Yield
#undef max
#undef min
#endif

namespace CppCommon {

//! @cond INTERNALS

class NamedSemaphore::Impl
{
public:
    Impl(const std::string& name, int resources) : _name(name), _resources(resources)
    {
        assert((resources > 0) && "Named semaphore resources counter must be greater than zero!");

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
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
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        _semaphore = CreateSemaphoreA(nullptr, resources, resources, name.c_str());
        if (_semaphore == nullptr)
            throwex SystemException("Failed to create or open a named semaphore!");
#endif
    }

    ~Impl()
    {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = sem_close(_semaphore);
        if (result != 0)
            fatality(SystemException("Failed to close a named semaphore!"));
        // Unlink the named semaphore (owner only)
        if (_owner)
        {
            result = sem_unlink(_name.c_str());
            if (result != 0)
                fatality(SystemException("Failed to unlink a named semaphore!"));
        }
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        if (!CloseHandle(_semaphore))
            fatality(SystemException("Failed to close a named semaphore!"));
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
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = sem_trywait(_semaphore);
        if ((result != 0) && (errno != EAGAIN))
            throwex SystemException("Failed to try lock a named semaphore!");
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        DWORD result = WaitForSingleObject(_semaphore, 0);
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named semaphore!");
        return (result == WAIT_OBJECT_0);
#endif
    }

    bool TryLockFor(const Timespan& timespan)
    {
        if (timespan < 0)
            return TryLock();
#if defined(__APPLE__)
        // Calculate a finish timestamp
        Timestamp finish = NanoTimestamp() + timespan;

        // Try to acquire lock at least one time
        if (TryLock())
            return true;
        else
        {
            // Try lock or yield for the given timespan
            while (NanoTimestamp() < finish)
            {
                if (TryLock())
                    return true;
                else
                    Thread::Yield();
            }

            // Failed to acquire lock
            return false;
        }
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
        struct timespec timeout;
        timeout.tv_sec = timespan.seconds();
        timeout.tv_nsec = timespan.nanoseconds() % 1000000000;
        int result = sem_timedwait(_semaphore, &timeout);
        if ((result != 0) && (errno != ETIMEDOUT))
            throwex SystemException("Failed to try lock a named semaphore for the given timeout!");
        return (result == 0);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        DWORD result = WaitForSingleObject(_semaphore, std::max((DWORD)1, (DWORD)timespan.milliseconds()));
        if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT))
            throwex SystemException("Failed to try lock a named semaphore for the given timeout!");
        return (result == WAIT_OBJECT_0);
#endif
    }

    void Lock()
    {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = sem_wait(_semaphore);
        if (result != 0)
            throwex SystemException("Failed to lock a named semaphore!");
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        DWORD result = WaitForSingleObject(_semaphore, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to lock a named semaphore!");
#endif
    }

    void Unlock()
    {
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        int result = sem_post(_semaphore);
        if (result != 0)
            throwex SystemException("Failed to unlock a named semaphore!");
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        if (!ReleaseSemaphore(_semaphore, 1, nullptr))
            throwex SystemException("Failed to unlock a named semaphore!");
#endif
    }

private:
    std::string _name;
    int _resources;
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
    sem_t* _semaphore;
    bool _owner;
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    HANDLE _semaphore;
#endif
};

//! @endcond

NamedSemaphore::NamedSemaphore(const std::string& name, int resources)
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "NamedSemaphore::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "NamedSemaphore::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(name, resources);
}

NamedSemaphore::~NamedSemaphore()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

const std::string& NamedSemaphore::name() const { return impl().name(); }
int NamedSemaphore::resources() const noexcept { return impl().resources(); }

bool NamedSemaphore::TryLock() { return impl().TryLock(); }
bool NamedSemaphore::TryLockFor(const Timespan& timespan) { return impl().TryLockFor(timespan); }

void NamedSemaphore::Lock() { impl().Lock(); }
void NamedSemaphore::Unlock() { impl().Unlock(); }

} // namespace CppCommon
