/*!
    \file condition_variable.inl
    \brief Condition variable synchronization primitive inline implementation
    \author Ivan Shynkarenka
    \date 03.10.2016
    \copyright MIT License
*/

namespace CppCommon {

template <typename TPredicate>
void ConditionVariable::Wait(CriticalSection& cs, TPredicate predicate)
{
    while (!predicate())
        Wait(cs);
}

template <typename TPredicate>
bool ConditionVariable::TryWaitFor(CriticalSection& cs, const Timespan& timespan, TPredicate predicate)
{
    Timestamp timeout = UtcTimestamp() + timespan;
    while (!predicate())
        if (!TryWaitFor(cs, timeout - UtcTimestamp()))
            return predicate();
    return true;
}

} // namespace CppCommon
