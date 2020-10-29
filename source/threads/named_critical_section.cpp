/*!
    \file named_critical_section.cpp
    \brief Named critical section synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 26.05.2016
    \copyright MIT License
*/

#include "threads/named_critical_section.h"

#include "errors/fatal.h"
#include "system/shared_type.h"
#include "threads/named_event_auto_reset.h"
#include "threads/thread.h"
#include "utility/validate_aligned_storage.h"

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
#include <pthread.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#undef Yield
#endif

namespace CppCommon {

//! @cond INTERNALS

class NamedCriticalSection::Impl
{
public:
    Impl(const std::string& name, uint32_t spin) : _shared(name), _event(name + "_event"), _spin(spin) {}

    ~Impl()
    {
        if (_shared->thread_id == Thread::CurrentThreadId())
            fatality(SystemException("Named critical section should not be destroyed if our thread owns it!"));
    }

    const std::string& name() const
    {
        return _shared.name();
    }

    bool TryLock()
    {
        return TryLock(0);
    }

    bool TryLock(uint32_t spin)
    {
        uint32_t iteration = 0;
        uint64_t thread_id = Thread::CurrentThreadId();

        while (true)
        {
            // If lock count = 0, the named critical section is unowned, we can own it
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
            if (__sync_bool_compare_and_swap(&_shared->lock_count, 0, 1))
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
            if (InterlockedCompareExchange(&_shared->lock_count, 1, 0) == 0)
#endif
            {
                // The named critical section is unowned, let this thread own it once
                _shared->thread_id = thread_id;
                _shared->recurse_count = 1;
                return true;
            }
            else if (_shared->thread_id == thread_id)
            {
                // If the named critical section is owned by this thread, own it again
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
                __sync_add_and_fetch(&_shared->lock_count, 1);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                InterlockedIncrement(&_shared->lock_count);
#endif
                _shared->recurse_count++;
                return true;
            }
            else if (iteration++ >= spin)
                break;

            // Yield to other threads
            Thread::Yield();
        }

        return false;
    }

    void Lock()
    {
        // Spin, trying to get the named critical section
        if (TryLock(_spin))
            return;

        // We couldn't get the the named critical section, wait for it
        uint64_t thread_id = Thread::CurrentThreadId();

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
        if (__sync_add_and_fetch(&_shared->lock_count, 1) == 1)
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        if (InterlockedIncrement(&_shared->lock_count) == 1)
#endif
        {
            // The named critical section is unowned, let this thread own it once
            _shared->thread_id = thread_id;
            _shared->recurse_count = 1;
        }
        else
        {
            if (_shared->thread_id == thread_id)
            {
                // If the named critical section is owned by this thread, own it again
                _shared->recurse_count++;
            }
            else
            {
                // The named critical section is owned by another thread, wait for it
                _event.Wait();

                // The named critical section is unowned, let this thread own it once
                _shared->thread_id = thread_id;
                _shared->recurse_count = 1;
            }
        }
    }

    void Unlock()
    {
        if (_shared->thread_id != Thread::CurrentThreadId())
            throwex SystemException("Named critical section can not be unlocked from other thread!");

        // Reduce this thread's ownership of the named critical section
        if (--_shared->recurse_count > 0)
        {
            // We still own the named critical section
#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
            __sync_sub_and_fetch(&_shared->lock_count, 1);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
            InterlockedDecrement(&_shared->lock_count);
#endif
        }
        else
        {
            // We don't own the named critical section anymore
            _shared->thread_id = 0;

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)) && !defined(__CYGWIN__)
            if (__sync_sub_and_fetch(&_shared->lock_count, 1) > 0)
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
            if (InterlockedDecrement(&_shared->lock_count) > 0)
#endif
            {
                // Other threads are waiting, the auto-reset event wakes one of them
                _event.Signal();
            }
        }
    }

private:
    // Shared critical section structure
    struct CriticalSectionHeader
    {
        volatile uint64_t lock_count;
        int recurse_count;
        uint64_t thread_id;

        CriticalSectionHeader() : lock_count(0), recurse_count(0), thread_id(0) {}
    };

    // Shared critical section structure wrapper
    SharedType<CriticalSectionHeader> _shared;
    NamedEventAutoReset _event;
    uint32_t _spin;
};

//! @endcond

NamedCriticalSection::NamedCriticalSection(const std::string& name)
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "NamedCriticalSection::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "NamedCriticalSection::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(name, 4000);
}

NamedCriticalSection::~NamedCriticalSection()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

const std::string& NamedCriticalSection::name() const { return impl().name(); }

bool NamedCriticalSection::TryLock() { return impl().TryLock(); }

bool NamedCriticalSection::TryLockFor(const Timespan& timespan)
{
    // Calculate a finish timestamp
    Timestamp finish = NanoTimestamp() + timespan;

    // Try to acquire named critical section at least one time
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

        // Failed to acquire named critical section
        return false;
    }
}

void NamedCriticalSection::Lock() { impl().Lock(); }
void NamedCriticalSection::Unlock() { impl().Unlock(); }

} // namespace CppCommon
