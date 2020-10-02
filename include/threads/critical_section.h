/*!
    \file critical_section.h
    \brief Critical section synchronization primitive definition
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_CRITICAL_SECTION_H
#define CPPCOMMON_THREADS_CRITICAL_SECTION_H

#include "threads/locker.h"
#include "time/timestamp.h"

#include <memory>

namespace CppCommon {

//! Critical section synchronization primitive
/*!
    Critical sections prevents code fragments from access by multiple threads simultaneously. Only one thread can
    access the code inside the critical section. Other threads must wait for the lock! Critical sections are usually
    more lightweight than mutexes and don't enter kernel mode.

    Thread-safe.

    https://en.wikipedia.org/wiki/Critical_section
*/
class CriticalSection
{
    friend class ConditionVariable;

public:
    CriticalSection();
    CriticalSection(const CriticalSection&) = delete;
    CriticalSection(CriticalSection&& cs) = delete;
    ~CriticalSection();

    CriticalSection& operator=(const CriticalSection&) = delete;
    CriticalSection& operator=(CriticalSection&& cs) = delete;

    //! Try to acquire critical section without block
    /*!
        Will not block.

        \return 'true' if the critical section was successfully acquired, 'false' if the critical section is busy
    */
    bool TryLock();

    //! Try to acquire critical section for the given timespan
    /*!
        Will block for the given timespan in the worst case.

        \param timespan - Timespan to wait for the critical section
        \return 'true' if the critical section was successfully acquired, 'false' if the critical section is busy
    */
    bool TryLockFor(const Timespan& timespan);
    //! Try to acquire critical section until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the critical section
        \return 'true' if the critical section was successfully acquired, 'false' if the critical section is busy
    */
    bool TryLockUntil(const UtcTimestamp& timestamp)
    { return TryLockFor(timestamp - UtcTimestamp()); }

    //! Acquire critical section with block
    /*!
        Will block.
    */
    void Lock();

    //! Release critical section
    /*!
        Will not block.
    */
    void Unlock();

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 64;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;

    //! Get the native critical section handler
    void* native() noexcept;
};

/*! \example threads_critical_section.cpp Critical section synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_CRITICAL_SECTION_H
