/*!
    \file thread.h
    \brief Thread abstraction definition
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_THREAD_H
#define CPPCOMMON_THREADS_THREAD_H

#include <chrono>

//! Current thread Id macro
/*!
    Get the current thread Id.
*/
#define __THREAD__ CppCommon::Thread::CurrentThreadId()

namespace CppCommon {

//! Thread abstraction
/*!
    Thread abstraction contains different kinds of thread manipulation functionality such as retrive the current thread
    Id, sleep for the given time period in nanoseconds, etc.

    Thread-safe.
*/
class Thread
{
public:
    Thread() = delete;
    Thread(const Thread&) = delete;
    Thread(Thread&&) = delete;
    ~Thread() = delete;

    Thread& operator=(const Thread&) = delete;
    Thread& operator=(Thread&&) = delete;

    //! Get the current thread Id
    /*!
        \return Current thread Id
    */
    static uint64_t CurrentThreadId();

    //! Sleep the current thread for the given nanoseconds
    /*!
        \param nanoseconds - Nanoseconds to sleep
    */
    static void Sleep(int64_t nanoseconds);
    //! Sleep the current thread for the given time duration
    /*!
        \param duration - Time duration to sleep
    */
    template <class Rep, class Period>
    static void SleepFor(const std::chrono::duration<Rep, Period>& duration);
    //! Sleep the current thread until the given timestamp
    /*!
        \param timestamp - Timestamp to stop sleeping
    */
    template <class Clock, class Duration>
    static void SleepUntil(const std::chrono::time_point<Clock, Duration>& timestamp);

    //! Yield to other threads
    static void Yield();
};

} // namespace CppCommon

#include "thread.inl"

#endif // CPPCOMMON_THREADS_THREAD_H
