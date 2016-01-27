/*!
    \file critical_section.inl
    \brief Critical section synchronization primitive inline implementation
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

namespace CppCommon {

inline bool CriticalSection::TryLockSpin(int64_t spin)
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

} // namespace CppCommon
