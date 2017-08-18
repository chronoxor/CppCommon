/*!
    \file timespan.inl
    \brief Timespan inline implementation
    \author Ivan Shynkarenka
    \date 11.07.2016
    \copyright MIT License
*/

namespace CppCommon {

inline void Timespan::swap(Timespan& timespan) noexcept
{
    using std::swap;
    swap(_duration, timespan._duration);
}

inline void swap(Timespan& timespan1, Timespan& timespan2) noexcept
{
    timespan1.swap(timespan2);
}

} // namespace CppCommon
