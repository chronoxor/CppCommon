/*!
    \file event_auto.inl
    \brief Auto-reset event synchronization primitive inline implementation
    \author Ivan Shynkarenka
    \date 14.04.2016
    \copyright MIT License
*/

namespace CppCommon {

template <class Rep, class Period>
inline bool EventAuto::TryLockFor(const std::chrono::duration<Rep, Period>& duration)
{
    return TryLockFor(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
}

template <class Clock, class Duration>
inline bool EventAuto::TryLockUntil(const std::chrono::time_point<Clock, Duration>& timestamp)
{
    return TryLockFor(timestamp - std::chrono::high_resolution_clock::now());
}

} // namespace CppCommon
