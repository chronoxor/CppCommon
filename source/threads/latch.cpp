/*!
    \file latch.cpp
    \brief Latch synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 25.03.2016
    \copyright MIT License
*/

#include "threads/latch.h"

namespace CppCommon {

void Latch::Reset(int threads) noexcept
{
    assert((threads > 0) && "Latch threads counter must be greater than zero!");

    std::scoped_lock lock(_mutex);

    // Reset the latch threads counter with a new value
    _threads = threads;
}

bool Latch::CountDown(std::unique_lock<std::mutex>& lock) noexcept
{
    // Count down the latch threads counter and check its value
    if (--_threads == 0)
    {
        // Increase the current latch generation
        ++_generation;

        // Notify all waiting threads
        _cond.notify_all();

        // Wait for the next latch generation
        return true;
    }

    // Notify each of remaining threads
    return false;
}

void Latch::CountDown() noexcept
{
    std::unique_lock<std::mutex> lock(_mutex);

    // Count down the latch threads counter
    CountDown(lock);
}

void Latch::CountDownAndWait() noexcept
{
    std::unique_lock<std::mutex> lock(_mutex);

    // Remember the current latch generation
    int generation = _generation;

    // Count down the latch threads counter
    if (CountDown(lock))
        return;

    // Wait for the next latch generation
    _cond.wait(lock, [&, this]() { return generation != _generation; });
}

void Latch::Wait() noexcept
{
    std::unique_lock<std::mutex> lock(_mutex);

    // Check the latch threads counter value
    if (_threads == 0)
        return;

    // Remember the current latch generation
    int generation = _generation;

    // Wait for the next latch generation
    _cond.wait(lock, [&, this]() { return generation != _generation; });
}

bool Latch::TryWait() noexcept
{
    std::unique_lock<std::mutex> lock(_mutex);

    // Check the latch threads counter value
    return (_threads == 0);
}

} // namespace CppCommon
