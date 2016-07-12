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
class Timespan
{
public:
    //! Initialize time span with a time duration value in nanoseconds
    /*!
        \param duration - Time duration value in nanoseconds
    */
    explicit Timespan(int64_t duration) noexcept : _duration(duration) {}
    //! Initialize time span with a time duration chrono value
    /*!
        \param duration - Time duration chrono value
    */
    template <class Rep, class Period>
    explicit Timespan(const std::chrono::duration<Rep, Period>& duration) noexcept
        : _duration(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count())
    {}
    Timespan(const Timespan&) noexcept = default;
    Timespan(Timespan&&) noexcept = default;
    ~Timespan() noexcept = default;

    Timespan& operator=(const Timespan&) noexcept = default;
    Timespan& operator=(Timespan&&) noexcept = default;

    Timespan operator+() const
    { return Timespan(+_duration); }
    Timespan operator-() const
    { return Timespan(-_duration); }

    Timespan& operator+=(int64_t offset) noexcept
    { _duration += offset; return *this; }
    Timespan& operator+=(const Timespan& offset) noexcept
    { _duration += offset.total(); return *this; }

    Timespan& operator-=(int64_t offset) noexcept
    { _duration -= offset; return *this; }
    Timespan& operator-=(const Timespan& offset) noexcept
    { _duration -= offset.total(); return *this; }

    friend Timespan operator+(const Timespan& timespan, int64_t offset) noexcept
    { return Timespan(timespan.total() + offset); }
    friend Timespan operator+(int64_t offset, const Timespan& timespan) noexcept
    { return Timespan(offset + timespan.total()); }
    friend Timespan operator+(const Timespan& timespan1, const Timespan& timespan2) noexcept
    { return Timespan(timespan1.total() + timespan2.total()); }

    friend Timespan operator-(const Timespan& timespan, int64_t offset) noexcept
    { return Timespan(timespan.total() - offset); }
    friend Timespan operator-(int64_t offset, const Timespan& timespan) noexcept
    { return Timespan(offset - timespan.total()); }
    friend Timespan operator-(const Timespan& timespan1, const Timespan& timespan2) noexcept
    { return Timespan(timespan1.total() - timespan2.total()); }

    friend bool operator==(const Timespan& timespan, int64_t offset) noexcept
    { return timespan.total() == offset; }
    friend bool operator==(int64_t offset, const Timespan& timespan) noexcept
    { return offset == timespan.total(); }
    friend bool operator==(const Timespan& timespan1, const Timespan& timespan2) noexcept
    { return timespan1.total() == timespan2.total(); }

    friend bool operator!=(const Timespan& timespan, int64_t offset) noexcept
    { return timespan.total() != offset; }
    friend bool operator!=(int64_t offset, const Timespan& timespan) noexcept
    { return offset != timespan.total(); }
    friend bool operator!=(const Timespan& timespan1, const Timespan& timespan2) noexcept
    { return timespan1.total() != timespan2.total(); }

    friend bool operator>(const Timespan& timespan, int64_t offset) noexcept
    { return timespan.total() > offset; }
    friend bool operator>(int64_t offset, const Timespan& timespan) noexcept
    { return offset > timespan.total(); }
    friend bool operator>(const Timespan& timespan1, const Timespan& timespan2) noexcept
    { return timespan1.total() > timespan2.total(); }

    friend bool operator<(const Timespan& timespan, int64_t offset) noexcept
    { return timespan.total() < offset; }
    friend bool operator<(int64_t offset, const Timespan& timespan) noexcept
    { return offset < timespan.total(); }
    friend bool operator<(const Timespan& timespan1, const Timespan& timespan2) noexcept
    { return timespan1.total() < timespan2.total(); }

    friend bool operator>=(const Timespan& timespan, int64_t offset) noexcept
    { return timespan.total() >= offset; }
    friend bool operator>=(int64_t offset, const Timespan& timespan) noexcept
    { return offset >= timespan.total(); }
    friend bool operator>=(const Timespan& timespan1, const Timespan& timespan2) noexcept
    { return timespan1.total() >= timespan2.total(); }

    friend bool operator<=(const Timespan& timespan, int64_t offset) noexcept
    { return timespan.total() <= offset; }
    friend bool operator<=(int64_t offset, const Timespan& timespan) noexcept
    { return offset <= timespan.total(); }
    friend bool operator<=(const Timespan& timespan1, const Timespan& timespan2) noexcept
    { return timespan1.total() <= timespan2.total(); }

    //! Convert time span to the std::chrono nanoseconds duration
    std::chrono::duration<int64_t, std::nano> chrono() const
    { return std::chrono::nanoseconds(_duration); }

    //! Get total days of the current time span
    int64_t days() const noexcept { return _duration / (24 * 60 * 60 * 1000000000ll); }
    //! Get total hours of the current time span
    int64_t hours() const noexcept { return _duration / (60 * 60 * 1000000000ll); }
    //! Get total minutes of the current time span
    int64_t minutes() const noexcept { return _duration / (60 * 1000000000ll); }
    //! Get total seconds of the current time span
    int64_t seconds() const noexcept { return _duration / 1000000000ll; }
    //! Get total milliseconds of the current time span
    int64_t milliseconds() const noexcept { return _duration / 1000000; }
    //! Get total microseconds of the current time span
    int64_t microseconds() const noexcept { return _duration / 1000; }
    //! Get total nanoseconds of the current time span
    int64_t nanoseconds() const noexcept { return _duration; }

    //! Get total value of the current time span (total nanoseconds)
    int64_t total() const noexcept { return _duration; }

private:
    int64_t _duration;
};

/*! \example time_timespan.cpp Time span example */

} // namespace CppCommon

#endif // CPPCOMMON_TIME_TIMESPAN_H
