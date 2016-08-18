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

UUID::UUID(const std::string& uuid)
{

}

std::string UUID::to_string() const
{
    std::stringstream stream;
    stream << std::hex << std::setfill('0')
    << std::setw(2) << _data[0]
    << std::setw(2) << _data[1]
    << std::setw(2) << _data[2]
    << std::setw(2) << _data[3]
    << '-'
    << std::setw(2) << _data[4]
    << std::setw(2) << _data[5]
    << '-'
    << std::setw(2) << _data[6]
    << std::setw(2) << _data[7]
    << '-'
    << std::setw(2) << _data[8]
    << std::setw(2) << _data[9]
    << '-'
    << std::setw(2) << _data[10]
    << std::setw(2) << _data[11]
    << std::setw(2) << _data[12]
    << std::setw(2) << _data[13]
    << std::setw(2) << _data[14]
    << std::setw(2) << _data[15];
    return stream.str();
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
