/*!
    \file event_auto_reset.h
    \brief Auto-reset event synchronization primitive definition
    \author Ivan Shynkarenka
    \date 13.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_EVENT_AUTO_RESET_H
#define CPPCOMMON_THREADS_EVENT_AUTO_RESET_H

#include <chrono>
#include <memory>

namespace CppCommon {

//! Auto-reset event synchronization primitive
/*!
    Auto-reset event synchronization primitive allows multiple threads to wait for some event occurred
    and signal only one thread at the time. Other thread will wait for the next event signalization.
    The order of thread signalization by auto-reset event is not guaranteed.

    Thread-safe.

    https://en.wikipedia.org/wiki/Event_(synchronization_primitive)
*/
class EventAutoReset
{
public:
    //! Default class constructor
    /*!
        \param signaled - Signaled event initial state (default is false)
    */
    explicit EventAutoReset(bool signaled = false);
    EventAutoReset(const EventAutoReset&) = delete;
    EventAutoReset(EventAutoReset&&) = default;
    ~EventAutoReset();

    EventAutoReset& operator=(const EventAutoReset&) = delete;
    EventAutoReset& operator=(EventAutoReset&&) = default;

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
    bool TryWaitFor(const std::chrono::duration<Rep, Period>& duration);
    //! Try to wait the event until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the event
        \return 'true' if the event was occurred, 'false' if the event was not occurred
    */
    template <class Clock, class Duration>
    bool TryWaitUntil(const std::chrono::time_point<Clock, Duration>& timestamp);

    //! Try to wait the event with block
    /*!
        Will block.
    */
    void Wait();

private:
    class Impl;
    std::unique_ptr<Impl> _pimpl;
};

/*! \example threads_event_auto_reset.cpp Auto-reset event synchronization primitive example */

} // namespace CppCommon

#include "event_auto_reset.inl"

#endif // CPPCOMMON_THREADS_EVENT_AUTO_RESET_H
