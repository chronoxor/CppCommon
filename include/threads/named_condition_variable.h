/*!
    \file named_condition_variable.h
    \brief Named condition variable synchronization primitive definition
    \author Ivan Shynkarenka
    \date 04.10.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_NAMED_CONDITION_VARIABLE_H
#define CPPCOMMON_THREADS_NAMED_CONDITION_VARIABLE_H

#include "time/timestamp.h"

#include <memory>
#include <string>

namespace CppCommon {

//! Named condition variable synchronization primitive
/*!
    Named condition variable behaves as a simple condition variable but could be shared
    between processes on the same machine.

    Thread-safe.
*/
class NamedConditionVariable
{
public:
    //! Default class constructor
    /*!
        \param name - Condition variable name
    */
    explicit NamedConditionVariable(const std::string& name);
    NamedConditionVariable(const NamedConditionVariable&) = delete;
    NamedConditionVariable(NamedConditionVariable&& cv) = delete;
    ~NamedConditionVariable();

    NamedConditionVariable& operator=(const NamedConditionVariable&) = delete;
    NamedConditionVariable& operator=(NamedConditionVariable&& cv) = delete;

    //! Get the condition variable name
    const std::string& name() const;

    //! Notify one of waiting thread about event occurred
    /*!
        Unblocks one of the threads currently waiting for this condition. If no
        threads are waiting, the function does nothing. If more than one, it is
        unspecified which of the threads is selected.

        Will not block.
    */
    void NotifyOne();
    //! Notify all waiting threads about event occurred
    /*!
        Unblocks all threads currently waiting for this condition. If no threads
        are waiting, the function does nothing.

        Will not block.
    */
    void NotifyAll();

    //! Wait until condition variable is notified
    /*!
        The execution of the current thread is blocked until notified.

        Will block.
    */
    void Wait();
    //! Wait until condition variable is notified using the given predicate
    /*!
        This method is equivalent to:
        \code{.cpp}
        while (!predicate()) Wait(cs);
        \endcode

        Will block.

        \param predicate - Predicate to check
    */
    template <typename TPredicate>
    void Wait(TPredicate predicate);

    //! Try to wait for the given timespan until condition variable is notified
    /*!
        The execution of the current thread is blocked during timespan, or until
        notified (if the latter happens first).

        Will block for the given timespan in the worst case.

        \param timespan - Timespan to wait for the condition variable notification
        \return 'true' if the condition variable was successfully notified, 'false' if the timeout was occurred
    */
    bool TryWaitFor(const Timespan& timespan);
    //! Try to wait for the given timespan until condition variable is notified using the given predicate
    /*!
        This method is equivalent to:
        \code{.cpp}
        Timestamp timeout = UtcTimestamp() + timespan;
        while (!predicate())
            if (!TryWaitFor(timeout - UtcTimestamp()))
                return predicate();
        return true;
        \endcode

        Will block for the given timespan in the worst case.

        \param timespan - Timespan to wait for the condition variable notification
        \param predicate - Predicate to check
        \return 'true' if the condition variable was successfully notified, 'false' if the timeout was occurred
    */
    template <typename TPredicate>
    bool TryWaitFor(const Timespan& timespan, TPredicate predicate);

    //! Try to wait until the given timestamp until condition variable is notified
    /*!
        The execution of the current thread is blocked either until notified or until
        timestamp, whichever happens first.

        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the condition variable notification
        \return 'true' if the condition variable was successfully notified, 'false' if the timeout was occurred
    */
    bool TryWaitUntil(const UtcTimestamp& timestamp)
    { return TryWaitFor(timestamp - UtcTimestamp()); }
    //! Try to wait until the given timestamp until condition variable is notified using the given predicate
    /*!
        This method is equivalent to:
        \code{.cpp}
        return TryWaitFor(timestamp - UtcTimestamp(), predicate);
        \endcode

        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the condition variable notification
        \param predicate - Predicate to check
        \return 'true' if the condition variable was successfully notified, 'false' if the timeout was occurred
    */
    template <typename TPredicate>
    bool TryWaitUntil(const UtcTimestamp& timestamp, TPredicate predicate)
    { return TryWaitFor(timestamp - UtcTimestamp(), predicate); }

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 168;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example threads_named_condition_variable.cpp Named condition variable synchronization primitive example */

} // namespace CppCommon

#include "named_condition_variable.inl"

#endif // CPPCOMMON_THREADS_NAMED_CONDITION_VARIABLE_H
