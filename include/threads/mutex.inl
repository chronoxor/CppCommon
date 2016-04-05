/*!
    \file mutex.inl
    \brief Mutex synchronization primitive inline implementation
    \author Ivan Shynkarenka
    \date 04.04.2016
    \copyright MIT License
*/

namespace CppCommon {

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
