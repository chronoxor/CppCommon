/*!
    \file filesystem_symlink.cpp
    \brief Filesystem symlink example
    \author Ivan Shynkarenka
    \date 04.09.2016
    \copyright MIT License
*/

#include "filesystem/symlink.h"

#include <iostream>

int main(int argc, char** argv)
{
    CppCommon::Symlink symlink("example.lnk");

    // Create a new symbolic link from the executable path
    CppCommon::Symlink::CreateSymlink(CppCommon::Path::executable(), symlink);

    // Read symbolic link target
    std::cout << "Symbolic link target: " << symlink.target() << std::endl;

    // Remove symbolic link
    CppCommon::Symlink::Remove(symlink);

    return 0;
}
