/*!
    \file mutex.inl
    \brief Mutex synchronization primitive inline implementation
    \author Ivan Shynkarenka
    \date 04.04.2016
    \copyright MIT License
*/

namespace CppCommon {

inline bool Mutex::TryLockSpin(int64_t spin)
{
    // Try to acquire spin-lock at least one time
    do
    {
        if (TryLock())
            return true;
    } while (spin-- > 0);

    // Failed to acquire spin-lock
    return false;
}

template <class Rep, class Period>
inline bool Mutex::TryLockFor(const std::chrono::duration<Rep, Period>& duration)
{
    return TryLockFor(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
}

template <class Clock, class Duration>
inline bool Mutex::TryLockUntil(const std::chrono::time_point<Clock, Duration>& timestamp)
{
    return TryLockFor(timestamp - std::chrono::high_resolution_clock::now());
}

} // namespace CppCommon
