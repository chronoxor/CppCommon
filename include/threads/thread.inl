/*!
    \file thread.inl
    \brief Thread abstraction inline implementation
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template <class Rep, class Period>
inline void Thread::SleepFor(const std::chrono::duration<Rep, Period>& duration) noexcept
{
    Sleep(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
}

template <class Clock, class Duration>
inline void Thread::SleepUntil(const std::chrono::time_point<Clock, Duration>& timestamp) noexcept
{
    SleepFor(timestamp - std::chrono::high_resolution_clock::now());
}

} // namespace CppCommon
