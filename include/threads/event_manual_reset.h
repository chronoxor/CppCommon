/*!
    \file event_manual_reset.h
    \brief Manual-reset event synchronization primitive definition
    \author Ivan Shynkarenka
    \date 13.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_EVENT_MANUAL_RESET_H
#define CPPCOMMON_THREADS_EVENT_MANUAL_RESET_H

#include "time/timestamp.h"

#include <memory>

namespace CppCommon {

//! Manual-reset event synchronization primitive
/*!
    Manual-reset event synchronization primitive allows multiple threads to wait for some event occurred
    and signal all waiting threads at the time. If the event is in the signaled state no thread will wait
    for it until the event is reset.

    Thread-safe.

    https://en.wikipedia.org/wiki/Event_(synchronization_primitive)
*/
class EventManualReset
{
public:
    //! Default class constructor
    /*!
        \param signaled - Signaled event initial state (default is false)
    */
    explicit EventManualReset(bool signaled = false);
    EventManualReset(const EventManualReset&) = delete;
    EventManualReset(EventManualReset&& event) = delete;
    ~EventManualReset();

    EventManualReset& operator=(const EventManualReset&) = delete;
    EventManualReset& operator=(EventManualReset&& event) = delete;

    //! Reset the event
    /*!
        If the event is in the signaled state then it will be reset to non signaled state.
        As the result other threads that wait for the event will be blocked.

        Will not block.
    */
    void Reset();

    //! Signal one of waiting thread about event occurred
    /*!
        If some threads are waiting for the event all of them will be signaled and continued.

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

/*! \example threads_event_manual_reset.cpp Manual-reset event synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_EVENT_MANUAL_RESET_H
