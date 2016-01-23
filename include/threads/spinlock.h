/*!
    \file spinlock.h
    \brief Spin-lock synchronization primitive definition
    \author Ivan Shynkarenka
    \date 22.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SPINLOCK_H
#define CPPCOMMON_SPINLOCK_H

#include <atomic>

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
    bool is_locked();

    //! Try to acquire spin-lock without block
    /*!
        Thread-safe. Will not block.

        \return 'true' if the spin-lock was successfully acquired, 'false' if the spin-lock is busy
    */
    bool try_lock();

    //! Try to acquire spin-lock for the given spin count without block
    /*!
        Thread-safe. Will not block.

        \param spin - spin count
        \return 'true' if the spin-lock was successfully acquired, 'false' if the spin-lock is busy
    */
    bool try_lock_for(uint64_t spin);

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

} // namespace CppCommon

#include "spinlock.inl"

#endif //CPPCOMMON_SPINLOCK_H
