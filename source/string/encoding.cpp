/*!
    \file encoding.cpp
    \brief Encoding utilities implementation
    \author Ivan Shynkarenka
    \date 12.08.2016
    \copyright MIT License
*/

#include "string/encoding.h"

#include <algorithm>
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

std::string Encoding::Base64Encode(std::string_view str)
{
    const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;

    int val = 0;
    int valb = -6;
    for (auto c : str)
    {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            result.push_back(base64[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if (valb > -6)
        result.push_back(base64[((val << 8) >> (valb + 8)) & 0x3F]);

    while (result.size() % 4)
        result.push_back('=');

    return result;
}

std::string Encoding::Base64Decode(std::string_view str)
{
    const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;

    std::vector<int> pattern(256, -1);
    for (int i = 0; i < 64; ++i)
        pattern[base64[i]] = i;

    int val = 0;
    int valb = -8;
    for (auto c : str)
    {
        if (pattern[c] == -1)
            break;

        val = (val << 6) + pattern[c];
        valb += 6;

        if (valb >= 0)
        {
            result.push_back((uint8_t)((val >> valb) & 0xFF));
            valb -= 8;
        }
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
