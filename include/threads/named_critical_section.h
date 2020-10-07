/*!
    \file named_critical_section.h
    \brief Named critical section synchronization primitive definition
    \author Ivan Shynkarenka
    \date 26.05.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_NAMED_CRITICAL_SECTION_H
#define CPPCOMMON_THREADS_NAMED_CRITICAL_SECTION_H

#include "threads/locker.h"
#include "time/timestamp.h"

#include <memory>
#include <string>

namespace CppCommon {

//! Named critical section synchronization primitive
/*!
    Named critical section behaves as a simple critical section but could be shared
    between processes on the same machine.

    Thread-safe.

    \see CriticalSection
*/
class NamedCriticalSection
{
public:
    //! Default class constructor
    /*!
        \param name - Critical section name
    */
    explicit NamedCriticalSection(const std::string& name);
    NamedCriticalSection(const NamedCriticalSection&) = delete;
    NamedCriticalSection(NamedCriticalSection&& cs) = delete;
    ~NamedCriticalSection();

    NamedCriticalSection& operator=(const NamedCriticalSection&) = delete;
    NamedCriticalSection& operator=(NamedCriticalSection&& cs) = delete;

    //! Get the critical section name
    const std::string& name() const;

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

    static const size_t StorageSize = 256;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example threads_named_critical_section.cpp Named critical section synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_NAMED_CRITICAL_SECTION_H
