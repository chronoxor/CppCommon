/*!
    \file string_utils.cpp
    \brief String utilities implementation
    \author Ivan Shynkarenka
    \date 15.05.2016
    \copyright MIT License
*/

#include "string/string_utils.h"

#include <algorithm>

namespace CppCommon {

std::string StringUtils::ToLower(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ToLowerInternal);
    return result;
}

std::string StringUtils::ToUpper(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ToUpperInternal);
    return result;
}

} // namespace CppCommon
