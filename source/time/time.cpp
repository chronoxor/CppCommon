/*!
    \file time.cpp
    \brief Time implementation
    \author Ivan Shynkarenka
    \date 12.07.2016
    \copyright MIT License
*/

#include "time/time.h"

#include "string/format.h"

#include <cassert>

#include <time.h>

namespace CppCommon {

Time::Time(const Timestamp& timestamp)
{
    struct tm result;
    time_t seconds = timestamp.seconds();
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    if (gmtime_r(&seconds, &result) != &result)
        throwex SystemException(format("Cannot convert the given timestamp ({}) to date & time structure!", timestamp.total()));
#elif defined(_WIN32) || defined(_WIN64)
    if (gmtime_s(&result, &seconds))
        throwex SystemException(format("Cannot convert the given timestamp ({}) to date & time structure!", timestamp.total()));
#endif
    _year = result.tm_year + 1900;
    _month = result.tm_mon + 1;
    _weekday = result.tm_wday;
    _day = result.tm_mday;
    _hour = result.tm_hour;
    _minute = result.tm_min;
    _second = result.tm_sec % 60;
    _millisecond = timestamp.milliseconds() % 1000;
    _microsecond = timestamp.microseconds() % 1000;
    _nanosecond = timestamp.nanoseconds() % 1000;
}

Time::Time(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond, int nanosecond)
{
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4127) // C4127: conditional expression is constant
#endif
    if (sizeof(time_t) == 4)
    {
        assert(((year >= 1970) && (year <= 2038)) && "Year value is limited in range from 1970 to 2038!");
        if ((year < 1970) || (year > 2038))
            throwex ArgumentException("Year value is limited in range from 1970 to 2038!");
    }
    else
    {
        assert(((year >= 1970) && (year <= 3000)) && "Year value is limited in range from 1970 to 3000!");
        if ((year < 1970) || (year > 3000))
            throwex ArgumentException("Year value is limited in range from 1970 to 3000!");
    }
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
    assert(((month >= 1) && (month <= 12)) && "Month value is limited in range from 1 to 12!");
    if ((month < 1) || (month > 12))
        throwex ArgumentException("Month value is limited in range from 1 to 12!");
    assert(((day >= 1) && (day <= 31)) && "Day value is limited in range from 1 to 31!");
    if ((day < 1) || (day > 31))
        throwex ArgumentException("Day value is limited in range from 1 to 31!");
    assert(((hour >= 0) && (hour <= 23)) && "Hour value is limited in range from 0 to 23!");
    if ((hour < 0) || (hour > 23))
        throwex ArgumentException("Hour value is limited in range from 0 to 23!");
    assert(((minute >= 0) && (minute <= 59)) && "Minute value is limited in range from 0 to 59!");
    if ((minute < 0) || (minute > 59))
        throwex ArgumentException("Minute value is limited in range from 0 to 59!");
    assert(((second >= 0) && (second <= 59)) && "Second value is limited in range from 0 to 59!");
    if ((second < 0) || (second > 59))
        throwex ArgumentException("Second value is limited in range from 0 to 59!");
    assert(((millisecond >= 0) && (millisecond <= 999)) && "Millisecond value is limited in range from 0 to 999!");
    if ((millisecond < 0) || (millisecond > 999))
        throwex ArgumentException("Millisecond value is limited in range from 0 to 999!");
    assert(((microsecond >= 0) && (microsecond <= 999)) && "Microsecond value is limited in range from 0 to 999!");
    if ((microsecond < 0) || (microsecond > 999))
        throwex ArgumentException("Microsecond value is limited in range from 0 to 999!");
    assert(((nanosecond >= 0) && (nanosecond <= 999)) && "Nanosecond value is limited in range from 0 to 999!");
    if ((nanosecond < 0) || (nanosecond > 999))
        throwex ArgumentException("Nanosecond value is limited in range from 0 to 999!");

    _year = year;
    _month = month;
    _weekday = 0;
    _day = day;
    _hour = hour;
    _minute = minute;
    _second = second;
    _millisecond = millisecond;
    _microsecond = microsecond;
    _nanosecond = nanosecond;
}

UtcTimestamp Time::utcstamp() const
{
    struct tm result;
    result.tm_year = _year - 1900;
    result.tm_mon = _month - 1;
    result.tm_mday = _day;
    result.tm_hour = _hour;
    result.tm_min = _minute;
    result.tm_sec = _second;
    result.tm_isdst = -1;

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    time_t seconds = timegm(&result);
#elif defined(_WIN32) || defined(_WIN64)
    time_t seconds = _mkgmtime(&result);
#endif
    if (seconds == -1)
        throwex SystemException("Cannot convert date & time to UTC timestamp!");

    return UtcTimestamp(seconds * 1000000000ull + _millisecond * 1000000ull + _microsecond * 1000ull + _nanosecond);
}

LocalTimestamp Time::localstamp() const
{
    struct tm result;
    result.tm_year = _year - 1900;
    result.tm_mon = _month - 1;
    result.tm_mday = _day;
    result.tm_hour = _hour;
    result.tm_min = _minute;
    result.tm_sec = _second;
    result.tm_isdst = -1;

    time_t seconds = mktime(&result);
    if (seconds == -1)
        throwex SystemException("Cannot convert date & time to local timestamp!");

    return LocalTimestamp(seconds * 1000000000ull + _millisecond * 1000000ull + _microsecond * 1000ull + _nanosecond);
}

UtcTime::UtcTime(const Timestamp& timestamp)
{
    struct tm result;
    time_t seconds = timestamp.seconds();
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    if (gmtime_r(&seconds, &result) != &result)
        throwex SystemException(format("Cannot convert the given timestamp ({}) to UTC date & time structure!", timestamp.total()));
#elif defined(_WIN32) || defined(_WIN64)
    if (gmtime_s(&result, &seconds))
        throwex SystemException(format("Cannot convert the given timestamp ({}) to UTC date & time structure!", timestamp.total()));
#endif
    _year = result.tm_year + 1900;
    _month = result.tm_mon + 1;
    _weekday = result.tm_wday;
    _day = result.tm_mday;
    _hour = result.tm_hour;
    _minute = result.tm_min;
    _second = result.tm_sec % 60;
    _millisecond = timestamp.milliseconds() % 1000;
    _microsecond = timestamp.microseconds() % 1000;
    _nanosecond = timestamp.nanoseconds() % 1000;
}

LocalTime::LocalTime(const Timestamp& timestamp)
{
    struct tm result;
    time_t seconds = timestamp.seconds();
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    if (localtime_r(&seconds, &result) != &result)
        throwex SystemException(format("Cannot convert the given timestamp ({}) to local date & time structure!", timestamp.total()));
#elif defined(_WIN32) || defined(_WIN64)
    if (localtime_s(&result, &seconds))
        throwex SystemException(format("Cannot convert the given timestamp ({}) to local date & time structure!", timestamp.total()));
#endif
    _year = result.tm_year + 1900;
    _month = result.tm_mon + 1;
    _weekday = result.tm_wday;
    _day = result.tm_mday;
    _hour = result.tm_hour;
    _minute = result.tm_min;
    _second = result.tm_sec % 60;
    _millisecond = timestamp.milliseconds() % 1000;
    _microsecond = timestamp.microseconds() % 1000;
    _nanosecond = timestamp.nanoseconds() % 1000;
}

} // namespace CppCommon
