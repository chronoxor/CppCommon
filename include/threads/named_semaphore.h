/*!
    \file named_semaphore.h
    \brief Named semaphore synchronization primitive definition
    \author Ivan Shynkarenka
    \date 15.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_NAMED_SEMAPHORE_H
#define CPPCOMMON_THREADS_NAMED_SEMAPHORE_H

#include "threads/locker.h"

#include <chrono>
#include <memory>
#include <string>

namespace CppCommon {

//! Named semaphore synchronization primitive
/*!
    Named semaphore behaves as a simple one but could be shared between processes
    on the same machine.

    Thread-safe.

    \see Semaphore
*/
class NamedSemaphore
{
public:
    //! Default class constructor
    /*!
        \param name - Semaphore name
        \param resources - Semaphore resources counter
    */
    explicit NamedSemaphore(const std::string& name, int resources);
    NamedSemaphore(const NamedSemaphore&) = delete;
    NamedSemaphore(NamedSemaphore&&) = default;
    ~NamedSemaphore();

    NamedSemaphore& operator=(const NamedSemaphore&) = delete;
    NamedSemaphore& operator=(NamedSemaphore&&) = default;

    //! Get the semaphore name
    const std::string& name() const { return _name; }
    //! Get the semaphore resources counter
    int resources() const noexcept { return _resources; }

    //! Try to acquire semaphore without block
    /*!
        Will not block.

        \return 'true' if the semaphore was successfully acquired, 'false' if the semaphore is busy
    */
    bool TryLock();

    //! Try to acquire semaphore for the given nanoseconds
    /*!
        Will block for the given nanoseconds in the worst case.

        \param nanoseconds - Nanoseconds to wait for the semaphore
        \return 'true' if the semaphore was successfully acquired, 'false' if the semaphore is busy
    */
    bool TryLockFor(int64_t nanoseconds);
    //! Try to acquire semaphore for the given time duration
    /*!
        Will block for the given time duration in the worst case.

        \param duration - Time duration to wait for the semaphore
        \return 'true' if the semaphore was successfully acquired, 'false' if the semaphore is busy
    */
    template <class Rep, class Period>
    bool TryLockFor(const std::chrono::duration<Rep, Period>& duration)
    { return TryLockFor(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()); }
    //! Try to acquire semaphore until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the semaphore
        \return 'true' if the semaphore was successfully acquired, 'false' if the semaphore is busy
    */
    template <class Clock, class Duration>
    bool TryLockUntil(const std::chrono::time_point<Clock, Duration>& timestamp)
    { return TryLockFor(timestamp - std::chrono::high_resolution_clock::now()); }

    //! Acquire semaphore with block
    /*!
        Will block.
    */
    void Lock();

    //! Release semaphore
    /*!
        Will not block.
    */
    void Unlock();

private:
    class Impl;
    std::unique_ptr<Impl> _pimpl;
    std::string _name;
    int _resources;
};

/*! \example threads_named_semaphore.cpp Named semaphore synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_NAMED_SEMAPHORE_H
