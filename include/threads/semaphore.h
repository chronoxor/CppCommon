/*!
    \file semaphore.h
    \brief Semaphore synchronization primitive definition
    \author Ivan Shynkarenka
    \date 12.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_SEMAPHORE_H
#define CPPCOMMON_THREADS_SEMAPHORE_H

#include "threads/locker.h"

#include <chrono>
#include <memory>

namespace CppCommon {

//! Semaphore synchronization primitive
/*!
    Semaphore synchronization primitive allows fixed count of threads to access some resource
    while other threads are waiting for it. When some thread unlocks the semaphore then one of
    waiting threads will lock it.

    Thread-safe.

    https://en.wikipedia.org/wiki/Semaphore_(programming)
*/
class Semaphore
{
public:
    //! Default class constructor
    /*!
        \param resources - Semaphore resources counter
    */
    explicit Semaphore(int resources);
    Semaphore(const Semaphore&) = delete;
    Semaphore(Semaphore&&) = default;
    ~Semaphore();

    Semaphore& operator=(const Semaphore&) = delete;
    Semaphore& operator=(Semaphore&&) = default;

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
    bool TryLockFor(const std::chrono::duration<Rep, Period>& duration);
    //! Try to acquire semaphore until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the semaphore
        \return 'true' if the semaphore was successfully acquired, 'false' if the semaphore is busy
    */
    template <class Clock, class Duration>
    bool TryLockUntil(const std::chrono::time_point<Clock, Duration>& timestamp);

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
    int _resources;
};

/*! \example threads_semaphore.cpp Semaphore synchronization primitive example */

} // namespace CppCommon

#include "semaphore.inl"

#endif // CPPCOMMON_THREADS_SEMAPHORE_H
