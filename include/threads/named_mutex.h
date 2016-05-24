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

#include <chrono>
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
    NamedMutex(NamedMutex&&) = default;
    ~NamedMutex();

    NamedMutex& operator=(const NamedMutex&) = delete;
    NamedMutex& operator=(NamedMutex&&) = default;

    //! Get the mutex name
    const std::string& name() const;

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
    bool TryLockFor(const std::chrono::duration<Rep, Period>& duration)
    { return TryLockFor(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()); }
    //! Try to acquire mutex until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the mutex
        \return 'true' if the mutex was successfully acquired, 'false' if the mutex is busy
    */
    template <class Clock, class Duration>
    bool TryLockUntil(const std::chrono::time_point<Clock, Duration>& timestamp)
    { return TryLockFor(timestamp - std::chrono::high_resolution_clock::now()); }

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

/*! \example threads_named_mutex.cpp Named mutex synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_NAMED_MUTEX_H
