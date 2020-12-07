/*!
    \file thread.h
    \brief Thread definition
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_THREAD_H
#define CPPCOMMON_THREADS_THREAD_H

#include "errors/exceptions_handler.h"
#include "time/timestamp.h"

#include <bitset>
#include <thread>

//! Current thread Id macro
/*!
    Get the current thread Id.
*/
#define __THREAD__ CppCommon::Thread::CurrentThreadId()

namespace CppCommon {

//! Thread priorities
enum class ThreadPriority : uint8_t
{
    IDLE     = 0x00,    //!< Idle thread priority
    LOWEST   = 0x1F,    //!< Lowest thread priority
    LOW      = 0x3F,    //!< Low thread priority
    NORMAL   = 0x7F,    //!< Normal thread priority
    HIGH     = 0x9F,    //!< High thread priority
    HIGHEST  = 0xBF,    //!< Highest thread priority
    REALTIME = 0xFF     //!< Realtime thread priority
};

//! Stream output: Thread priorities
/*!
    \param stream - Output stream
    \param priority - Thread priority
    \return Output stream
*/
template <class TOutputStream>
TOutputStream& operator<<(TOutputStream& stream, ThreadPriority priority);

//! Thread abstraction
/*!
    Thread contains different kinds of thread manipulation  functionality  such  as
    retrive the current thread Id, sleep for the given time period in  nanoseconds,
    etc.

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

    //! Get the current thread CPU affinity
    /*!
        \return Current thread CPU affinity
    */
    static uint32_t CurrentThreadAffinity() noexcept;

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
    static void SleepUntil(const UtcTimestamp& timestamp) noexcept
    { SleepFor(timestamp - UtcTimestamp()); }

    //! Yield to other threads
    static void Yield() noexcept;

    //! Get the current thread CPU affinity bitset
    /*!
        \return CPU affinity bitset of the current thread
    */
    static std::bitset<64> GetAffinity();
    //! Get the given thread CPU affinity bitset
    /*!
        \param thread - Thread
        \return CPU affinity bitset of the given thread
    */
    static std::bitset<64> GetAffinity(std::thread& thread);

    //! Set the current thread CPU affinity bitset
    /*!
        \param affinity - Thread CPU affinity bitset
    */
    static void SetAffinity(const std::bitset<64>& affinity);
    //! Set the given thread CPU affinity bitset
    /*!
        \param thread - Thread
        \param affinity - Thread CPU affinity bitset
    */
    static void SetAffinity(std::thread& thread, const std::bitset<64>& affinity);

    //! Get the current thread priority
    /*!
        \return Priority of the current thread
    */
    static ThreadPriority GetPriority();
    //! Get the given thread priority
    /*!
        \param thread - Thread
        \return Priority of the given thread
    */
    static ThreadPriority GetPriority(std::thread& thread);

    //! Set the current thread priority
    /*!
        \param priority - Thread priority
    */
    static void SetPriority(ThreadPriority priority);
    //! Set the given thread priority
    /*!
        \param thread - Thread
        \param priority - Thread priority
    */
    static void SetPriority(std::thread& thread, ThreadPriority priority);
};

/*! \example threads_thread.cpp Thread example */

} // namespace CppCommon

#include "thread.inl"

#endif // CPPCOMMON_THREADS_THREAD_H
