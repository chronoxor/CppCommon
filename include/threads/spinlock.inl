/*!
    \file spinlock.inl
    \brief Spin-lock synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 22.01.2016
    \copyright MIT License
*/

namespace CppCommon {

inline bool SpinLock::is_locked()
{
    return _lock.load(std::memory_order_acquire);
}

inline bool SpinLock::try_lock()
{
    return !_lock.exchange(true, std::memory_order_acquire);
}

inline bool SpinLock::try_lock_spin(int64_t spin)
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
inline bool SpinLock::try_lock_for(const std::chrono::duration<Rep, Period>& duration)
{
    int64_t nanoduration = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    uint64_t start = timestamp();

    // Try to acquire spin-lock at least one time
    do 
    {
        if (!_lock.exchange(true, std::memory_order_acquire))
            return true;
    } while ((int64_t)(timestamp() - start) < nanoduration);

    // Failed to acquire spin-lock
    return false;
}

template <class Clock, class Duration>
inline bool SpinLock::try_lock_until(const std::chrono::time_point<Clock, Duration>& timestamp)
{
    return try_lock_for(timestamp - std::chrono::high_resolution_clock::now());
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
