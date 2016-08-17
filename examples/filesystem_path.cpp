/*!
    \file filesystem_path.cpp
    \brief Filesystem path wrapper example
    \author Ivan Shynkarenka
    \date 17.08.2016
    \copyright MIT License
*/

#include "filesystem/path.h"

#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Path separator: " << CppCommon::Path::separator() << std::endl;
    std::cout << "Initial path: " << CppCommon::Path::initial() << std::endl;
    std::cout << "Current path: " << CppCommon::Path::current() << std::endl;
    std::cout << "Executable path: " << CppCommon::Path::executable() << std::endl;
    std::cout << "Home path: " << CppCommon::Path::home() << std::endl;
    std::cout << "Temporary path: " << CppCommon::Path::temp() << std::endl;
    return 0;
}
