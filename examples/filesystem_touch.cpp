/*!
    \file filesystem_touch.cpp
    \brief Filesystem touch example
    \author Ivan Shynkarenka
    \date 21.03.2019
    \copyright MIT License
*/

#include "filesystem/path.h"
#include "time/timestamp.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Touch path is required!" << std::endl;
        return -1;
    }

    CppCommon::Path::Touch(argv[1]);
    return 0;
}
