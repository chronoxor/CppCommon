/*!
    \file timespan.h
    \brief Timespan definition
    \author Ivan Shynkarenka
    \date 11.07.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_TIME_TIMESPAN_H
#define CPPCOMMON_TIME_TIMESPAN_H

#include <chrono>

#include "errors/exceptions.h"

namespace CppCommon {

//! Timespan
/*!
    Timespan wraps time duration in nanoseconds or difference between two timestamps
    and allows to get separate values of days, hours, minutes, seconds, milliseconds,
    microseconds or nanoseconds.

    Not thread-safe.
*/
class Timespan
{
public:
    //! Initialize timespan with a zero time duration
    Timespan() noexcept : _duration(0) {}
    //! Initialize timespan with a given time duration value in nanoseconds
    /*!
        \param duration - Time duration value in nanoseconds
    */
    explicit Timespan(int64_t duration) noexcept : _duration(duration) {}
    //! Initialize timespan with a given std::chrono duration
    /*!
        \param duration - std::chrono duration
    */
    template <class Rep, class Period>
    explicit Timespan(const std::chrono::duration<Rep, Period>& duration) noexcept : _duration(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) {}
    Timespan(const Timespan&) noexcept = default;
    Timespan(Timespan&&) noexcept = default;
    ~Timespan() noexcept = default;

    Timespan& operator=(int64_t duration) noexcept
    { _duration = duration; return *this; }
    Timespan& operator=(const Timespan&) noexcept = default;
    Timespan& operator=(Timespan&&) noexcept = default;

    // Timespan offset operations
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

    // Timespan comparison
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

    //! Convert timespan to the std::chrono nanoseconds duration
    std::chrono::system_clock::duration chrono() const noexcept
    { return std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::nanoseconds(_duration)); }

    //! Get total days of the current timespan
    int64_t days() const noexcept
    { return _duration / (24 * 60 * 60 * 1000000000ll); }
    //! Get total hours of the current timespan
    int64_t hours() const noexcept
    { return _duration / (60 * 60 * 1000000000ll); }
    //! Get total minutes of the current timespan
    int64_t minutes() const noexcept
    { return _duration / (60 * 1000000000ll); }
    //! Get total seconds of the current timespan
    int64_t seconds() const noexcept
    { return _duration / 1000000000; }
    //! Get total milliseconds of the current timespan
    int64_t milliseconds() const noexcept
    { return _duration / 1000000; }
    //! Get total microseconds of the current timespan
    int64_t microseconds() const noexcept
    { return _duration / 1000; }
    //! Get total nanoseconds of the current timespan
    int64_t nanoseconds() const noexcept
    { return _duration; }

    //! Get total value of the current timespan (total nanoseconds)
    int64_t total() const noexcept
    { return _duration; }

    //! Create the timespan based on the given days value
    static Timespan days(int64_t days) noexcept
    { return Timespan(days * 24 * 60 * 60 * 1000000000ll); }
    //! Create the timespan based on the given hours value
    static Timespan hours(int64_t hours) noexcept
    { return Timespan(hours * 60 * 60 * 1000000000ll); }
    //! Create the timespan based on the given minutes value
    static Timespan minutes(int64_t minutes) noexcept
    { return Timespan(minutes * 60 * 1000000000ll); }
    //! Create the timespan based on the given seconds value
    static Timespan seconds(int64_t seconds) noexcept
    { return Timespan(seconds * 1000000000); }
    //! Create the timespan based on the given milliseconds value
    static Timespan milliseconds(int64_t milliseconds) noexcept
    { return Timespan(milliseconds * 1000000); }
    //! Create the timespan based on the given microseconds value
    static Timespan microseconds(int64_t microseconds) noexcept
    { return Timespan(microseconds * 1000); }
    //! Create the timespan based on the given nanoseconds value
    static Timespan nanoseconds(int64_t nanoseconds) noexcept
    { return Timespan(nanoseconds); }

    //! Get zero timespan
    /*!
        Thread-safe.

        \return Zero timestamp
    */
    static Timespan zero() noexcept { return Timespan(0); }

    //! Swap two instances
    void swap(Timespan& timespan) noexcept;
    friend void swap(Timespan& timespan1, Timespan& timespan2) noexcept;

private:
    int64_t _duration;
};

/*! \example time_timespan.cpp Timespan example */

} // namespace CppCommon

#include "timespan.inl"

#endif // CPPCOMMON_TIME_TIMESPAN_H
