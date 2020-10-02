/*!
    \file named_mutex.h
    \brief Named mutex synchronization primitive definition
    \author Ivan Shynkarenka
    \date 15.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_NAMED_MUTEX_H
#define CPPCOMMON_THREADS_NAMED_MUTEX_H

#include "threads/locker.h"
#include "time/timestamp.h"

#include <memory>
#include <string>

namespace CppCommon {

//! Named mutex synchronization primitive
/*!
    Named mutex behaves as a simple mutex but could be shared between processes
    on the same machine.

    Thread-safe.

    \see Mutex
*/
class NamedMutex
{
public:
    //! Default class constructor
    /*!
        \param name - Mutex name
    */
    explicit NamedMutex(const std::string& name);
    NamedMutex(const NamedMutex&) = delete;
    NamedMutex(NamedMutex&& mutex) = delete;
    ~NamedMutex();

    NamedMutex& operator=(const NamedMutex&) = delete;
    NamedMutex& operator=(NamedMutex&& mutex) = delete;

    //! Get the mutex name
    const std::string& name() const;

    //! Try to acquire mutex without block
    /*!
        Will not block.

        \return 'true' if the mutex was successfully acquired, 'false' if the mutex is busy
    */
    bool TryLock();

    //! Try to acquire mutex for the given timespan
    /*!
        Will block for the given timespan in the worst case.

        \param timespan - Timespan to wait for the mutex
        \return 'true' if the mutex was successfully acquired, 'false' if the mutex is busy
    */
    bool TryLockFor(const Timespan& timespan);
    //! Try to acquire mutex until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the mutex
        \return 'true' if the mutex was successfully acquired, 'false' if the mutex is busy
    */
    bool TryLockUntil(const UtcTimestamp& timestamp)
    { return TryLockFor(timestamp - UtcTimestamp()); }

    //! Acquire mutex with block
    /*!
        Will block.
    */
    void Lock();

    //! Release mutex
    /*!
        Will not block.
    */
    void Unlock();

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 136;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example threads_named_mutex.cpp Named mutex synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_NAMED_MUTEX_H
