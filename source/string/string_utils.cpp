/*!
    \file string_utils.cpp
    \brief String utilities implementation
    \author Ivan Shynkarenka
    \date 15.05.2016
    \copyright MIT License
*/

#include "string/string_utils.h"

#include <cassert>
#include <regex>

namespace CppCommon {

bool StringUtils::IsBlank(const char* str)
{
    for (size_t i = 0; str[i] != 0; ++i)
        if (!IsBlank(str[i]))
            return false;

    return true;
}

bool StringUtils::IsBlank(std::string_view str)
{
    if (str.empty())
        return true;

    for (auto ch : str)
        if (!IsBlank(ch))
            return false;

    return true;
}

bool StringUtils::IsPatternMatch(const std::string& patterns, const std::string& str)
{
    bool result = false;
    auto keys = Split(patterns, ';');
    for (const std::string& key : keys)
    {
        bool negative = StartsWith(key, "!");
        std::string pattern = negative ? key.substr(1) : key;

        // Try to match regex pattern
        try
        {
            std::regex expression(pattern);
            bool matched = std::regex_match(str, expression);
            if (matched)
                return !negative;
        }
        catch (const std::regex_error&) {}

        // Last negative pattern should success result
        result = negative;
    }
    return result;
}

std::string StringUtils::ToLTrim(std::string_view str)
{
    return std::string(std::find_if(str.begin(), str.end(), [](int c) { return !std::isspace(c); }), str.end());
}

std::string StringUtils::ToRTrim(std::string_view str)
{
    return std::string(str.begin(), std::find_if(str.rbegin(), str.rend(), [](int c) { return !std::isspace(c); }).base());
}

std::string StringUtils::ToTrim(std::string_view str)
{
    auto start = std::find_if(str.begin(), str.end(), [](int c) { return !std::isspace(c); });
    auto end = std::find_if(str.rbegin(), str.rend(), [](int c) { return !std::isspace(c); }).base();

    return (start != str.end()) ? std::string(start, end) : std::string();
}

std::string& StringUtils::LTrim(std::string& str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int c) { return !std::isspace(c); }));
    return str;
}

std::string& StringUtils::RTrim(std::string& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int c) { return !std::isspace(c); }).base(), str.end());
    return str;
}

bool StringUtils::Compare(std::string_view str1, std::string_view str2)
{
    return (str1 == str2);
}

bool StringUtils::CompareNoCase(std::string_view str1, std::string_view str2)
{
    if (str1.size() != str2.size())
        return false;
    return std::equal(str1.cbegin(), str1.cend(), str2.cbegin(), [](std::string::value_type l, std::string::value_type r) { return std::tolower(l) == std::tolower(r); });
}

size_t StringUtils::CountAll(std::string_view str, std::string_view substr)
{
    size_t count=0;

    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != std::string::npos)
    {
        pos += substr.size();
        ++count;
    }

    return count;
}

bool StringUtils::ReplaceFirst(std::string& str, std::string_view substr, std::string_view with)
{
    size_t pos = str.find(substr);
    if (pos == std::string::npos)
        return false;

    str.replace(pos, substr.size(), with);
    return true;
}

bool StringUtils::ReplaceLast(std::string& str, std::string_view substr, std::string_view with)
{
    size_t pos = str.rfind(substr);
    if (pos == std::string::npos)
        return false;

    str.replace(pos, substr.size(), with);
    return true;
}

bool StringUtils::ReplaceAll(std::string& str, std::string_view substr, std::string_view with)
{
    bool result = false;

    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != std::string::npos)
    {
        str.replace(pos, substr.size(), with);
        pos += with.size();
        result = true;
    }

    return result;
}

std::vector<std::string> StringUtils::Split(std::string_view str, char delimiter, bool skip_empty)
{
    std::vector<std::string> tokens;

    size_t pos_current;
    size_t pos_last = 0;
    size_t length;

    while (true)
    {
        pos_current = str.find(delimiter, pos_last);
        if (pos_current == std::string::npos)
            pos_current = str.size();

        length = pos_current - pos_last;
        if (!skip_empty || (length != 0))
            tokens.emplace_back(str.substr(pos_last, length));

        if (pos_current == str.size())
            break;
        else
            pos_last = pos_current + 1;
    }

    return tokens;
}

std::vector<std::string> StringUtils::Split(std::string_view str, std::string_view delimiter, bool skip_empty)
{
    std::vector<std::string> tokens;

    size_t pos_current;
    size_t pos_last = 0;
    size_t length;

    while (true)
    {
        pos_current = str.find(delimiter, pos_last);
        if (pos_current == std::string::npos)
            pos_current = str.size();

        length = pos_current - pos_last;
        if (!skip_empty || (length != 0))
            tokens.emplace_back(str.substr(pos_last, length));

        if (pos_current == str.size())
            break;
        else
            pos_last = pos_current + delimiter.size();
    }

    return tokens;
}

std::vector<std::string> StringUtils::SplitByAny(std::string_view str, std::string_view delimiters, bool skip_empty)
{
    std::vector<std::string> tokens;

    size_t pos_current;
    size_t pos_last = 0;
    size_t length;

    while (true)
    {
        pos_current = str.find_first_of(delimiters, pos_last);
        if (pos_current == std::string::npos)
            pos_current = str.size();

        length = pos_current - pos_last;
        if (!skip_empty || (length != 0))
            tokens.emplace_back(str.substr(pos_last, length));

        if (pos_current == str.size())
            break;
        else
            pos_last = pos_current + 1;
    }

    return tokens;
}

std::string StringUtils::Join(const std::vector<std::string>& tokens, bool skip_empty, bool skip_blank)
{
    if (tokens.empty())
        return "";

    std::ostringstream result;

    for (size_t i = 0; i < tokens.size(); ++i)
        if (!((skip_empty && tokens[i].empty()) || (skip_blank && IsBlank(tokens[i]))))
            result << tokens[i];

    return result.str();
}

std::string StringUtils::Join(const std::vector<std::string>& tokens, char delimiter, bool skip_empty, bool skip_blank)
{
    if (tokens.empty())
        return "";

    std::ostringstream result;

    for (size_t i = 0; i < tokens.size() - 1; ++i)
        if (!((skip_empty && tokens[i].empty()) || (skip_blank && IsBlank(tokens[i]))))
            result << tokens[i] << delimiter;

    if (!((skip_empty && tokens[tokens.size() - 1].empty()) || (skip_blank && IsBlank(tokens[tokens.size() - 1]))))
        result << tokens[tokens.size() - 1];

    return result.str();
}

std::string StringUtils::Join(const std::vector<std::string>& tokens, const char* delimiter, bool skip_empty, bool skip_blank)
{
    if (tokens.empty())
        return "";

    std::ostringstream result;

    for (size_t i = 0; i < tokens.size() - 1; ++i)
        if (!((skip_empty && tokens[i].empty()) || (skip_blank && IsBlank(tokens[i]))))
            result << tokens[i] << delimiter;

    if (!((skip_empty && tokens[tokens.size() - 1].empty()) || (skip_blank && IsBlank(tokens[tokens.size() - 1]))))
        result << tokens[tokens.size() - 1];

    return result.str();
}

std::string StringUtils::Join(const std::vector<std::string>& tokens, std::string_view delimiter, bool skip_empty, bool skip_blank)
{
    if (tokens.empty())
        return "";

    std::ostringstream result;

    for (size_t i = 0; i < tokens.size() - 1; ++i)
        if (!((skip_empty && tokens[i].empty()) || (skip_blank && IsBlank(tokens[i]))))
            result << tokens[i] << delimiter;

    if (!((skip_empty && tokens[tokens.size() - 1].empty()) || (skip_blank && IsBlank(tokens[tokens.size() - 1]))))
        result << tokens[tokens.size() - 1];

    return result.str();
}

template <>
bool StringUtils::FromString(std::string_view str)
{
    std::string value = ToLower(str);
    if ((value == "true") || (value == "yes") || (value == "on") || (value == "1"))
        return true;
    if ((value == "false") || (value == "no") || (value == "off") || (value == "0"))
        return false;

    assert("Invalid boolean value represented in string!");
    return false;
}

} // namespace CppCommon
