/*!
    \file event_auto_reset.h
    \brief Auto-reset event synchronization primitive definition
    \author Ivan Shynkarenka
    \date 13.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_EVENT_AUTO_RESET_H
#define CPPCOMMON_THREADS_EVENT_AUTO_RESET_H

#include "time/timestamp.h"

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
    EventAutoReset(EventAutoReset&& event) = delete;
    ~EventAutoReset();

    EventAutoReset& operator=(const EventAutoReset&) = delete;
    EventAutoReset& operator=(EventAutoReset&& event) = delete;

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

    //! Try to wait the event for the given timespan
    /*!
        Will block for the given timespan in the worst case.

        \param timespan - Timespan to wait for the event
        \return 'true' if the event was occurred, 'false' if the event was not occurred
    */
    bool TryWaitFor(const Timespan& timespan);
    //! Try to wait the event until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the event
        \return 'true' if the event was occurred, 'false' if the event was not occurred
    */
    bool TryWaitUntil(const UtcTimestamp& timestamp)
    { return TryWaitFor(timestamp - UtcTimestamp()); }

    //! Try to wait the event with block
    /*!
        Will block.
    */
    void Wait();

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 128;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example threads_event_auto_reset.cpp Auto-reset event synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_EVENT_AUTO_RESET_H
