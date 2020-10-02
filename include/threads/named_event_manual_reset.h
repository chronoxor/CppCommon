/*!
    \file named_event_manual_reset.h
    \brief Named manual-reset event synchronization primitive definition
    \author Ivan Shynkarenka
    \date 24.05.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_NAMED_EVENT_MANUAL_RESET_H
#define CPPCOMMON_THREADS_NAMED_EVENT_MANUAL_RESET_H

#include "time/timestamp.h"

#include <memory>
#include <string>

namespace CppCommon {

//! Named manual-reset event synchronization primitive
/*!
    Named manual-reset event behaves as a simple manual-reset event but could be shared
    between processes on the same machine.

    Thread-safe.

    \see EventManualReset
*/
class NamedEventManualReset
{
public:
    //! Default class constructor
    /*!
        \param name - Event name
        \param signaled - Signaled event initial state (default is false)
    */
    explicit NamedEventManualReset(const std::string& name, bool signaled = false);
    NamedEventManualReset(const NamedEventManualReset&) = delete;
    NamedEventManualReset(NamedEventManualReset&& event) = delete;
    ~NamedEventManualReset();

    NamedEventManualReset& operator=(const NamedEventManualReset&) = delete;
    NamedEventManualReset& operator=(NamedEventManualReset&& event) = delete;

    //! Get the event name
    const std::string& name() const;

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

    static const size_t StorageSize = 136;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example threads_named_event_manual_reset.cpp Named manual-reset event synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_NAMED_EVENT_MANUAL_RESET_H
