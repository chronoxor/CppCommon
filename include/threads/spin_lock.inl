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

inline bool SpinLock::TryLockFor(const Timespan& timespan) noexcept
{
    // Calculate a finish timestamp
    Timestamp finish = NanoTimestamp() + timespan;

    // Try to acquire spin-lock at least one time
    do
    {
        if (TryLock())
            return true;
    } while (NanoTimestamp() < finish);

    // Failed to acquire spin-lock
    return false;
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
