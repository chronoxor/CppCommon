/*!
    \file latch.h
    \brief Latch synchronization primitive definition
    \author Ivan Shynkarenka
    \date 25.03.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_LATCH_H
#define CPPCOMMON_THREADS_LATCH_H

#include "time/timestamp.h"

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
        \param threads - Latch threads counter initial value
    */
    explicit Latch(int threads) noexcept;
    Latch(const Latch&) = delete;
    Latch(Latch&&) = delete;
    ~Latch() = default;

    Latch& operator=(const Latch&) = delete;
    Latch& operator=(Latch&&) = delete;

    //! Get the count of threads to wait for the latch
    int threads() const noexcept { return _threads; }

    //! Reset the latch with a new threads counter value
    /*!
        This method may only be invoked when there are no other threads currently
        inside the waiting for the latch.

        Will not block.

        \param threads - Latch threads counter value
    */
    void Reset(int threads) noexcept;

    //! Countdown the latch
    /*!
        Decrements the latch counter by 1, and returns. If the latch counter reaches 0,
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

    //! Try to wait for the latch for the given timespan
    /*!
        Will block for the given timespan in the worst case.

        \param timespan - Timepan to wait for the latch
        \return 'true' if the latch counter is zero, 'false' if the latch counter is not zero
    */
    bool TryWaitFor(const Timespan& timespan) noexcept;
    //! Try to wait for the latch until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the latch
        \return 'true' if the latch counter is zero, 'false' if the latch counter is not zero
    */
    bool TryWaitUntil(const Timestamp& timestamp) noexcept;

private:
    std::mutex _mutex;
    std::condition_variable _cond;
    int _generation;
    int _threads;

    bool CountDown(std::unique_lock<std::mutex>& lock) noexcept;
};

/*! \example threads_latch_multi.cpp Latch synchronization primitive example for multiple threads waiting */
/*! \example threads_latch_single.cpp Latch synchronization primitive example for single thread waiting */

} // namespace CppCommon

#include "latch.inl"

#endif // CPPCOMMON_THREADS_LATCH_H
