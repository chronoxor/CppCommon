/*!
    \file condition_variable.h
    \brief Condition variable synchronization primitive definition
    \author Ivan Shynkarenka
    \date 03.10.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_CONDITION_VARIABLE_H
#define CPPCOMMON_THREADS_CONDITION_VARIABLE_H

#include "threads/critical_section.h"

namespace CppCommon {

//! Condition variable synchronization primitive
/*!
    Condition variable is a synchronization primitive that enable threads to wait
    until a particular condition occurs. Condition variables are user-mode objects
    that cannot be shared across processes.

    Condition variables enable threads to atomically release a lock and enter the
    sleeping state. They can be used with critical sections. Condition variables
    support operations that "notify one" or "notify all" waiting threads. After
    a thread is woken, it re-acquires the lock it released when the thread entered
    the sleeping state.

    Thread-safe.

    https://en.wikipedia.org/wiki/Monitor_(synchronization)
*/
class ConditionVariable
{
public:
    ConditionVariable();
    ConditionVariable(const ConditionVariable&) = delete;
    ConditionVariable(ConditionVariable&& cv) = delete;
    ~ConditionVariable();

    ConditionVariable& operator=(const ConditionVariable&) = delete;
    ConditionVariable& operator=(ConditionVariable&& cv) = delete;

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
        The execution of the current thread (which shall have locked critical
        section) is blocked until notified.

        Will block.

        \param cs - Critical section (must be locked)
    */
    void Wait(CriticalSection& cs);
    //! Wait until condition variable is notified using the given predicate
    /*!
        This method is equivalent to:
        \code{.cpp}
        while (!predicate()) Wait(cs);
        \endcode

        Will block.

        \param cs - Critical section (must be locked)
        \param predicate - Predicate to check
    */
    template <typename TPredicate>
    void Wait(CriticalSection& cs, TPredicate predicate);

    //! Try to wait for the given timespan until condition variable is notified
    /*!
        The execution of the current thread (which shall have locked critical
        section) is blocked during timespan, or until notified (if the latter
        happens first).

        Will block for the given timespan in the worst case.

        \param cs - Critical section (must be locked)
        \param timespan - Timespan to wait for the condition variable notification
        \return 'true' if the condition variable was successfully notified, 'false' if the timeout was occurred
    */
    bool TryWaitFor(CriticalSection& cs, const Timespan& timespan);
    //! Try to wait for the given timespan until condition variable is notified using the given predicate
    /*!
        This method is equivalent to:
        \code{.cpp}
        Timestamp timeout = UtcTimestamp() + timespan;
        while (!predicate())
            if (!TryWaitFor(cs, timeout - UtcTimestamp()))
                return predicate();
        return true;
        \endcode

        Will block for the given timespan in the worst case.

        \param cs - Critical section (must be locked)
        \param timespan - Timespan to wait for the condition variable notification
        \param predicate - Predicate to check
        \return 'true' if the condition variable was successfully notified, 'false' if the timeout was occurred
    */
    template <typename TPredicate>
    bool TryWaitFor(CriticalSection& cs, const Timespan& timespan, TPredicate predicate);

    //! Try to wait until the given timestamp until condition variable is notified
    /*!
        The execution of the current thread (which shall have locked critical section)
        is blocked either until notified or until timestamp, whichever happens first.

        Will block until the given timestamp in the worst case.

        \param cs - Critical section (must be locked)
        \param timestamp - Timestamp to stop wait for the condition variable notification
        \return 'true' if the condition variable was successfully notified, 'false' if the timeout was occurred
    */
    bool TryWaitUntil(CriticalSection& cs, const UtcTimestamp& timestamp)
    { return TryWaitFor(cs, timestamp - UtcTimestamp()); }
    //! Try to wait until the given timestamp until condition variable is notified using the given predicate
    /*!
        This method is equivalent to:
        \code{.cpp}
        return TryWaitFor(cs, timestamp - UtcTimestamp(), predicate);
        \endcode

        Will block until the given timestamp in the worst case.

        \param cs - Critical section (must be locked)
        \param timestamp - Timestamp to stop wait for the condition variable notification
        \param predicate - Predicate to check
        \return 'true' if the condition variable was successfully notified, 'false' if the timeout was occurred
    */
    template <typename TPredicate>
    bool TryWaitUntil(CriticalSection& cs, const UtcTimestamp& timestamp, TPredicate predicate)
    { return TryWaitFor(cs, timestamp - UtcTimestamp(), predicate); }

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 48;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example threads_condition_variable.cpp Condition variable synchronization primitive example */

} // namespace CppCommon

#include "condition_variable.inl"

#endif // CPPCOMMON_THREADS_CONDITION_VARIABLE_H
