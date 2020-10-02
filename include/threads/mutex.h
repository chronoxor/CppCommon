/*!
    \file mutex.h
    \brief Mutex synchronization primitive definition
    \author Ivan Shynkarenka
    \date 04.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_MUTEX_H
#define CPPCOMMON_THREADS_MUTEX_H

#include "threads/locker.h"
#include "time/timestamp.h"

#include <memory>

namespace CppCommon {

//! Mutex synchronization primitive
/*!
    A mutex object facilitates protection against data races and allows thread-safe synchronization
    of data between threads. A thread obtains ownership of a mutex object by calling one of the lock
    functions and relinquishes ownership by calling the corresponding unlock function.

    Thread-safe.

    https://en.wikipedia.org/wiki/Mutual_exclusion
*/
class Mutex
{
public:
    Mutex();
    Mutex(const Mutex&) = delete;
    Mutex(Mutex&& mutex) = delete;
    ~Mutex();

    Mutex& operator=(const Mutex&) = delete;
    Mutex& operator=(Mutex&& mutex) = delete;

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

    static const size_t StorageSize = 64;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example threads_mutex.cpp Mutex synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_MUTEX_H
