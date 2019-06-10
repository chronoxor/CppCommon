/*!
    \file encoding.cpp
    \brief Encoding utilities implementation
    \author Ivan Shynkarenka
    \date 12.08.2016
    \copyright MIT License
*/

#include "string/encoding.h"

#include <algorithm>
#include <cassert>
#include <codecvt>
#include <locale>
#include <vector>

namespace CppCommon {

std::string Encoding::ToUTF8(std::wstring_view wstr)
{
#if defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes((char16_t*)wstr.data(), (char16_t*)wstr.data() + wstr.size());
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    return convert.to_bytes(wstr.data(), wstr.data() + wstr.size());
#elif defined(_WIN32) || defined(_WIN64)
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.to_bytes(wstr.data(), wstr.data() + wstr.size());
#endif
}

std::wstring Encoding::FromUTF8(std::string_view str)
{
#if defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    auto tmp = convert.from_bytes(str.data(), str.data() + str.size());
    return std::wstring(tmp.data(), tmp.data() + tmp.size());
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    return convert.from_bytes(str.data(), str.data() + str.size());
#elif defined(_WIN32) || defined(_WIN64)
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.from_bytes(str.data(), str.data() + str.size());
#endif
}

std::u16string Encoding::UTF8toUTF16(std::string_view str)
{
#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t> convert;
    auto tmp = convert.from_bytes(str.data(), str.data() + str.size());
    return std::u16string(tmp.data(), tmp.data() + tmp.size());
#else
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.from_bytes(str.data(), str.data() + str.size());
#endif
}

std::u32string Encoding::UTF8toUTF32(std::string_view str)
{
#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> convert;
    auto tmp = convert.from_bytes(str.data(), str.data() + str.size());
    return std::u32string(tmp.data(), tmp.data() + tmp.size());
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.from_bytes(str.data(), str.data() + str.size());
#endif
}

std::string Encoding::UTF16toUTF8(std::u16string_view str)
{
#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t> convert;
    return convert.to_bytes((uint16_t*)str.data(), (uint16_t*)str.data() + str.size());
#else
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes(str.data(), str.data() + str.size());
#endif
}

std::u32string Encoding::UTF16toUTF32(std::u16string_view str)
{
    std::string bytes;
    bytes.reserve(str.size() * 2);

    for (const char16_t ch : str)
    {
        bytes.push_back((uint8_t)(ch / 256));
        bytes.push_back((uint8_t)(ch % 256));
    }

#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf16<uint32_t>, uint32_t> convert;
    auto tmp = convert.from_bytes(bytes);
    return std::u32string(tmp.data(), tmp.data() + tmp.size());
#else
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> convert;
    return convert.from_bytes(bytes);
#endif
}

std::string Encoding::UTF32toUTF8(std::u32string_view str)
{
#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> convert;
    return convert.to_bytes((uint32_t*)str.data(), (uint32_t*)str.data() + str.size());
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(str.data(), str.data() + str.size());
#endif
}

std::u16string Encoding::UTF32toUTF16(std::u32string_view str)
{
#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf16<uint32_t>, uint32_t> convert;
    std::string bytes = convert.to_bytes((uint32_t*)str.data(), (uint32_t*)str.data() + str.size());
#else
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> convert;
    std::string bytes = convert.to_bytes(str.data(), str.data() + str.size());
#endif

    std::u16string result;
    result.reserve(bytes.size() / 2);

    for (size_t i = 0; i < bytes.size(); i += 2)
        result.push_back((char16_t)((uint8_t)(bytes[i]) * 256 + (uint8_t)(bytes[i + 1])));

    return result;
}

std::string Encoding::Base16Encode(std::string_view str)
{
    const char base16[] = "0123456789ABCDEF";

    size_t ilength = str.length();
    size_t olength = ilength * 2;

    std::string result;
    result.resize(olength, 0);

    for (size_t i = 0, j = 0; i < ilength;)
    {
        uint8_t ch = (uint8_t)str[i++];

        result[j++] = base16[(ch & 0xF0) >> 4];
        result[j++] = base16[(ch & 0x0F) >> 0];
    }

    return result;
}

std::string Encoding::Base16Decode(std::string_view str)
{
    static const unsigned char base16[128] =
    {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

    size_t ilength = str.length();

    assert(((ilength % 2) == 0) && "Invalid Base16 sting!");
    if ((ilength % 2) != 0)
        return "";

    size_t olength = ilength / 2;

    std::string result;
    result.resize(olength, 0);

    for (size_t i = 0, j = 0; i < ilength;)
    {
        uint8_t a = (uint8_t)str[i++];
        uint8_t b = (uint8_t)str[i++];

        // Validate ASCII
        assert(((a < 0x80) && (b < 0x80)) && "Invalid Base16 content!");
        if ((a >= 0x80) || (b >= 0x80))
            return "";

        // Convert ASCII to Base16
        a = base16[a];
        b = base16[b];

        result[j++] = ((a << 4) | b);
    }

    return result;
}

std::string Encoding::Base32Encode(std::string_view str)
{
    const char base32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";

    size_t ilength = str.length();
    size_t olength = ((ilength / 5) * 8) + ((ilength % 5) ? 8 : 0);

    std::string result;
    result.resize(olength, 0);

    for (size_t i = 0, j = 0; i < ilength;)
    {
        size_t block = ((ilength - i) < 5 ? (ilength - i) : 5);
        uint8_t n1, n2, n3, n4, n5, n6, n7, n8;
        n1 = n2 = n3 = n4 = n5 = n6 = n7 = n8 = 0;

        switch (block)
        {
            case 5:
                n8  = (((uint8_t)str[i + 4] & 0x1F) >> 0);
                n7  = (((uint8_t)str[i + 4] & 0xE0) >> 5);
            case 4:
                n7 |= (((uint8_t)str[i + 3] & 0x03) << 3);
                n6  = (((uint8_t)str[i + 3] & 0x7C) >> 2);
                n5  = (((uint8_t)str[i + 3] & 0x80) >> 7);
            case 3:
                n5 |= (((uint8_t)str[i + 2] & 0x0F) << 1);
                n4  = (((uint8_t)str[i + 2] & 0xF0) >> 4);
            case 2:
                n4 |= (((uint8_t)str[i + 1] & 0x01) << 4);
                n3  = (((uint8_t)str[i + 1] & 0x3E) >> 1);
                n2  = (((uint8_t)str[i + 1] & 0xC0) >> 6);
            case 1:
                n2 |= (((uint8_t)str[i + 0] & 0x07) << 2);
                n1  = (((uint8_t)str[i + 0] & 0xF8) >> 3);
                break;
            default:
                assert(false && "Invalid Base32 operation!");
        }
        i += block;

        // Validate
        assert((n1 <= 31) && "Invalid Base32 n1 value!");
        assert((n2 <= 31) && "Invalid Base32 n2 value!" );
        assert((n3 <= 31) && "Invalid Base32 n3 value!" );
        assert((n4 <= 31) && "Invalid Base32 n4 value!" );
        assert((n5 <= 31) && "Invalid Base32 n5 value!" );
        assert((n6 <= 31) && "Invalid Base32 n6 value!" );
        assert((n7 <= 31) && "Invalid Base32 n7 value!" );
        assert((n8 <= 31) && "Invalid Base32 n8 value!" );

        // Padding
        switch (block)
        {
            case 1: n3 = n4 = 32;
            case 2: n5 = 32;
            case 3: n6 = n7 = 32;
            case 4: n8 = 32;
            case 5:
                break;
            default:
                assert(false && "Invalid Base32 operation!");
        }

        // 8 outputs
        result[j++] = base32[n1];
        result[j++] = base32[n2];
        result[j++] = base32[n3];
        result[j++] = base32[n4];
        result[j++] = base32[n5];
        result[j++] = base32[n6];
        result[j++] = base32[n7];
        result[j++] = base32[n8];
    }

    return result;
}

std::string Encoding::Base32Decode(std::string_view str)
{
    static const unsigned char base32[128] =
    {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0xFF, 0xFF,
        0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
        0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
        0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
        0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
        0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

    size_t ilength = str.length();

    assert(((ilength % 8) == 0) && "Invalid Base32 sting!");
    if ((ilength % 8) != 0)
        return "";

    size_t olength = (ilength / 8) * 5;

    std::string result;
    result.resize(olength, 0);

    for (size_t i = 0, j = 0; i < ilength;)
    {
        // 8 inputs
        uint8_t n1 = (uint8_t)str[i++];
        uint8_t n2 = (uint8_t)str[i++];
        uint8_t n3 = (uint8_t)str[i++];
        uint8_t n4 = (uint8_t)str[i++];
        uint8_t n5 = (uint8_t)str[i++];
        uint8_t n6 = (uint8_t)str[i++];
        uint8_t n7 = (uint8_t)str[i++];
        uint8_t n8 = (uint8_t)str[i++];

        // Validate ASCII
        assert(((n1 < 0x80) && (n2 < 0x80) && (n3 < 0x80) && (n4 < 0x80) && (n5 < 0x80) && (n6 < 0x80) && (n7 < 0x80) && (n8 < 0x80)) && "Invalid Base32 content!");
        if ((n1 >= 0x80) || (n2 >= 0x80) || (n3 >= 0x80) || (n4 >= 0x80) || (n5 >= 0x80) || (n6 >= 0x80) || (n7 >= 0x80) || (n8 >= 0x80))
            return "";

        // Convert ASCII to Base32
        n1 = base32[n1];
        n2 = base32[n2];
        n3 = base32[n3];
        n4 = base32[n4];
        n5 = base32[n5];
        n6 = base32[n6];
        n7 = base32[n7];
        n8 = base32[n8];

        // Validate Base32
        assert(((n1 <= 31) && (n2 <= 31)) && "Invalid Base32 content!");
        if ((n1 > 31) || (n2 > 31))
            return "";

        // The following can be padding
        assert(((n3 <= 32) && (n4 <= 32) && (n5 <= 32) && (n6 <= 32) && (n7 <= 32) && (n8 <= 32)) && "Invalid Base32 content!");
        if ((n3 > 32) || (n4 > 32) || (n5 > 32) || (n6 > 32) || (n7 > 32) || (n8 > 32))
            return "";

        // 5 outputs
        result[j++] = ((n1 & 0x1f) << 3) | ((n2 & 0x1c) >> 2);
        result[j++] = ((n2 & 0x03) << 6) | ((n3 & 0x1f) << 1) | ((n4 & 0x10) >> 4);
        result[j++] = ((n4 & 0x0f) << 4) | ((n5 & 0x1e) >> 1);
        result[j++] = ((n5 & 0x01) << 7) | ((n6 & 0x1f) << 2) | ((n7 & 0x18) >> 3);
        result[j++] = ((n7 & 0x07) << 5) | ((n8 & 0x1f));

        // Padding
        if (n8 == 32)
        {
            result.resize(result.size() - 1);
            assert((((n7 == 32) && (n6 == 32)) || (n7 != 32)) && "Invalid Base32 content!");
            if (n6 == 32)
            {
                result.resize(result.size() - 1);
                if (n5 == 32)
                {
                    result.resize(result.size() - 1);
                    assert((((n4 == 32) && (n3 == 32)) || (n4 != 32)) && "Invalid Base32 content!");
                    if (n3 == 32)
                    {
                        result.resize(result.size() - 1);
                    }
                }
            }
        }
    }

    return result;
}

std::string Encoding::Base64Encode(std::string_view str)
{
    const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const size_t mods[] = { 0, 2, 1 };

    size_t ilength = str.length();
    size_t olength = 4 * ((ilength + 2) / 3);

    std::string result;
    result.resize(olength, 0);

    for (size_t i = 0, j = 0; i < ilength;)
    {
        uint32_t octet_a = i < ilength ? (uint8_t)str[i++] : 0;
        uint32_t octet_b = i < ilength ? (uint8_t)str[i++] : 0;
        uint32_t octet_c = i < ilength ? (uint8_t)str[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        result[j++] = base64[(triple >> 3 * 6) & 0x3F];
        result[j++] = base64[(triple >> 2 * 6) & 0x3F];
        result[j++] = base64[(triple >> 1 * 6) & 0x3F];
        result[j++] = base64[(triple >> 0 * 6) & 0x3F];
    }

    for (size_t i = 0; i < mods[ilength % 3]; ++i)
        result[result.size() - 1 - i] = '=';

    return result;
}

std::string Encoding::Base64Decode(std::string_view str)
{
    static const unsigned char base64[256] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3f,
        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
        0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    size_t ilength = str.length();

    if (ilength % 4 != 0)
        return "";

    size_t olength = ilength / 4 * 3;

    if (str[ilength - 1] == '=') olength--;
    if (str[ilength - 2] == '=') olength--;

    std::string result;
    result.resize(olength, 0);

    for (size_t i = 0, j = 0; i < ilength;)
    {
        uint32_t sextet_a = str[i] == '=' ? 0 & i++ : base64[(uint8_t)str[i++]];
        uint32_t sextet_b = str[i] == '=' ? 0 & i++ : base64[(uint8_t)str[i++]];
        uint32_t sextet_c = str[i] == '=' ? 0 & i++ : base64[(uint8_t)str[i++]];
        uint32_t sextet_d = str[i] == '=' ? 0 & i++ : base64[(uint8_t)str[i++]];

        uint32_t triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

        if (j < olength) result[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < olength) result[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < olength) result[j++] = (triple >> 0 * 8) & 0xFF;

    }

    return result;
}

std::string Encoding::URLEncode(std::string_view str)
{
    std::string result;
    result.reserve(str.size());

    for (char ch : str)
    {
        if (isalnum(ch) || (ch == '-') || (ch == '.') || (ch == '/') || (ch == '_') || (ch == '~'))
            result.push_back(ch);
        else if (ch == ' ')
            result.push_back('+');
        else
        {
            const char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
            result.push_back('%');
            result.push_back(hex[(ch >> 4) & 0x0F]);
            result.push_back(hex[(ch >> 0) & 0x0F]);
        }
    }

    return result;
}

std::string Encoding::URLDecode(std::string_view str)
{
    std::string result;
    result.reserve(str.size());

    for (size_t i = 0; i < str.size(); ++i)
    {
        char ch = str[i];
        if (ch == '%')
        {
            const char hex[] = "0123456789ABCDEF";

            if (++i == str.size())
            {
                result.push_back('?');
                break;
            }

            int hi = (int)(std::find(hex, hex + 16, toupper(str[i])) - hex);

            if (++i == str.size())
            {
                result.push_back('?');
                break;
            }

            int lo = (int)(std::find(hex, hex + 16, toupper(str[i])) - hex);

            if ((hi >= 16) || (lo >= 16))
            {
                result.push_back('?');
                break;
            }

            result.push_back((char)((hi << 4) + lo));
        }
        else if (ch == '+')
            result.push_back(' ');
        else
            result.push_back(ch);
    }

    return result;
}

} // namespace CppCommon
