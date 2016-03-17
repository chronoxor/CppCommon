/*!
    \file spin_barrier.inl
    \brief Spin barrier synchronization primitive inline implementation
    \author Ivan Shynkarenka
    \date 16.03.2016
    \copyright MIT License
*/

namespace CppCommon {

inline SpinBarrier::SpinBarrier(int threads) noexcept : _threads(threads), _waiting(threads), _generation(0)
{
    assert((threads > 0) && "Count of barrier threads must be greater than zero!");
}

inline bool SpinBarrier::wait() noexcept
{
    // Remember the current barrier generation
    int generation = _generation;

    // Decrease the count of waiting threads
    if (--_waiting == 0)
    {
        // Reset waiting threads counter
        _waiting = _threads;
        // Increase the current barrier generation
        ++_generation;

        // Notify last thread that reach barrier
        return true;
    }
    else
    {
        // Spin-wait for the next barrier generation
        while (generation == _generation);

        // Notify each of the remaining threads
        return false;
    }
}

} // namespace CppCommon
