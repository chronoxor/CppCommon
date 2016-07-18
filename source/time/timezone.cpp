/*!
    \file timezone.cpp
    \brief Time zone wrapper implementation
    \author Ivan Shynkarenka
    \date 18.07.2016
    \copyright MIT License
*/

#include "time/timezone.h"

#include "errors/exceptions.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <time.h>
#endif

namespace CppCommon {

Timezone::Timezone() : _name(), _offset(Timespan::zero()), _dstoffset(Timespan::zero())
{
#if defined(_WIN32) || defined(_WIN64)
    char buffer[1024];
    DYNAMIC_TIME_ZONE_INFORMATION dtzi;
    DWORD result = GetDynamicTimeZoneInformation(&dtzi);
    switch (result)
    {
        case TIME_ZONE_ID_UNKNOWN:
            if (!WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, dtzi.TimeZoneKeyName, -1, buffer, 1024, nullptr, nullptr))
                throwex SystemException("Cannot get dynamic time zone key name!");
            _name = buffer;
            _offset = Timespan::minutes(dtzi.Bias);
            _dstoffset = Timespan::zero();
            break;
        case TIME_ZONE_ID_STANDARD:
            if (!WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, dtzi.StandardName, -1, buffer, 1024, nullptr, nullptr))
                throwex SystemException("Cannot get dynamic time zone standard name!");
            _name = buffer;
            _offset = Timespan::minutes(dtzi.Bias);
            _dstoffset = Timespan::zero();
            break;
        case TIME_ZONE_ID_DAYLIGHT:
            if (!WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, dtzi.DaylightName, -1, buffer, 1024, nullptr, nullptr))
                throwex SystemException("Cannot get dynamic time zone daylight name!");
            _name = buffer;
            _offset = Timespan::minutes(dtzi.Bias - dtzi.DaylightBias);
            _dstoffset = Timespan::minutes(dtzi.DaylightBias);
            break;
        default:
            throwex SystemException("Cannot get dynamic time zone informaction!");
            break;
    }
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct tm local;
    time_t time = time(nullptr);
    if (localtime_r(&time, &local) != &local)
        throwex SystemException("Cannot convert current time to local date & time structure!");
    _name = local.tm_zone;
    if (local.tm_isdst > 0)
    {
        _offset = Timespan::seconds(local.tm_gmtoff - 3600);
        _dstoffset = Timespan::seconds(3600);
    }
    else
    {
        _offset = Timespan::seconds(local.tm_gmtoff);
        _dstoffset = Timespan::zero();
    }
#endif
}

} // namespace CppCommon
