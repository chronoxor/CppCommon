/*!
    \file time.h
    \brief Time definition
    \author Ivan Shynkarenka
    \date 12.07.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_TIME_TIME_H
#define CPPCOMMON_TIME_TIME_H

#include "time/timestamp.h"

namespace CppCommon {

//! Weekday
enum class Weekday
{
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

//! Stream output: Weekday
/*!
    \param stream - Output stream
    \param weekday - Weekday
    \return Output stream
*/
template <class TOutputStream>
TOutputStream& operator<<(TOutputStream& stream, Weekday weekday);

//! Time
/*!
    Time wraps date & time in a single object with a set of accessors - year, month, day,
    hours, minutes, seconds, milliseconds, microseconds or nanoseconds.

    32-bit: time is limited in range 1970-01-01T00:00:00Z - 2038-01-18T23:59:59Z
    64-bit: time is limited in range 1970-01-01T00:00:00Z - 3000-12-31T23:59:59Z

    Not thread-safe.
*/
class Time
{
public:
    //! Initialize time with an epoch time
    Time() noexcept;
    //! Initialize time with a given date & time components (year, month, day, hour, minute, second, etc.)
    /*!
        \param year - Year value (1970-2038 for 32-bit or 1970-3000 for 64-bit)
        \param month - Month value (1-12)
        \param day - Day value (1-31)
        \param hour - Hour value (0-23) (default is 0)
        \param minute - Minute value (0-59) (default is 0)
        \param second - Second value (0-59) (default is 0)
        \param millisecond - Millisecond value (0-999) (default is 0)
        \param microsecond - Microsecond value (0-999) (default is 0)
        \param nanosecond - Nanosecond value (0-999) (default is 0)
    */
    explicit Time(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0, int nanosecond = 0);
    Time(const Time&) noexcept = default;
    Time(Time&&) noexcept = default;
    ~Time() noexcept = default;

    Time& operator=(const Timestamp& timestamp)
    { return operator=(Time(timestamp)); }
    Time& operator=(const Time&) noexcept = default;
    Time& operator=(Time&&) noexcept = default;

    // Time offset operations
    Time& operator+=(const Timespan& offset)
    { return operator=(Time(utcstamp() + offset)); }

    Time& operator-=(const Timespan& offset)
    { return operator=(Time(utcstamp() - offset)); }

    friend Time operator+(const Time& time, const Timespan& offset)
    { return Time(time.utcstamp() + offset); }
    friend Time operator+(const Timespan& offset, const Time& time)
    { return Time(offset + time.utcstamp()); }

    friend Time operator-(const Time& time, const Timespan& offset)
    { return Time(time.utcstamp() - offset); }
    friend Time operator-(const Timespan& offset, const Time& time)
    { return Time(offset - time.utcstamp()); }

    friend Timespan operator-(const Time& time1, const Time& time2)
    { return time1.utcstamp() - time2.utcstamp(); }

    // Time comparison
    friend bool operator==(const Time& time, const Timestamp& timestamp)
    { return time == Time(timestamp); }
    friend bool operator==(const Timestamp& timestamp, const Time& time)
    { return Time(timestamp) == time; }
    friend bool operator==(const Time& time1, const Time& time2) noexcept;

    friend bool operator!=(const Time& time, const Timestamp& timestamp)
    { return time != Time(timestamp); }
    friend bool operator!=(const Timestamp& timestamp, const Time& time)
    { return Time(timestamp) != time; }
    friend bool operator!=(const Time& time1, const Time& time2) noexcept;

    friend bool operator>(const Time& time, const Timestamp& timestamp)
    { return time > Time(timestamp); }
    friend bool operator>(const Timestamp& timestamp, const Time& time)
    { return Time(timestamp) > time; }
    friend bool operator>(const Time& time1, const Time& time2) noexcept;

    friend bool operator<(const Time& time, const Timestamp& timestamp)
    { return time < Time(timestamp); }
    friend bool operator<(const Timestamp& timestamp, const Time& time)
    { return Time(timestamp) < time; }
    friend bool operator<(const Time& time1, const Time& time2) noexcept;

    friend bool operator>=(const Time& time, const Timestamp& timestamp)
    { return time >= Time(timestamp); }
    friend bool operator>=(const Timestamp& timestamp, const Time& time)
    { return Time(timestamp) >= time; }
    friend bool operator>=(const Time& time1, const Time& time2) noexcept;

    friend bool operator<=(const Time& time, const Timestamp& timestamp)
    { return time <= Time(timestamp); }
    friend bool operator<=(const Timestamp& timestamp, const Time& time)
    { return Time(timestamp) <= time; }
    friend bool operator<=(const Time& time1, const Time& time2) noexcept;

    //! Convert date & time to the std::chrono time point
    std::chrono::time_point<std::chrono::system_clock> chrono() const
    { return utcstamp().chrono(); }

    //! Get year value (1970-2038 for 32-bit or 1970-3000 for 64-bit)
    int year() const noexcept { return _year; }
    //! Get month value (1-12)
    int month() const noexcept { return _month; }
    //! Get weekday
    Weekday weekday() const noexcept { return (Weekday)_weekday; }
    //! Get day value (1-31)
    int day() const noexcept { return _day; }
    //! Get hour value (0-23)
    int hour() const noexcept { return _hour; }
    //! Get minute value (0-59)
    int minute() const noexcept { return _minute; }
    //! Get second value (0-59)
    int second() const noexcept { return _second; }
    //! Get millisecond value (0-999)
    int millisecond() const noexcept { return _millisecond; }
    //! Get microsecond value (0-999)
    int microsecond() const noexcept { return _microsecond; }
    //! Get nanosecond value (0-999)
    int nanosecond() const noexcept { return _nanosecond; }

    //! Get UTC timestamp from the current date & time value
    UtcTimestamp utcstamp() const;
    //! Get local timestamp from the current date & time value
    LocalTimestamp localstamp() const;

    //! Get the epoch date & time
    /*!
        Thread-safe.

        \return Epoch date & time
    */
    static Time epoch()
    { return Time(1970, 1, 1); }

    //! Swap two instances
    void swap(Time& time) noexcept;
    friend void swap(Time& time1, Time& time2) noexcept;

protected:
    //! Year value
    int _year;
    //! Month value
    int _month;
    //! Weekday value
    int _weekday;
    //! Day value
    int _day;
    //! Hour value
    int _hour;
    //! Minute value
    int _minute;
    //! Second value
    int _second;
    //! Millisecond value
    int _millisecond;
    //! Microsecond value
    int _microsecond;
    //! Nanosecond value
    int _nanosecond;

    //! Protected initialize time with a timestamp
    /*!
        \param timestamp - Timestamp
    */
    explicit Time(const Timestamp& timestamp);
};

// Forward class declarations
class UtcTime;
class LocalTime;

//! UTC time
class UtcTime : public Time
{
public:
    using Time::Time;
    using Time::chrono;

    //! Initialize UTC time with a current value
    UtcTime() : UtcTime(UtcTimestamp()) {}
    //! Initialize UTC time with a given timestamp
    /*!
        \param timestamp - Timestamp
    */
    explicit UtcTime(const Timestamp& timestamp);
    //! Initialize UTC time with a given std::chrono time point
    /*!
        \param time_point - std::chrono time point
    */
    template <class Clock, class Duration>
    explicit UtcTime(const std::chrono::time_point<Clock, Duration>& time_point) : UtcTime(Timestamp(time_point)) {}
    //! Initialize UTC time with another time value
    UtcTime(const Time& time) : Time(time) {}
    //! Initialize UTC time with another local time value
    UtcTime(const LocalTime& time);
};

//! Local time
class LocalTime : public Time
{
public:
    using Time::Time;
    using Time::chrono;

    //! Initialize local time with a current value
    LocalTime() : LocalTime(UtcTimestamp()) {}
    //! Initialize local time with a given timestamp
    /*!
        \param timestamp - Timestamp
    */
    explicit LocalTime(const Timestamp& timestamp);
    //! Initialize local time with a given std::chrono time point
    /*!
        \param time_point - std::chrono time point
    */
    template <class Clock, class Duration>
    explicit LocalTime(const std::chrono::time_point<Clock, Duration>& time_point) : LocalTime(Timestamp(time_point)) {}
    //! Initialize local time with another time value
    LocalTime(const Time& time) : Time(time) {}
    //! Initialize local time with another UTC time value
    LocalTime(const UtcTime& time);
};

/*! \example time_time.cpp Time example */

} // namespace CppCommon

#include "time.inl"

#endif // CPPCOMMON_TIME_TIME_H
