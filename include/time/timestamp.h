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
    to get difference between two timestamps as a timespan.

    Nanosecond timestamp based on 64-bit integer can represent each nanosecond in the time
    range of ~584.554531 years. Therefore timestamp bounds from 01.01.1970 to 31.12.2553.

    Timestamp epoch January 1, 1970 at 00:00:00

    Not thread-safe.
*/
class Timestamp
{
public:
    //! Initialize timestamp with an epoch time
    Timestamp() noexcept : _timestamp(epoch()) {}
    //! Initialize timestamp with a given time moment in nanoseconds
    /*!
        \param timestamp - Time moment in nanoseconds
    */
    explicit Timestamp(uint64_t timestamp) noexcept : _timestamp(timestamp) {}
    //! Initialize timestamp with a given std::chrono time point
    /*!
        \param time_point - std::chrono time point
    */
    template <class Clock, class Duration>
    explicit Timestamp(const std::chrono::time_point<Clock, Duration>& time_point) noexcept : _timestamp(std::chrono::duration_cast<std::chrono::nanoseconds>(time_point.time_since_epoch()).count()) {}
    Timestamp(const Timestamp&) noexcept = default;
    Timestamp(Timestamp&&) noexcept = default;
    ~Timestamp() noexcept = default;

    Timestamp& operator=(uint64_t timestamp) noexcept
    { _timestamp = timestamp; return *this; }
    Timestamp& operator=(const Timestamp&) noexcept = default;
    Timestamp& operator=(Timestamp&&) noexcept = default;

    // Timestamp offset operations
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

    // Timestamp comparison
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

    //! Convert timestamp to the std::chrono time point
    std::chrono::system_clock::time_point chrono() const noexcept
    { return std::chrono::time_point_cast<std::chrono::system_clock::duration>((std::chrono::time_point<std::chrono::system_clock>() + std::chrono::nanoseconds(_timestamp))); }

    //! Get total days of the current timestamp
    uint64_t days() const noexcept
    { return _timestamp / (24 * 60 * 60 * 1000000000ull); }
    //! Get total hours of the current timestamp
    uint64_t hours() const noexcept
    { return _timestamp / (60 * 60 * 1000000000ull); }
    //! Get total minutes of the current timestamp
    uint64_t minutes() const noexcept
    { return _timestamp / (60 * 1000000000ull); }
    //! Get total seconds of the current timestamp
    uint64_t seconds() const noexcept
    { return _timestamp / 1000000000; }
    //! Get total milliseconds of the current timestamp
    uint64_t milliseconds() const noexcept
    { return _timestamp / 1000000; }
    //! Get total microseconds of the current timestamp
    uint64_t microseconds() const noexcept
    { return _timestamp / 1000; }
    //! Get total nanoseconds of the current timestamp
    uint64_t nanoseconds() const noexcept
    { return _timestamp; }

    //! Get total value of the current timestamp (total nanoseconds)
    uint64_t total() const noexcept { return _timestamp; }

    //! Create the timestamp based on the given days value
    static Timestamp days(int64_t days) noexcept
    { return Timestamp(days * 24 * 60 * 60 * 1000000000ull); }
    //! Create the timestamp based on the given hours value
    static Timestamp hours(int64_t hours) noexcept
    { return Timestamp(hours * 60 * 60 * 1000000000ull); }
    //! Create the timestamp based on the given minutes value
    static Timestamp minutes(int64_t minutes) noexcept
    { return Timestamp(minutes * 60 * 1000000000ull); }
    //! Create the timestamp based on the given seconds value
    static Timestamp seconds(int64_t seconds) noexcept
    { return Timestamp(seconds * 1000000000); }
    //! Create the timestamp based on the given milliseconds value
    static Timestamp milliseconds(int64_t milliseconds) noexcept
    { return Timestamp(milliseconds * 1000000); }
    //! Create the timestamp based on the given microseconds value
    static Timestamp microseconds(int64_t microseconds) noexcept
    { return Timestamp(microseconds * 1000); }
    //! Create the timestamp based on the given nanoseconds value
    static Timestamp nanoseconds(int64_t nanoseconds) noexcept
    { return Timestamp(nanoseconds); }

    //! Get the epoch timestamp
    /*!
        Thread-safe.

        \return Epoch timestamp
    */
    static uint64_t epoch() noexcept { return 0; }

    //! Get the UTC timestamp
    /*!
        Thread-safe.

        \return UTC timestamp
    */
    static uint64_t utc();

    //! Get the local timestamp
    /*!
        Thread-safe.

        \return Local timestamp
    */
    static uint64_t local();

    //! Get the high resolution timestamp
    /*!
        Thread-safe.

        \return High resolution timestamp
    */
    static uint64_t nano();

    //! Get the current value of RDTS (Read Time Stamp Counter)
    /*!
        Counts the number of CPU cycles since reset. The Time Stamp Counter (TSC)
        is a 64-bit register present on all x86 processors since the Pentium.

        Thread-safe.

        https://en.wikipedia.org/wiki/Time_Stamp_Counter

        \return Number of CPU cycles since the last system reset
    */
    static uint64_t rdts();

    //! Swap two instances
    void swap(Timestamp& timestamp) noexcept;
    friend void swap(Timestamp& timestamp1, Timestamp& timestamp2) noexcept;

protected:
    //! Timestamp value
    uint64_t _timestamp;
};

//! Epoch timestamp
class EpochTimestamp : public Timestamp
{
public:
    using Timestamp::Timestamp;

    //! Initialize epoch timestamp
    EpochTimestamp() : Timestamp(Timestamp::epoch()) {}
    //! Initialize epoch timestamp with another timestamp value
    EpochTimestamp(const Timestamp& timestamp) : Timestamp(timestamp) {}
};

//! UTC timestamp
class UtcTimestamp : public Timestamp
{
public:
    using Timestamp::Timestamp;

    //! Initialize UTC timestamp with a current UTC time
    UtcTimestamp() : Timestamp(Timestamp::utc()) {}
    //! Initialize UTC timestamp with another timestamp value
    UtcTimestamp(const Timestamp& timestamp) : Timestamp(timestamp) {}
};

//! Local timestamp
class LocalTimestamp : public Timestamp
{
public:
    using Timestamp::Timestamp;

    //! Initialize local timestamp with a current local time
    LocalTimestamp() : Timestamp(Timestamp::local()) {}
    //! Initialize local timestamp with another timestamp value
    LocalTimestamp(const Timestamp& timestamp) : Timestamp(timestamp) {}
};

//! High resolution timestamp
class NanoTimestamp : public Timestamp
{
public:
    using Timestamp::Timestamp;

    //! Initialize high resolution timestamp with a current high resolution time
    NanoTimestamp() : Timestamp(Timestamp::nano()) {}
    //! Initialize high resolution timestamp with another timestamp value
    NanoTimestamp(const Timestamp& timestamp) : Timestamp(timestamp) {}
};

//! RDTS timestamp
class RdtsTimestamp : public Timestamp
{
public:
    using Timestamp::Timestamp;

    //! Initialize RDTS timestamp with a current RDTS time
    RdtsTimestamp() : Timestamp(Timestamp::rdts()) {}
    //! Initialize RDTS timestamp with another timestamp value
    RdtsTimestamp(const Timestamp& timestamp) : Timestamp(timestamp) {}
};

/*! \example time_timestamp.cpp Timestamp example */

} // namespace CppCommon

#include "timestamp.inl"

#endif // CPPCOMMON_TIME_TIMESTAMP_H
