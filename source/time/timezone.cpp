/*!
    \file timezone.cpp
    \brief Timezone implementation
    \author Ivan Shynkarenka
    \date 18.07.2016
    \copyright MIT License
*/

#include "time/timezone.h"

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <time.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

Timezone::Timezone() : _name(), _offset(Timespan::zero()), _dstoffset(Timespan::zero())
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    struct tm local;
    time_t seconds = time(nullptr);
    if (localtime_r(&seconds, &local) != &local)
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
#elif defined(_WIN32) || defined(_WIN64)
    WCHAR* name;
    char buffer[1024];
    DYNAMIC_TIME_ZONE_INFORMATION dtzi;
    DWORD result = GetDynamicTimeZoneInformation(&dtzi);
    switch (result)
    {
        case TIME_ZONE_ID_UNKNOWN:
            name = dtzi.TimeZoneKeyName;
            _offset = -Timespan::minutes(dtzi.Bias);
            _dstoffset = Timespan::zero();
            break;
        case TIME_ZONE_ID_STANDARD:
            name = dtzi.StandardName;
            _offset = -Timespan::minutes(dtzi.Bias);
            _dstoffset = Timespan::zero();
            break;
        case TIME_ZONE_ID_DAYLIGHT:
            name = dtzi.DaylightName;
            _offset = -Timespan::minutes(dtzi.Bias);
            _dstoffset = -Timespan::minutes(dtzi.DaylightBias);
            break;
        default:
            throwex SystemException("Cannot get dynamic timezone informaction!");
    }
    // Convert timezone name to UTF-8 encoding.
    if (!WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, name, -1, buffer, 1024, nullptr, nullptr))
        throwex SystemException("Cannot get dynamic timezone key name!");
    _name = buffer;
#endif
}

} // namespace CppCommon
