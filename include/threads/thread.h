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
#include "time/timestamp.h"

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

    //! Sleep the current thread for the given milliseconds
    /*!
        \param milliseconds - Milliseconds to sleep
    */
    static void Sleep(int64_t milliseconds) noexcept
    { SleepFor(Timespan::milliseconds(milliseconds)); }
    //! Sleep the current thread for the given timespan
    /*!
        \param timespan - Timespan to sleep
    */
    static void SleepFor(const Timespan& timespan) noexcept;
    //! Sleep the current thread until the given timestamp
    /*!
        \param timestamp - Timestamp to stop sleeping
    */
    static void SleepUntil(const NanoTimestamp& timestamp) noexcept
    { SleepFor(timestamp - NanoTimestamp()); }

    //! Yield to other threads
    static void Yield() noexcept;
};

} // namespace CppCommon

#include "thread.inl"

#endif // CPPCOMMON_THREADS_THREAD_H
