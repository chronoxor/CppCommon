/*!
    \file spin_lock.inl
    \brief Spin-lock synchronization primitive inline implementation
    \author Ivan Shynkarenka
    \date 22.01.2016
    \copyright MIT License
*/

namespace CppCommon {

inline bool SpinLock::IsLocked() noexcept
{
    return _lock.load(std::memory_order_acquire);
}

inline bool SpinLock::TryLock() noexcept
{
    return !_lock.exchange(true, std::memory_order_acquire);
}

inline bool SpinLock::TryLockSpin(int64_t spin) noexcept
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

inline bool SpinLock::TryLockFor(int64_t nanoseconds) noexcept
{
    uint64_t finish = timestamp() + nanoseconds;

    // Try to acquire spin-lock at least one time
    do
    {
        if (TryLock())
            return true;
    } while (timestamp() < finish);

    // Failed to acquire spin-lock
    return false;
}

template <class Rep, class Period>
inline bool SpinLock::TryLockFor(const std::chrono::duration<Rep, Period>& duration) noexcept
{
    return TryLockFor(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
}

template <class Clock, class Duration>
inline bool SpinLock::TryLockUntil(const std::chrono::time_point<Clock, Duration>& timestamp) noexcept
{
    return TryLockFor(timestamp - std::chrono::high_resolution_clock::now());
}

inline void SpinLock::Lock() noexcept
{
    while (_lock.exchange(true, std::memory_order_acquire));
}

inline void SpinLock::Unlock() noexcept
{
    _lock.store(false, std::memory_order_release);
}

} // namespace CppCommon
