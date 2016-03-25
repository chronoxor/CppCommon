/*!
    \file latch.cpp
    \brief Latch synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 25.03.2016
    \copyright MIT License
*/

#include "threads/latch.h"

namespace CppCommon {

Latch::Latch(int counter) noexcept : _counter(counter), _generation(0)
{
    assert((counter > 0) && "Latch counter must be greater than zero!");
}

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
        // Increase the latch generation
        ++_generation;
        // Notify all waiting threads
        _cond.notify_all();
        return true;
    }
    else
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

    // Save the current generation value
    int generation = _generation;

    // Count down the latch counter
    if (CountDown(lock))
        return;

    // Wait for the next generation
    _cond.wait(lock, [=, this]() { return generation != _generation; });
}

void Latch::Wait() noexcept
{
    std::unique_lock<std::mutex> lock(_mutex);

    // Check the latch counter value
    if (_counter == 0)
        return;

    // Save the current generation value
    int generation = _generation;

    // Wait for the next generation
    _cond.wait(lock, [=, this]() { return generation != _generation; });
}

bool Latch::TryWait() noexcept
{
    std::unique_lock<std::mutex> lock(_mutex);

    // Check the latch counter value
    return (_counter == 0);
}

} // namespace CppCommon
