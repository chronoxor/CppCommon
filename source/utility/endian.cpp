/*!
    \file endian.cpp
    \brief Big/Little-endian utilities implementation
    \author Ivan Shynkarenka
    \date 21.07.2017
    \copyright MIT License
*/

#include "utility/endian.h"

namespace CppCommon {

void Endian::ReadBigEndian(const void* buffer, int16_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[0];
    }
}

void Endian::ReadBigEndian(const void* buffer, uint16_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[0];
    }
}

void Endian::ReadBigEndian(const void* buffer, int32_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[3];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[0];
    }
}

void Endian::ReadBigEndian(const void* buffer, uint32_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[3];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[0];
    }
}

void Endian::ReadBigEndian(const void* buffer, int64_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[4] = ((const uint8_t*)buffer)[4];
        ((uint8_t*)&value)[5] = ((const uint8_t*)buffer)[5];
        ((uint8_t*)&value)[6] = ((const uint8_t*)buffer)[6];
        ((uint8_t*)&value)[7] = ((const uint8_t*)buffer)[7];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[7];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[6];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[5];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[4];
        ((uint8_t*)&value)[4] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[5] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[6] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[7] = ((const uint8_t*)buffer)[0];
    }
}

void Endian::ReadBigEndian(const void* buffer, uint64_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[4] = ((const uint8_t*)buffer)[4];
        ((uint8_t*)&value)[5] = ((const uint8_t*)buffer)[5];
        ((uint8_t*)&value)[6] = ((const uint8_t*)buffer)[6];
        ((uint8_t*)&value)[7] = ((const uint8_t*)buffer)[7];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[7];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[6];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[5];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[4];
        ((uint8_t*)&value)[4] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[5] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[6] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[7] = ((const uint8_t*)buffer)[0];
    }
}

void Endian::ReadLittleEndian(const void* buffer, int16_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[0];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
    }
}

void Endian::ReadLittleEndian(const void* buffer, uint16_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[0];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
    }
}

void Endian::ReadLittleEndian(const void* buffer, int32_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[0];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[3];
    }
}

void Endian::ReadLittleEndian(const void* buffer, uint32_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[0];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[3];
    }
}

void Endian::ReadLittleEndian(const void* buffer, int64_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[7];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[6];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[5];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[4];
        ((uint8_t*)&value)[4] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[5] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[6] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[7] = ((const uint8_t*)buffer)[0];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[4] = ((const uint8_t*)buffer)[4];
        ((uint8_t*)&value)[5] = ((const uint8_t*)buffer)[5];
        ((uint8_t*)&value)[6] = ((const uint8_t*)buffer)[6];
        ((uint8_t*)&value)[7] = ((const uint8_t*)buffer)[7];
    }
}

void Endian::ReadLittleEndian(const void* buffer, uint64_t& value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[7];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[6];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[5];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[4];
        ((uint8_t*)&value)[4] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[5] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[6] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[7] = ((const uint8_t*)buffer)[0];
    }
    else
    {
        ((uint8_t*)&value)[0] = ((const uint8_t*)buffer)[0];
        ((uint8_t*)&value)[1] = ((const uint8_t*)buffer)[1];
        ((uint8_t*)&value)[2] = ((const uint8_t*)buffer)[2];
        ((uint8_t*)&value)[3] = ((const uint8_t*)buffer)[3];
        ((uint8_t*)&value)[4] = ((const uint8_t*)buffer)[4];
        ((uint8_t*)&value)[5] = ((const uint8_t*)buffer)[5];
        ((uint8_t*)&value)[6] = ((const uint8_t*)buffer)[6];
        ((uint8_t*)&value)[7] = ((const uint8_t*)buffer)[7];
    }
}

void Endian::WriteBigEndian(void* buffer, int16_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[0];
    }
}

void Endian::WriteBigEndian(void* buffer, uint16_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[0];
    }
}

void Endian::WriteBigEndian(void* buffer, int32_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[3];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[0];
    }
}

void Endian::WriteBigEndian(void* buffer, uint32_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[3];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[0];
    }
}

void Endian::WriteBigEndian(void* buffer, int64_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[4] = ((const uint8_t*)&value)[4];
        ((uint8_t*)buffer)[5] = ((const uint8_t*)&value)[5];
        ((uint8_t*)buffer)[6] = ((const uint8_t*)&value)[6];
        ((uint8_t*)buffer)[7] = ((const uint8_t*)&value)[7];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[7];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[6];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[5];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[4];
        ((uint8_t*)buffer)[4] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[5] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[6] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[7] = ((const uint8_t*)&value)[0];
    }
}

void Endian::WriteBigEndian(void* buffer, uint64_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[4] = ((const uint8_t*)&value)[4];
        ((uint8_t*)buffer)[5] = ((const uint8_t*)&value)[5];
        ((uint8_t*)buffer)[6] = ((const uint8_t*)&value)[6];
        ((uint8_t*)buffer)[7] = ((const uint8_t*)&value)[7];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[7];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[6];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[5];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[4];
        ((uint8_t*)buffer)[4] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[5] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[6] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[7] = ((const uint8_t*)&value)[0];
    }
}

void Endian::WriteLittleEndian(void* buffer, int16_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[0];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
    }
}

void Endian::WriteLittleEndian(void* buffer, uint16_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[0];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
    }
}

void Endian::WriteLittleEndian(void* buffer, int32_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[0];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[3];
    }
}

void Endian::WriteLittleEndian(void* buffer, uint32_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[0];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[3];
    }
}

void Endian::WriteLittleEndian(void* buffer, int64_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[7];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[6];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[5];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[4];
        ((uint8_t*)buffer)[4] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[5] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[6] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[7] = ((const uint8_t*)&value)[0];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[4] = ((const uint8_t*)&value)[4];
        ((uint8_t*)buffer)[5] = ((const uint8_t*)&value)[5];
        ((uint8_t*)buffer)[6] = ((const uint8_t*)&value)[6];
        ((uint8_t*)buffer)[7] = ((const uint8_t*)&value)[7];
    }
}

void Endian::WriteLittleEndian(void* buffer, uint64_t value)
{
    if (IsBigEndian())
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[7];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[6];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[5];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[4];
        ((uint8_t*)buffer)[4] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[5] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[6] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[7] = ((const uint8_t*)&value)[0];
    }
    else
    {
        ((uint8_t*)buffer)[0] = ((const uint8_t*)&value)[0];
        ((uint8_t*)buffer)[1] = ((const uint8_t*)&value)[1];
        ((uint8_t*)buffer)[2] = ((const uint8_t*)&value)[2];
        ((uint8_t*)buffer)[3] = ((const uint8_t*)&value)[3];
        ((uint8_t*)buffer)[4] = ((const uint8_t*)&value)[4];
        ((uint8_t*)buffer)[5] = ((const uint8_t*)&value)[5];
        ((uint8_t*)buffer)[6] = ((const uint8_t*)&value)[6];
        ((uint8_t*)buffer)[7] = ((const uint8_t*)&value)[7];
    }
}

} // namespace CppCommon
