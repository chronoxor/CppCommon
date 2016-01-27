/*!
    \file spin_lock.h
    \brief Spin-lock synchronization primitive definition
    \author Ivan Shynkarenka
    \date 22.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SPIN_LOCK_H
#define CPPCOMMON_SPIN_LOCK_H

#include <atomic>
#include <chrono>

#include "system/timestamp.h"

namespace CppCommon {

//! Spin-lock synchronization primitive
/*!
    The purpose of a spin lock is to prevent multiple threads from concurrently accessing a shared data structure.
    In contrast to a mutex, threads will busy-wait and waste CPU cycles instead of yielding the CPU to another thread.
    Do not use spinlocks unless you are certain that you understand the consequences!

    https://en.wikipedia.org/wiki/Spinlock
*/
class SpinLock
{
public:
    SpinLock() : _lock(false) {}
    SpinLock(const SpinLock&) = delete;
    SpinLock(SpinLock&&) = default;
    ~SpinLock() = default;

    SpinLock& operator=(const SpinLock&) = delete;
    SpinLock& operator=(SpinLock&&) = default;

    //! Is already locked?
    /*!
        Thread-safe. Will not block.

        \return 'true' if the spin-lock is already locked, 'false' if the spin-lock is released
    */
    bool IsLocked();

    //! Try to acquire spin-lock without block
    /*!
        Thread-safe. Will not block.

        \return 'true' if the spin-lock was successfully acquired, 'false' if the spin-lock is busy
    */
    bool TryLock();

    //! Try to acquire spin-lock for the given spin count
    /*!
        Thread-safe. Will block for the given spin count in the worst case.

        \param spin - spin count
        \return 'true' if the spin-lock was successfully acquired, 'false' if the spin-lock is busy
    */
    bool TryLockSpin(int64_t spin);

    //! Try to acquire spin-lock for the given time duration
    /*!
        Thread-safe. Will block for the given time duration in the worst case.

        \param duration - time duration for spin-lock
        \return 'true' if the spin-lock was successfully acquired, 'false' if the spin-lock is busy
    */
    template <class Rep, class Period>
    bool TryLockFor(const std::chrono::duration<Rep, Period>& duration);

    //! Try to acquire spin-lock until the given timestamp
    /*!
        Thread-safe. Will block until the given timestamp in the worst case.

        \param timestamp - timestamp to stop spin-lock
        \return 'true' if the spin-lock was successfully acquired, 'false' if the spin-lock is busy
    */
    template <class Clock, class Duration>
    bool TryLockUntil(const std::chrono::time_point<Clock, Duration>& timestamp);

    //! Acquire spin-lock with block
    /*!
        Thread-safe. Will block.
    */
    void lock();

    //! Release spin-lock
    /*!
        Thread-safe. Will not block.
    */
    void unlock();

private:
    std::atomic<bool> _lock;
};

/*! \example threads_spin_lock.cpp Spin-lock synchronization primitive example */

} // namespace CppCommon

#include "spin_lock.inl"

#endif //CPPCOMMON_SPIN_LOCK_H
