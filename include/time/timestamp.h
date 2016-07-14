/*!
    \file timestamp.h
    \brief Timestamp wrapper definition
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

    Timestamp epoch January 1, 1970 at 00:00:00

    Not thread-safe.
*/
class Timestamp
{
public:
    //! Initialize timestamp with a given time moment in nanoseconds
    /*!
        \param timestamp - Time moment in nanoseconds
    */
    explicit Timestamp(uint64_t timestamp) noexcept : _timestamp(timestamp) {}
    Timestamp(const Timestamp&) noexcept = default;
    Timestamp(Timestamp&&) noexcept = default;
    ~Timestamp() noexcept = default;

    Timestamp& operator=(uint64_t timestamp) noexcept
    { _timestamp = timestamp; return *this; }
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

    //! Convert timestamp to the std::chrono time point
    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<uint64_t, std::nano>> chrono() const noexcept
    { return std::chrono::time_point<std::chrono::system_clock>() + std::chrono::nanoseconds(_timestamp); }
    //! Convert std::chrono time point to timestamp
    template <class Clock, class Duration>
    static Timestamp chrono(const std::chrono::time_point<Clock, Duration>& time_point) noexcept
    { return Timestamp(std::chrono::duration_cast<std::chrono::nanoseconds>(time_point.time_since_epoch()).count()); }

    //! Get total days of the current timestamp
    uint64_t days() const noexcept
    { return _timestamp / (24 * 60 * 60 * 1000000000ll); }
    //! Get total hours of the current timestamp
    uint64_t hours() const noexcept
    { return _timestamp / (60 * 60 * 1000000000ll); }
    //! Get total minutes of the current timestamp
    uint64_t minutes() const noexcept
    { return _timestamp / (60 * 1000000000ll); }
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

    //! Set total days of the current timestamp
    static Timestamp days(int64_t days) noexcept
    { return Timestamp(days * 24 * 60 * 60 * 1000000000ll); }
    //! Set total hours of the current timestamp
    static Timestamp hours(int64_t hours) noexcept
    { return Timestamp(hours * 60 * 60 * 1000000000ll); }
    //! Set total minutes of the current timestamp
    static Timestamp minutes(int64_t minutes) noexcept
    { return Timestamp(minutes * 60 * 1000000000ll); }
    //! Set total seconds of the current timestamp
    static Timestamp seconds(int64_t seconds) noexcept
    { return Timestamp(seconds * 1000000000); }
    //! Set total milliseconds of the current timestamp
    static Timestamp milliseconds(int64_t milliseconds) noexcept
    { return Timestamp(milliseconds * 1000000); }
    //! Set total microseconds of the current timestamp
    static Timestamp microseconds(int64_t microseconds) noexcept
    { return Timestamp(microseconds * 1000); }
    //! Set total nanoseconds of the current timestamp
    static Timestamp nanoseconds(int64_t nanoseconds) noexcept
    { return Timestamp(nanoseconds); }

    //! Get total value of the current timestamp (total nanoseconds)
    uint64_t total() const noexcept { return _timestamp; }

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

protected:
    uint64_t _timestamp;
};

//! UTC timestamp
class UtcTimestamp : public Timestamp
{
public:
    using Timestamp::Timestamp;

    //! Initialize UTC timestamp with a current UTC time
    UtcTimestamp() : Timestamp(Timestamp::utc()) {}
    //! Initialize UTC timestamp with another timestamp value
    /*!
        \param timestamp - Timestamp
    */
    UtcTimestamp(const Timestamp& timestamp) noexcept : Timestamp(timestamp) {};
    UtcTimestamp(const UtcTimestamp&) noexcept = default;
    UtcTimestamp(UtcTimestamp&&) noexcept = default;
    ~UtcTimestamp() noexcept = default;

    UtcTimestamp& operator=(const Timestamp& timestamp) noexcept
    { Timestamp::operator=(timestamp); return *this; }
    UtcTimestamp& operator=(Timestamp&& timestamp) noexcept
    { Timestamp::operator=(timestamp); return *this; }
    UtcTimestamp& operator=(const UtcTimestamp&) noexcept = default;
    UtcTimestamp& operator=(UtcTimestamp&&) noexcept = default;
};

//! Local timestamp
class LocalTimestamp : public Timestamp
{
public:
    using Timestamp::Timestamp;

    //! Initialize local timestamp with a current local time
    LocalTimestamp() : Timestamp(Timestamp::local()) {}
    //! Initialize local timestamp with another timestamp value
    /*!
        \param timestamp - Timestamp
    */
    LocalTimestamp(const Timestamp& timestamp) noexcept : Timestamp(timestamp) {};
    LocalTimestamp(const LocalTimestamp&) noexcept = default;
    LocalTimestamp(LocalTimestamp&&) noexcept = default;
    ~LocalTimestamp() noexcept = default;

    LocalTimestamp& operator=(const Timestamp& timestamp) noexcept
    { Timestamp::operator=(timestamp); return *this; }
    LocalTimestamp& operator=(Timestamp&& timestamp) noexcept
    { Timestamp::operator=(timestamp); return *this; }
    LocalTimestamp& operator=(const LocalTimestamp&) noexcept = default;
    LocalTimestamp& operator=(LocalTimestamp&&) noexcept = default;
};

//! High resolution timestamp
class NanoTimestamp : public Timestamp
{
public:
    using Timestamp::Timestamp;

    //! Initialize high resolution timestamp with a current high resolution time
    NanoTimestamp() : Timestamp(Timestamp::nano()) {}
    //! Initialize high resolution timestamp with another timestamp value
    /*!
        \param timestamp - Timestamp
    */
    NanoTimestamp(const Timestamp& timestamp) noexcept : Timestamp(timestamp) {};
    NanoTimestamp(const NanoTimestamp&) noexcept = default;
    NanoTimestamp(NanoTimestamp&&) noexcept = default;
    ~NanoTimestamp() noexcept = default;


    NanoTimestamp& operator=(const Timestamp& timestamp) noexcept
    { Timestamp::operator=(timestamp); return *this; }
    NanoTimestamp& operator=(Timestamp&& timestamp) noexcept
    { Timestamp::operator=(timestamp); return *this; }
    NanoTimestamp& operator=(const NanoTimestamp&) noexcept = default;
    NanoTimestamp& operator=(NanoTimestamp&&) noexcept = default;
};

//! RDTS timestamp
class RdtsTimestamp : public Timestamp
{
public:
    using Timestamp::Timestamp;

    //! Initialize RDTS timestamp with a current RDTS time
    RdtsTimestamp() : Timestamp(Timestamp::rdts()) {}
    //! Initialize RDTS timestamp with another timestamp value
    /*!
        \param timestamp - Timestamp
    */
    RdtsTimestamp(const Timestamp& timestamp) noexcept : Timestamp(timestamp) {};
    RdtsTimestamp(const RdtsTimestamp&) noexcept = default;
    RdtsTimestamp(RdtsTimestamp&&) noexcept = default;
    ~RdtsTimestamp() noexcept = default;

    RdtsTimestamp& operator=(const Timestamp& timestamp) noexcept
    { Timestamp::operator=(timestamp); return *this; }
    RdtsTimestamp& operator=(Timestamp&& timestamp) noexcept
    { Timestamp::operator=(timestamp); return *this; }
    RdtsTimestamp& operator=(const RdtsTimestamp&) noexcept = default;
    RdtsTimestamp& operator=(RdtsTimestamp&&) noexcept = default;
};

/*! \example time_timestamp.cpp Timestamp wrapper example */

} // namespace CppCommon

#endif // CPPCOMMON_TIME_TIMESTAMP_H
