/*!
    \file time.inl
    \brief Time inline implementation
    \author Ivan Shynkarenka
    \date 12.07.2016
    \copyright MIT License
*/

namespace CppCommon {

template <class TOutputStream>
inline TOutputStream& operator<<(TOutputStream& stream, Weekday weekday)
{
    switch (weekday)
    {
        case Weekday::Sunday:
            stream << "Sunday";
            break;
        case Weekday::Monday:
            stream << "Monday";
            break;
        case Weekday::Tuesday:
            stream << "Tuesday";
            break;
        case Weekday::Wednesday:
            stream << "Wednesday";
            break;
        case Weekday::Thursday:
            stream << "Thursday";
            break;
        case Weekday::Friday:
            stream << "Friday";
            break;
        case Weekday::Saturday:
            stream << "Saturday";
            break;
        default:
            stream << "<unknown>";
            break;
    }
    return stream;
}

inline Time::Time() noexcept
    : _year(1970),
      _month(1),
      _day(1),
      _hour(0),
      _minute(0),
      _second(0),
      _millisecond(0),
      _microsecond(0),
      _nanosecond(0)
{
}

inline bool operator==(const Time& time1, const Time& time2) noexcept
{
    return ((time1._year == time2._year) && (time1._month == time2._month) && (time1._day == time2._day) &&
            (time1._hour == time2._hour) && (time1._minute == time2._minute) && (time1._second == time2._second) &&
            (time1._millisecond == time2._millisecond) && (time1._microsecond == time2._microsecond) && (time1._nanosecond == time2._nanosecond));
}

inline bool operator!=(const Time& time1, const Time& time2) noexcept
{
    return ((time1._year != time2._year) || (time1._month != time2._month) || (time1._day != time2._day) ||
            (time1._hour != time2._hour) || (time1._minute != time2._minute) || (time1._second != time2._second) ||
            (time1._millisecond != time2._millisecond) || (time1._microsecond != time2._microsecond) || (time1._nanosecond != time2._nanosecond));
}

inline bool operator>(const Time& time1, const Time& time2) noexcept
{
    if (time1._year < time2._year)
        return false;
    else if (time1._year == time2._year)
    {
        if (time1._month < time2._month)
            return false;
        else if (time1._month == time2._month)
        {
            if (time1._day < time2._day)
                return false;
            else if (time1._day == time2._day)
            {
                if (time1._hour < time2._hour)
                    return false;
                else if (time1._hour == time2._hour)
                {
                    if (time1._minute < time2._minute)
                        return false;
                    else if (time1._minute == time2._minute)
                    {
                        if (time1._second < time2._second)
                            return false;
                        else if (time1._second == time2._second)
                        {
                            if (time1._millisecond < time2._millisecond)
                                return false;
                            else if (time1._millisecond == time2._millisecond)
                            {
                                if (time1._microsecond < time2._microsecond)
                                    return false;
                                else if (time1._microsecond == time2._microsecond)
                                {
                                    if (time1._nanosecond <= time2._nanosecond)
                                        return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

inline bool operator<(const Time& time1, const Time& time2) noexcept
{
    if (time1._year > time2._year)
        return false;
    else if (time1._year == time2._year)
    {
        if (time1._month > time2._month)
            return false;
        else if (time1._month == time2._month)
        {
            if (time1._day > time2._day)
                return false;
            else if (time1._day == time2._day)
            {
                if (time1._hour > time2._hour)
                    return false;
                else if (time1._hour == time2._hour)
                {
                    if (time1._minute > time2._minute)
                        return false;
                    else if (time1._minute == time2._minute)
                    {
                        if (time1._second > time2._second)
                            return false;
                        else if (time1._second == time2._second)
                        {
                            if (time1._millisecond > time2._millisecond)
                                return false;
                            else if (time1._millisecond == time2._millisecond)
                            {
                                if (time1._microsecond > time2._microsecond)
                                    return false;
                                else if (time1._microsecond == time2._microsecond)
                                {
                                    if (time1._nanosecond >= time2._nanosecond)
                                        return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

inline bool operator>=(const Time& time1, const Time& time2) noexcept
{
    if (time1._year < time2._year)
        return false;
    else if (time1._year == time2._year)
    {
        if (time1._month < time2._month)
            return false;
        else if (time1._month == time2._month)
        {
            if (time1._day < time2._day)
                return false;
            else if (time1._day == time2._day)
            {
                if (time1._hour < time2._hour)
                    return false;
                else if (time1._hour == time2._hour)
                {
                    if (time1._minute < time2._minute)
                        return false;
                    else if (time1._minute == time2._minute)
                    {
                        if (time1._second < time2._second)
                            return false;
                        else if (time1._second == time2._second)
                        {
                            if (time1._millisecond < time2._millisecond)
                                return false;
                            else if (time1._millisecond == time2._millisecond)
                            {
                                if (time1._microsecond < time2._microsecond)
                                    return false;
                                else if (time1._microsecond == time2._microsecond)
                                {
                                    if (time1._nanosecond < time2._nanosecond)
                                        return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

inline bool operator<=(const Time& time1, const Time& time2) noexcept
{
    if (time1._year > time2._year)
        return false;
    else if (time1._year == time2._year)
    {
        if (time1._month > time2._month)
            return false;
        else if (time1._month == time2._month)
        {
            if (time1._day > time2._day)
                return false;
            else if (time1._day == time2._day)
            {
                if (time1._hour > time2._hour)
                    return false;
                else if (time1._hour == time2._hour)
                {
                    if (time1._minute > time2._minute)
                        return false;
                    else if (time1._minute == time2._minute)
                    {
                        if (time1._second > time2._second)
                            return false;
                        else if (time1._second == time2._second)
                        {
                            if (time1._millisecond > time2._millisecond)
                                return false;
                            else if (time1._millisecond == time2._millisecond)
                            {
                                if (time1._microsecond > time2._microsecond)
                                    return false;
                                else if (time1._microsecond == time2._microsecond)
                                {
                                    if (time1._nanosecond > time2._nanosecond)
                                        return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

inline void Time::swap(Time& time) noexcept
{
    using std::swap;
    swap(_year, time._year);
    swap(_month, time._month);
    swap(_day, time._day);
    swap(_hour, time._hour);
    swap(_minute, time._minute);
    swap(_second, time._second);
    swap(_millisecond, time._millisecond);
    swap(_microsecond, time._microsecond);
    swap(_nanosecond, time._nanosecond);
}

inline void swap(Time& time1, Time& time2) noexcept
{
    time1.swap(time2);
}

inline UtcTime::UtcTime(const LocalTime& time) : UtcTime(time.localstamp())
{}

inline LocalTime::LocalTime(const UtcTime& time) : LocalTime(time.utcstamp())
{}

} // namespace CppCommon

template <>
struct std::hash<CppCommon::Time>
{
    typedef CppCommon::Time argument_type;
    typedef size_t result_type;

    result_type operator() (const argument_type& value) const
    {
        result_type result = 17;
        std::hash<int> hasher;
        result = result * 31 + hasher(value.year());
        result = result * 31 + hasher(value.month());
        result = result * 31 + hasher(value.day());
        result = result * 31 + hasher(value.hour());
        result = result * 31 + hasher(value.minute());
        result = result * 31 + hasher(value.second());
        result = result * 31 + hasher(value.millisecond());
        result = result * 31 + hasher(value.microsecond());
        result = result * 31 + hasher(value.nanosecond());
        return result;
    }
};
