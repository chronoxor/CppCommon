/*!
    \file uuid.cpp
    \brief Universally unique identifier (UUID) implementation
    \author Ivan Shynkarenka
    \date 18.08.2016
    \copyright MIT License
*/

#include "system/uuid.h"

#include "memory/memory.h"

#if defined(__MSYS__) || defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <rpc.h>
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <uuid/uuid.h>
#endif

namespace CppCommon {

//! @cond INTERNALS
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
        return 255;
}

} // namespace Internals
//! @endcond

UUID::UUID(const std::string& uuid)
{
    char v1 = 0;
    char v2 = 0;
    bool pack = false;
    size_t index = 0;

    // Parse UUID string
    for (auto ch : uuid)
    {
        if ((ch == '-') || (ch == '{') || (ch == '}'))
            continue;

        if (pack)
        {
            v2 = ch;
            pack = false;
            uint8_t ui1 = Internals::unhex(v1);
            uint8_t ui2 = Internals::unhex(v2);
            if ((ui1 > 15) || (ui2 > 15))
                throwex ArgumentException("Invalid UUID string: " + uuid);
            _data[index++] = ui1 * 16 + ui2;
            if (index >= 16)
                break;
        }
        else
        {
            v1 = ch;
            pack = true;
        }
    }

    // Fill remaining data with zeros
    for (; index < 16; ++index)
        _data[index++] = 0;
}

std::string UUID::string() const
{
    const char* digits = "0123456789abcdef";

    std::string result(36, '0');

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

UUID UUID::Sequential()
{
    UUID result;
#if defined(__MSYS__) || defined(_WIN32) || defined(_WIN64)
    ::UUID uuid;
    if (UuidCreateSequential(&uuid) != RPC_S_OK)
        throwex SystemException("Cannot generate sequential UUID!");

    result._data[0] = (uuid.Data1 >> 24) & 0xFF;
    result._data[1] = (uuid.Data1 >> 16) & 0xFF;
    result._data[2] = (uuid.Data1 >>  8) & 0xFF;
    result._data[3] = (uuid.Data1 >>  0) & 0xFF;

    result._data[4] = (uuid.Data2 >>  8) & 0xFF;
    result._data[5] = (uuid.Data2 >>  0) & 0xFF;

    result._data[6] = (uuid.Data3 >>  8) & 0xFF;
    result._data[7] = (uuid.Data3 >>  0) & 0xFF;

    result._data[8] = uuid.Data4[0];
    result._data[9] = uuid.Data4[1];

    result._data[10] = uuid.Data4[2];
    result._data[11] = uuid.Data4[3];
    result._data[12] = uuid.Data4[4];
    result._data[13] = uuid.Data4[5];
    result._data[14] = uuid.Data4[6];
    result._data[15] = uuid.Data4[7];
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    uuid_t uuid;
    uuid_generate_time(uuid);

    result._data[0] = uuid[0];
    result._data[1] = uuid[1];
    result._data[2] = uuid[2];
    result._data[3] = uuid[3];

    result._data[4] = uuid[4];
    result._data[5] = uuid[5];

    result._data[6] = uuid[6];
    result._data[7] = uuid[7];

    result._data[8] = uuid[8];
    result._data[9] = uuid[9];

    result._data[10] = uuid[10];
    result._data[11] = uuid[11];
    result._data[12] = uuid[12];
    result._data[13] = uuid[13];
    result._data[14] = uuid[14];
    result._data[15] = uuid[15];
#endif
    return result;
}

UUID UUID::Random()
{
    UUID result;
#if defined(__MSYS__) || defined(_WIN32) || defined(_WIN64)
    ::UUID uuid;
    if (UuidCreate(&uuid) != RPC_S_OK)
        throwex SystemException("Cannot generate random UUID!");

    result._data[0] = (uuid.Data1 >> 24) & 0xFF;
    result._data[1] = (uuid.Data1 >> 16) & 0xFF;
    result._data[2] = (uuid.Data1 >>  8) & 0xFF;
    result._data[3] = (uuid.Data1 >>  0) & 0xFF;

    result._data[4] = (uuid.Data2 >>  8) & 0xFF;
    result._data[5] = (uuid.Data2 >>  0) & 0xFF;

    result._data[6] = (uuid.Data3 >>  8) & 0xFF;
    result._data[7] = (uuid.Data3 >>  0) & 0xFF;

    result._data[8] = uuid.Data4[0];
    result._data[9] = uuid.Data4[1];

    result._data[10] = uuid.Data4[2];
    result._data[11] = uuid.Data4[3];
    result._data[12] = uuid.Data4[4];
    result._data[13] = uuid.Data4[5];
    result._data[14] = uuid.Data4[6];
    result._data[15] = uuid.Data4[7];
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    uuid_t uuid;
    uuid_generate_random(uuid);

    result._data[0] = uuid[0];
    result._data[1] = uuid[1];
    result._data[2] = uuid[2];
    result._data[3] = uuid[3];

    result._data[4] = uuid[4];
    result._data[5] = uuid[5];

    result._data[6] = uuid[6];
    result._data[7] = uuid[7];

    result._data[8] = uuid[8];
    result._data[9] = uuid[9];

    result._data[10] = uuid[10];
    result._data[11] = uuid[11];
    result._data[12] = uuid[12];
    result._data[13] = uuid[13];
    result._data[14] = uuid[14];
    result._data[15] = uuid[15];
#endif
    return result;
}

UUID UUID::Secure()
{
    UUID result;
    Memory::CryptoFill(result._data.data(), result._data.size());
    return result;
}

} // namespace CppCommon
