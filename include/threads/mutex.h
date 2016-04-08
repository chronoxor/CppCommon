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

#include <chrono>
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
    Mutex(Mutex&&) = default;
    ~Mutex();

    Mutex& operator=(const Mutex&) = delete;
    Mutex& operator=(Mutex&&) = default;

    //! Try to acquire mutex without block
    /*!
        Will not block.

        \return 'true' if the mutex was successfully acquired, 'false' if the mutex is busy
    */
    bool TryLock();

    //! Try to acquire mutex for the given nanoseconds
    /*!
        Will block for the given nanoseconds in the worst case.

        \param nanoseconds - Nanoseconds to wait for the mutex
        \return 'true' if the mutex was successfully acquired, 'false' if the mutex is busy
    */
    bool TryLockFor(int64_t nanoseconds);
    //! Try to acquire mutex for the given time duration
    /*!
        Will block for the given time duration in the worst case.

        \param duration - Time duration to wait for the mutex
        \return 'true' if the mutex was successfully acquired, 'false' if the mutex is busy
    */
    template <class Rep, class Period>
    bool TryLockFor(const std::chrono::duration<Rep, Period>& duration);
    //! Try to acquire mutex until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the mutex
        \return 'true' if the mutex was successfully acquired, 'false' if the mutex is busy
    */
    template <class Clock, class Duration>
    bool TryLockUntil(const std::chrono::time_point<Clock, Duration>& timestamp);

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
    std::unique_ptr<Impl> _pimpl;
};

/*! \example threads_mutex.cpp Mutex synchronization primitive example */

} // namespace CppCommon

#include "mutex.inl"

#endif // CPPCOMMON_THREADS_MUTEX_H
