/*!
    \file timespan.h
    \brief Time span definition
    \author Ivan Shynkarenka
    \date 11.07.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_TIME_TIMESPAN_H
#define CPPCOMMON_TIME_TIMESPAN_H

#include <chrono>

namespace CppCommon {

//! Time span
/*!
    Time span wraps time duration in nanoseconds or difference between two timestamps
    and allows to get separate values of days, hours, minutes, seconds, milliseconds,
    microseconds or nanoseconds.

    Not thread-safe.
*/
class TimeSpan
{
public:
    //! Initialize time span with a time duration value in nanoseconds
    /*!
        \param duration - Time duration value in nanoseconds
    */
    explicit TimeSpan(int64_t duration) noexcept
        : _duration(duration)
    {}
    //! Initialize time span with a time duration chrono value
    /*!
        \param duration - Time duration chrono value
    */
    template <class Rep, class Period>
    explicit TimeSpan(const std::chrono::duration<Rep, Period>& duration) noexcept
        : _duration(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count())
    {}
    TimeSpan(const TimeSpan&) noexcept = default;
    TimeSpan(TimeSpan&&) noexcept = default;
    ~TimeSpan() noexcept = default;

    TimeSpan& operator=(const TimeSpan&) noexcept = default;
    TimeSpan& operator=(TimeSpan&&) noexcept = default;

    //! Get total days in the current time span
    int64_t days() const noexcept { return _duration / (24 * 60 * 60 * 1000000000ll); }
    //! Get total hours in the current time span
    int64_t hours() const noexcept { return _duration / (60 * 60 * 1000000000ll); }
    //! Get total minutes in the current time span
    int64_t minutes() const noexcept { return _duration / (60 * 1000000000ll); }
    //! Get total seconds in the current time span
    int64_t seconds() const noexcept { return _duration / 1000000000ll; }
    //! Get total milliseconds in the current time span
    int64_t milliseconds() const noexcept { return _duration / 1000000; }
    //! Get total microseconds in the current time span
    int64_t microseconds() const noexcept { return _duration / 1000; }
    //! Get total nanoseconds in the current time span
    int64_t nanoseconds() const noexcept { return _duration; }

    //! Convert time span to the std::chrono nanoseconds duration
    std::chrono::duration<int64_t, std::nano> chrono() const
    { return std::chrono::nanoseconds(_duration); }

private:
    int64_t _duration;
};

/*! \example time_timespan.cpp Time span example */

} // namespace CppCommon

#endif // CPPCOMMON_TIME_TIMESPAN_H
