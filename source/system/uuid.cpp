/*!
    \file uuid.cpp
    \brief Universally unique identifier (UUID) implementation
    \author Ivan Shynkarenka
    \date 18.08.2016
    \copyright MIT License
*/

#include "system/uuid.h"

#include "errors/exceptions.h"

#include <iomanip>
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)

#endif

namespace CppCommon {

//! @cond
namespace Internals {

uint8_t unhex(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ch - '0';
    else if ((ch >= 'a') && (ch <= 'f'))
        return 10 + ch - 'a';
    else if ((ch >= 'A') && (ch <= 'F'))
        return 10 + ch - 'A';
    else
        return 0;
}

} // namespace Internals
//! @endcond

UUID::UUID(const std::string& uuid)
{
    char v1 = 0;
    char v2 = 0;
    bool pack = false;
    int index = 0;

    for (auto ch : uuid)
    {
        if ((ch == '-') || (ch == '{') || (ch == '}'))
            continue;

        if (pack)
        {
            v2 = ch;
            pack = false;
            _data[index++] = Internals::unhex(v1) * 16 + Internals::unhex(v2);
            if (index >= 16)
                break;
        }
        else
        {
            v1 = ch;
            pack = true;
        }
    }
}

std::string UUID::to_string() const
{
    std::string result(36, '0');

    const char* digits = "0123456789ABCDEF";

    int index = 0;
    for (auto value : _data)
    {
        result[index++] = digits[(value >> 4) & 0x0F];
        result[index++] = digits[(value >> 0) & 0x0F];
        if ((index == 8) || (index == 13) || (index == 18) || (index == 23))
            result[index++] = '-';
    }

    return result;
}

UUID UUID::Generate()
{
    UUID uuid;
#if defined(_WIN32) || defined(_WIN64)
    GUID guid;
    if (CoCreateGuid(&guid) != S_OK)
        throwex SystemException("Cannot generate UUID using CoCreateGuid() function!");

    uuid._data[0] = (guid.Data1 >> 24) & 0xFF;
    uuid._data[1] = (guid.Data1 >> 16) & 0xFF;
    uuid._data[2] = (guid.Data1 >>  8) & 0xFF;
    uuid._data[3] = (guid.Data1 >>  0) & 0xFF;

    uuid._data[4] = (guid.Data2 >>  8) & 0xFF;
    uuid._data[5] = (guid.Data2 >>  0) & 0xFF;

    uuid._data[6] = (guid.Data3 >>  8) & 0xFF;
    uuid._data[7] = (guid.Data3 >>  0) & 0xFF;

    uuid._data[8] = guid.Data4[0];
    uuid._data[9] = guid.Data4[1];

    uuid._data[10] = guid.Data4[2];
    uuid._data[11] = guid.Data4[3];
    uuid._data[12] = guid.Data4[4];
    uuid._data[13] = guid.Data4[5];
    uuid._data[14] = guid.Data4[6];
    uuid._data[15] = guid.Data4[7];
#elif defined(unix) || defined(__unix) || defined(__unix__)

#endif
    return uuid;
}

} // namespace CppCommon
