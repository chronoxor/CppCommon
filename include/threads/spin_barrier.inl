/*!
    \file spin_barrier.inl
    \brief Spin barrier synchronization primitive inline implementation
    \author Ivan Shynkarenka
    \date 16.03.2016
    \copyright MIT License
*/

namespace CppCommon {

inline SpinBarrier::SpinBarrier(int threads) noexcept : _counter(threads), _generation(0), _threads(threads)
{
    assert((threads > 0) && "Count of barrier threads must be greater than zero!");
}

inline bool SpinBarrier::Wait() noexcept
{
    // Remember the current barrier generation
    int generation = _generation;

    // Decrease the count of waiting threads
    if (--_counter == 0)
    {
        // Increase the current barrier generation
        ++_generation;

        // Reset waiting threads counter
        _counter = _threads;

        // Notify the last thread that reached the barrier
        return true;
    }
    else
    {
        // Spin-wait for the next barrier generation
        while ((generation == _generation) || (_counter == 0));

        // Notify each of remaining threads
        return false;
    }
}

} // namespace CppCommon
