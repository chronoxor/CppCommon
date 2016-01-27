/*!
    \file spinlock.inl
    \brief Spin-lock synchronization primitive inline implementation
    \author Ivan Shynkarenka
    \date 22.01.2016
    \copyright MIT License
*/

namespace CppCommon {

inline bool SpinLock::IsLocked()
{
    return _lock.load(std::memory_order_acquire);
}

inline bool SpinLock::TryLock()
{
    return !_lock.exchange(true, std::memory_order_acquire);
}

inline bool SpinLock::TryLockSpin(int64_t spin)
{
    // Try to acquire spin-lock at least one time
    do
    {
        if (!_lock.exchange(true, std::memory_order_acquire))
            return true;
    } while (spin-- > 0);

    // Failed to acquire spin-lock
    return false;
}

template <class Rep, class Period>
inline bool SpinLock::TryLockFor(const std::chrono::duration<Rep, Period>& duration)
{
    uint64_t finish = timestamp() + std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

    // Try to acquire spin-lock at least one time
    do
    {
        if (!_lock.exchange(true, std::memory_order_acquire))
            return true;
    } while (timestamp() < finish);

    // Failed to acquire spin-lock
    return false;
}

template <class Clock, class Duration>
inline bool SpinLock::TryLockUntil(const std::chrono::time_point<Clock, Duration>& timestamp)
{
    return TryLockFor(timestamp - std::chrono::high_resolution_clock::now());
}

inline void SpinLock::lock()
{
    while (_lock.exchange(true, std::memory_order_acquire));
}

inline void SpinLock::unlock()
{
    _lock.store(false, std::memory_order_release);
}

} // namespace CppCommon
