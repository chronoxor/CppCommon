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
    std::cout << "Path separator: " << CppCommon::Path::Separator() << std::endl;
    std::cout << "Executable path: " << CppCommon::Path::Executable() << std::endl;
    return 0;
}
