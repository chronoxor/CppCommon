/*!
    \file string_utils.cpp
    \brief String utilities example
    \author Ivan Shynkarenka
    \date 15.05.2018
    \copyright MIT License
*/

#include "string/string_utils.h"

#include <iostream>

int main(int argc, char** argv)
{
    std::cout << CppCommon::StringUtils::ToLower('A') << std::endl;
    std::cout << CppCommon::StringUtils::ToUpper('a') << std::endl;
    std::cout << CppCommon::StringUtils::ToLower("aBcDeFg") << std::endl;
    std::cout << CppCommon::StringUtils::ToUpper("aBcDeFg") << std::endl;

    std::string str;

    str = "  foobar  ";
    std::cout << '"' << CppCommon::StringUtils::LTrim(str) << '"' << std::endl;
    str = "  foobar  ";
    std::cout << '"' << CppCommon::StringUtils::RTrim(str) << '"' << std::endl;
    str = "  foobar  ";
    std::cout << '"' << CppCommon::StringUtils::Trim(str) << '"' << std::endl;

    str = "a foo a bar a baz";
    std::cout << CppCommon::StringUtils::Contains(str, '!') << std::endl;
    std::cout << CppCommon::StringUtils::Contains(str, 'z') << std::endl;
    std::cout << CppCommon::StringUtils::Contains(str, "foo") << std::endl;
    std::cout << CppCommon::StringUtils::Contains(str, "foobar") << std::endl;

    str = "a foo a bar a baz";
    std::cout << CppCommon::StringUtils::CountAll(str, "a ") << std::endl;

    str = "a foo a bar a baz";
    CppCommon::StringUtils::ReplaceFirst(str, "a ", "the ");
    std::cout << str << std::endl;

    str = "a foo a bar a baz";
    CppCommon::StringUtils::ReplaceLast(str, "a ", "the ");
    std::cout << str << std::endl;

    str = "a foo a bar a baz";
    CppCommon::StringUtils::ReplaceAll(str, "a ", "the ");
    std::cout << str << std::endl;

    str = "a foo a bar a baz";
    std::cout << CppCommon::StringUtils::Join(CppCommon::StringUtils::Split(str, ' '), '+') << std::endl;
    str = "a foo a bar a baz";
    std::cout << CppCommon::StringUtils::Join(CppCommon::StringUtils::Split(str, "a "), "the ") << std::endl;

    std::cout << CppCommon::StringUtils::ToString(true) << std::endl;
    std::cout << CppCommon::StringUtils::ToString(false) << std::endl;
    std::cout << CppCommon::StringUtils::ToString(100) << std::endl;
    std::cout << CppCommon::StringUtils::ToString(123.456) << std::endl;
    std::cout << CppCommon::StringUtils::FromString<bool>("true") << std::endl;
    std::cout << CppCommon::StringUtils::FromString<bool>("yes") << std::endl;
    std::cout << CppCommon::StringUtils::FromString<bool>("on") << std::endl;
    std::cout << CppCommon::StringUtils::FromString<bool>("1") << std::endl;
    std::cout << CppCommon::StringUtils::FromString<bool>("false") << std::endl;
    std::cout << CppCommon::StringUtils::FromString<bool>("no") << std::endl;
    std::cout << CppCommon::StringUtils::FromString<bool>("off") << std::endl;
    std::cout << CppCommon::StringUtils::FromString<bool>("0") << std::endl;
    std::cout << CppCommon::StringUtils::FromString<int>("100") << std::endl;
    std::cout << CppCommon::StringUtils::FromString<double>("123.456") << std::endl;

    return 0;
}
