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
    std::cout << CppCommon::StringUtils::ToLower("aBcDeFg") << std::endl;
    std::cout << CppCommon::StringUtils::ToUpper("aBcDeFg") << std::endl;
    return 0;
}
