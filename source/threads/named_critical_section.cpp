/*!
    \file named_critical_section.cpp
    \brief Named critical section synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 26.05.2016
    \copyright MIT License
*/

#include "threads/named_critical_section.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"
#include "system/shared_type.h"
#include "threads/named_event_auto_reset.h"
#include "threads/thread.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef Yield
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#endif

namespace CppCommon {

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
#if defined(_WIN32) || defined(_WIN64)
            if (InterlockedCompareExchange(&_shared->lock_count, 1, 0) == 0)
#elif defined(unix) || defined(__unix) || defined(__unix__)
            if (__sync_bool_compare_and_swap(&_shared->lock_count, 0, 1))
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
#if defined(_WIN32) || defined(_WIN64)
                InterlockedIncrement(&_shared->lock_count);
#elif defined(unix) || defined(__unix) || defined(__unix__)
                __sync_add_and_fetch(&_shared->lock_count, 1);
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

#if defined(_WIN32) || defined(_WIN64)
        if (InterlockedIncrement(&_shared->lock_count) == 1)
#elif defined(unix) || defined(__unix) || defined(__unix__)
        if (__sync_add_and_fetch(&_shared->lock_count, 1) == 1)
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
#if defined(_WIN32) || defined(_WIN64)
            InterlockedDecrement(&_shared->lock_count);
#elif defined(unix) || defined(__unix) || defined(__unix__)
            __sync_sub_and_fetch(&_shared->lock_count, 1);
#endif
        }
        else
        {
            // We don't own the named critical section anymore
            _shared->thread_id = 0;

#if defined(_WIN32) || defined(_WIN64)
            if (InterlockedDecrement(&_shared->lock_count) > 0)
#elif defined(unix) || defined(__unix) || defined(__unix__)
            if (__sync_sub_and_fetch(&_shared->lock_count, 1) > 0)
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
        long lock_count;
        long recurse_count;
        uint64_t thread_id;

        CriticalSectionHeader() : lock_count(0), recurse_count(0), thread_id(0) {}
    };

    // Shared critical section structure wrapper
    SharedType<CriticalSectionHeader> _shared;
    NamedEventAutoReset _event;
    uint32_t _spin;
};

NamedCriticalSection::NamedCriticalSection(const std::string& name) : _pimpl(std::make_unique<Impl>(name, 4000))
{
}

NamedCriticalSection::~NamedCriticalSection()
{
}

const std::string& NamedCriticalSection::name() const
{
    return _pimpl->name();
}

bool NamedCriticalSection::TryLock()
{
    return _pimpl->TryLock();
}

void NamedCriticalSection::Lock()
{
    _pimpl->Lock();
}

void NamedCriticalSection::Unlock()
{
    _pimpl->Unlock();
}

} // namespace CppCommon
