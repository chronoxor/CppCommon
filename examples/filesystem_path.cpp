/*!
    \file filesystem_path.cpp
    \brief Filesystem path example
    \author Ivan Shynkarenka
    \date 17.08.2016
    \copyright MIT License
*/

#include "filesystem/path.h"

#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Path deprecated: " << CppCommon::Path::deprecated() << std::endl;
    std::cout << "Path separator: " << CppCommon::Path::separator() << std::endl;
    std::cout << "Initial path: " << CppCommon::Path::initial() << std::endl;
    std::cout << "Current path: " << CppCommon::Path::current() << std::endl;
    std::cout << "Executable path: " << CppCommon::Path::executable() << std::endl;
    std::cout << "Home path: " << CppCommon::Path::home() << std::endl;
    std::cout << "Temporary path: " << CppCommon::Path::temp() << std::endl;
    std::cout << "Unique filename: " << CppCommon::Path::unique() << std::endl;

    std::cout << std::endl;

    std::cout << "Filesystem capacity: " << CppCommon::Path::current().space().capacity << std::endl;
    std::cout << "Filesystem free: " << CppCommon::Path::current().space().free << std::endl;
    std::cout << "Filesystem available: " << CppCommon::Path::current().space().available << std::endl;

    return 0;
}
