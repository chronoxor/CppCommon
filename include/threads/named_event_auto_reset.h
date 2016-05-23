/*!
    \file named_event_auto_reset.h
    \brief Named auto-reset event synchronization primitive definition
    \author Ivan Shynkarenka
    \date 24.05.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_NAMED_EVENT_AUTO_RESET_H
#define CPPCOMMON_THREADS_NAMED_EVENT_AUTO_RESET_H

#include <chrono>
#include <memory>
#include <string>

namespace CppCommon {

//! Named auto-reset event synchronization primitive
/*!
    Named auto-reset event behaves as a simple auto-reset event but could be shared
    between processes on the same machine.

    Thread-safe.

    \see EventAutoReset
*/
class NamedEventAutoReset
{
public:
    //! Default class constructor
    /*!
        \param name - Event name
        \param signaled - Signaled event initial state (default is false)
    */
    explicit NamedEventAutoReset(const std::string& name, bool signaled = false);
    NamedEventAutoReset(const NamedEventAutoReset&) = delete;
    NamedEventAutoReset(NamedEventAutoReset&&) = default;
    ~NamedEventAutoReset();

    NamedEventAutoReset& operator=(const NamedEventAutoReset&) = delete;
    NamedEventAutoReset& operator=(NamedEventAutoReset&&) = default;

    //! Get the event name
    const std::string& name() const { return _name; }

    //! Signal one of waiting thread about event occurred
    /*!
        If some threads are waiting for the event one will be chosen, signaled and continued.
        The order of thread signalization by auto-reset event is not guaranteed.

        Will not block.
    */
    void Signal();

    //! Try to wait the event without block
    /*!
        Will not block.

        \return 'true' if the event was occurred before and no other threads were signaled, 'false' if the event was not occurred before
    */
    bool TryWait();

    //! Try to wait the event for the given nanoseconds
    /*!
        Will block for the given nanoseconds in the worst case.

        \param nanoseconds - Nanoseconds to wait for the event
        \return 'true' if the event was occurred, 'false' if the event was not occurred
    */
    bool TryWaitFor(int64_t nanoseconds);
    //! Try to wait the event for the given time duration
    /*!
        Will block for the given time duration in the worst case.

        \param duration - Time duration to wait for the event
        \return 'true' if the event was occurred, 'false' if the event was not occurred
    */
    template <class Rep, class Period>
    bool TryWaitFor(const std::chrono::duration<Rep, Period>& duration)
    { return TryLockFor(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()); }
    //! Try to wait the event until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the event
        \return 'true' if the event was occurred, 'false' if the event was not occurred
    */
    template <class Clock, class Duration>
    bool TryWaitUntil(const std::chrono::time_point<Clock, Duration>& timestamp)
    { return TryLockFor(timestamp - std::chrono::high_resolution_clock::now()); }

    //! Try to wait the event with block
    /*!
        Will block.
    */
    void Wait();

private:
    class Impl;
    std::unique_ptr<Impl> _pimpl;
    std::string _name;
};

/*! \example threads_named_event_auto_reset.cpp Named auto-reset event synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_NAMED_EVENT_AUTO_RESET_H
