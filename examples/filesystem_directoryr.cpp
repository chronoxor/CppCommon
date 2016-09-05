/*!
    \file filesystem_directory.cpp
    \brief Filesystem directory wrapper example
    \author Ivan Shynkarenka
    \date 05.09.2016
    \copyright MIT License
*/

#include "filesystem/directory.h"

#include <iostream>

int main(int argc, char** argv)
{
    // Get the temp directory
    CppCommon::Directory temp("temp");

    // Iterate through all directory entries
    for (auto entry = temp.rbegin(); entry != temp.rend(); ++entry)
        std::cout << entry->native() << std::endl;

    return 0;
}
