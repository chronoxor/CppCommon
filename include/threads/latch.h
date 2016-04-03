/*!
    \file latch.h
    \brief Latch synchronization primitive definition
    \author Ivan Shynkarenka
    \date 25.03.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_LATCH_H
#define CPPCOMMON_THREADS_LATCH_H

#include <cassert>
#include <condition_variable>
#include <memory>
#include <mutex>

namespace CppCommon {

//! Latch synchronization primitive
/*!
    Latches are a thread co-ordination mechanism that allow one or more threads to block
    until one or more threads have reached a point.

    Thread-safe.
*/
class Latch
{
public:
    //! Default class constructor
    /*!
        \param counter - Latch counter initial value
    */
    explicit Latch(int counter) noexcept;
    Latch(const Latch&) = delete;
    Latch(Latch&&) = default;
    ~Latch() = default;

    Latch& operator=(const Latch&) = delete;
    Latch& operator=(Latch&&) = default;

    //! Reset the latch with a new counter value
    /*!
        This method may only be invoked when there are no other threads currently
        inside the waiting for the latch.

        Will not block.

        \param counter - Latch counter value
    */
    void Reset(int counter) noexcept;

    //! Countdown the latch
    /*!
        Decrements the latch counter by 1, and returns. If the latch count reaches 0,
        any threads blocked in Wait() will be released.

        Will not block.
    */
    void CountDown() noexcept;

    //! Countdown the latch
    /*!
        Decrements the latch counter by 1. If the latch counter is not 0, blocks the calling
        thread until the latch counter is decremented to 0 by one or more other threads calling
        CountDown() or CountDownAndWait().

        Will block.
    */
    void CountDownAndWait() noexcept;

    //! Wait for the latch
    /*!
        Method will block the current thread until the latch counter is zero.

        Will block.
    */
    void Wait() noexcept;

    //! Try to wait for the latch without block
    /*!
        Will not block.

        \return 'true' if the latch counter is zero, 'false' if the latch counter is not zero
    */
    bool TryWait() noexcept;

    //! Try to wait for the latch for the given time duration
    /*!
        Will block for the given time duration in the worst case.

        \param duration - Time duration to wait for the latch
        \return 'true' if the latch counter is zero, 'false' if the latch counter is not zero
    */
    template <class Rep, class Period>
    bool TryWaitFor(const std::chrono::duration<Rep, Period>& duration) noexcept;

    //! Try to wait for the latch until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the latch
        \return 'true' if the latch counter is zero, 'false' if the latch counter is not zero
    */
    template <class Clock, class Duration>
    bool TryWaitUntil(const std::chrono::time_point<Clock, Duration>& timestamp) noexcept;

private:
    std::mutex _mutex;
    std::condition_variable _cond;
    int _counter;
    int _generation;
    int _threads;

    bool CountDown(std::unique_lock<std::mutex>& lock) noexcept;
};

/*! \example threads_latch_multi.cpp Latch synchronization primitive example for multiple threads waiting */
/*! \example threads_latch_single.cpp Latch synchronization primitive example for single thread waiting */

} // namespace CppCommon

#include "latch.inl"

#endif // CPPCOMMON_THREADS_LATCH_H
