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

inline bool SpinLock::try_lock_spin(uint64_t spin)
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

inline void SpinLock::lock()
{
    while (_lock.exchange(true, std::memory_order_acquire));
}

inline void SpinLock::unlock()
{
    _lock.store(false, std::memory_order_release);
}

} // namespace CppCommon
