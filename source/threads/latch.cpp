/*!
    \file latch.cpp
    \brief Latch synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 25.03.2016
    \copyright MIT License
*/

#include "threads/latch.h"

namespace CppCommon {

void Latch::Reset(int counter) noexcept
{
    assert((counter > 0) && "Latch counter must be greater than zero!");

    std::lock_guard<std::mutex> lock(_mutex);
    _counter = counter;
}

bool Latch::CountDown(std::unique_lock<std::mutex>& lock) noexcept
{
    // Count down the latch counter and check its value
    if (--_counter == 0)
    {
        // Increase the current latch generation
        ++_generation;

        // Reset the current latch counter
        _counter = _threads;

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

    // Count down the latch counter
    CountDown(lock);
}

void Latch::CountDownAndWait() noexcept
{
    std::unique_lock<std::mutex> lock(_mutex);

    // Remember the current latch generation
    int generation = _generation;

    // Count down the latch counter
    if (CountDown(lock))
        return;

    // Wait for the next latch generation
    _cond.wait(lock, [=, this]() { return generation != _generation; });
}

void Latch::Wait() noexcept
{
    std::unique_lock<std::mutex> lock(_mutex);

    // Check the latch counter value
    if (_counter == 0)
        return;

    // Remember the current latch generation
    int generation = _generation;

    // Wait for the next latch generation
    _cond.wait(lock, [=, this]() { return generation != _generation; });
}

bool Latch::TryWait() noexcept
{
    std::unique_lock<std::mutex> lock(_mutex);

    // Check the latch counter value
    return (_counter == 0);
}

} // namespace CppCommon
