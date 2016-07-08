/*!
    \file thread.h
    \brief Thread abstraction definition
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_THREAD_H
#define CPPCOMMON_THREADS_THREAD_H

#include "errors/exceptions_handler.h"

#include <chrono>
#include <thread>

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
    static uint64_t CurrentThreadId() noexcept;

    //! Start a new thread with an exception handler registered
    /*!
        Works the same way as std::thread() does but also register an exception handler
        with ExceptionsHandler.SetupThread() call.

        \param fn - Thread function
        \param args - Thread function arguments
        \return Instance of a new thread
    */
    template <class Fn, class... Args>
    static std::thread Start(Fn&& fn, Args&&... args);

    //! Sleep the current thread for the given nanoseconds
    /*!
        \param nanoseconds - Nanoseconds to sleep
    */
    static void Sleep(int64_t nanoseconds) noexcept;
    //! Sleep the current thread for the given time duration
    /*!
        \param duration - Time duration to sleep
    */
    template <class Rep, class Period>
    static void SleepFor(const std::chrono::duration<Rep, Period>& duration) noexcept
    { Sleep(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()); }
    //! Sleep the current thread until the given timestamp
    /*!
        \param timestamp - Timestamp to stop sleeping
    */
    template <class Clock, class Duration>
    static void SleepUntil(const std::chrono::time_point<Clock, Duration>& timestamp) noexcept
    { SleepFor(timestamp - std::chrono::high_resolution_clock::now()); }

    //! Yield to other threads
    static void Yield() noexcept;
};

} // namespace CppCommon

#include "thread.inl"

#endif // CPPCOMMON_THREADS_THREAD_H
