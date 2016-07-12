/*!
    \file timestamp.h
    \brief Timestamp definition
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_TIME_TIMESTAMP_H
#define CPPCOMMON_TIME_TIMESTAMP_H

#include "time/timespan.h"

namespace CppCommon {

//! Timestamp
/*!
    Timestamp wraps time moment in nanoseconds and allows to get separate values of days,
    hours, minutes, seconds, milliseconds, microseconds or nanoseconds. Also it is possible
    to get difference between two timestamps as a time span.

    Not thread-safe.
*/
class Timestamp
{
public:
    //! Initialize timestamp with a current time moment in nanoseconds
    Timestamp() noexcept : _timestamp(Timestamp::current()) {}
    //! Initialize timestamp with a time moment in nanoseconds
    /*!
        \param timestamp - Time moment in nanoseconds (default is the current time moment)
    */
    explicit Timestamp(uint64_t timestamp) noexcept : _timestamp(timestamp) {}
    Timestamp(const Timestamp&) noexcept = default;
    Timestamp(Timestamp&&) noexcept = default;
    ~Timestamp() noexcept = default;

    Timestamp& operator=(const Timestamp&) noexcept = default;
    Timestamp& operator=(Timestamp&&) noexcept = default;

    Timestamp& operator+=(int64_t offset) noexcept
    { _timestamp += offset; return *this; }
    Timestamp& operator+=(const Timespan& offset) noexcept
    { _timestamp += offset.total(); return *this; }

    Timestamp& operator-=(int64_t offset) noexcept
    { _timestamp -= offset; return *this; }
    Timestamp& operator-=(const Timespan& offset) noexcept
    { _timestamp -= offset.total(); return *this; }

    friend Timestamp operator+(const Timestamp& timestamp, int64_t offset) noexcept
    { return Timestamp(timestamp.total() + offset); }
    friend Timestamp operator+(int64_t offset, const Timestamp& timestamp) noexcept
    { return Timestamp(offset + timestamp.total()); }
    friend Timestamp operator+(const Timestamp& timestamp, const Timespan& offset) noexcept
    { return Timestamp(timestamp.total() + offset.total()); }
    friend Timestamp operator+(const Timespan& offset, const Timestamp& timestamp) noexcept
    { return Timestamp(offset.total() + timestamp.total()); }

    friend Timestamp operator-(const Timestamp& timestamp, int64_t offset) noexcept
    { return Timestamp(timestamp.total() - offset); }
    friend Timestamp operator-(int64_t offset, const Timestamp& timestamp) noexcept
    { return Timestamp(offset - timestamp.total()); }
    friend Timestamp operator-(const Timestamp& timestamp, const Timespan& offset) noexcept
    { return Timestamp(timestamp.total() - offset.total()); }
    friend Timestamp operator-(const Timespan& offset, const Timestamp& timestamp) noexcept
    { return Timestamp(offset.total() - timestamp.total()); }

    friend Timespan operator-(const Timestamp& timestamp1, const Timestamp& timestamp2) noexcept
    { return Timespan(timestamp1.total() - timestamp2.total()); }

    friend bool operator==(const Timestamp& timestamp1, uint64_t timestamp2) noexcept
    { return timestamp1.total() == timestamp2; }
    friend bool operator==(uint64_t timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1 == timestamp2.total(); }
    friend bool operator==(const Timestamp& timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1.total() == timestamp2.total(); }

    friend bool operator!=(const Timestamp& timestamp1, uint64_t timestamp2) noexcept
    { return timestamp1.total() != timestamp2; }
    friend bool operator!=(uint64_t timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1 != timestamp2.total(); }
    friend bool operator!=(const Timestamp& timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1.total() != timestamp2.total(); }

    friend bool operator>(const Timestamp& timestamp1, uint64_t timestamp2) noexcept
    { return timestamp1.total() > timestamp2; }
    friend bool operator>(uint64_t timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1 > timestamp2.total(); }
    friend bool operator>(const Timestamp& timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1.total() > timestamp2.total(); }

    friend bool operator<(const Timestamp& timestamp1, uint64_t timestamp2) noexcept
    { return timestamp1.total() < timestamp2; }
    friend bool operator<(uint64_t timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1 < timestamp2.total(); }
    friend bool operator<(const Timestamp& timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1.total() < timestamp2.total(); }

    friend bool operator>=(const Timestamp& timestamp1, uint64_t timestamp2) noexcept
    { return timestamp1.total() >= timestamp2; }
    friend bool operator>=(uint64_t timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1 >= timestamp2.total(); }
    friend bool operator>=(const Timestamp& timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1.total() >= timestamp2.total(); }

    friend bool operator<=(const Timestamp& timestamp1, uint64_t timestamp2) noexcept
    { return timestamp1.total() <= timestamp2; }
    friend bool operator<=(uint64_t timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1 <= timestamp2.total(); }
    friend bool operator<=(const Timestamp& timestamp1, const Timestamp& timestamp2) noexcept
    { return timestamp1.total() <= timestamp2.total(); }

    //! Get total days of the current timestamp
    uint64_t days() const noexcept { return _timestamp / (24 * 60 * 60 * 1000000000ll); }
    //! Get total hours of the current timestamp
    uint64_t hours() const noexcept { return _timestamp / (60 * 60 * 1000000000ll); }
    //! Get total minutes of the current timestamp
    uint64_t minutes() const noexcept { return _timestamp / (60 * 1000000000ll); }
    //! Get total seconds of the current timestamp
    uint64_t seconds() const noexcept { return _timestamp / 1000000000ll; }
    //! Get total milliseconds of the current timestamp
    uint64_t milliseconds() const noexcept { return _timestamp / 1000000; }
    //! Get total microseconds of the current timestamp
    uint64_t microseconds() const noexcept { return _timestamp / 1000; }
    //! Get total nanoseconds of the current timestamp
    uint64_t nanoseconds() const noexcept { return _timestamp; }

    //! Get total value of the current timestamp (total nanoseconds)
    uint64_t total() const noexcept { return _timestamp; }

    //! Get the current nanoseconds timestamp
    /*!
        Thread-safe.

        \return Timestamp in nanoseconds resolution
    */
    static uint64_t current() noexcept;

    //! Get the current value of RDTS (Read Time Stamp Counter)
    /*!
        Counts the number of CPU cycles since reset. The Time Stamp Counter (TSC)
        is a 64-bit register present on all x86 processors since the Pentium.

        Thread-safe.

        https://en.wikipedia.org/wiki/Time_Stamp_Counter

        \return Number of CPU cycles since the last system reset
    */
    static uint64_t rdts() noexcept;

private:
    uint64_t _timestamp;
};

/*! \example time_timestamp.cpp Timestamp example */

} // namespace CppCommon

#endif // CPPCOMMON_TIME_TIMESTAMP_H
