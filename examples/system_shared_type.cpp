/*!
    \file system_shared_type.cpp
    \brief Shared memory type wrapper example
    \author Ivan Shynkarenka
    \date 23.05.2016
    \copyright MIT License
*/

#include "system/shared_type.h"

#include <algorithm>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    // Create or open a shared memory string wrapper
    CppCommon::SharedType<std::string> shared("shared_type_example");
    if (shared.owner())
        std::cout << "SharedType<std::string> created!" << std::endl;
    else
        std::cout << "SharedType<std::string> opened! Value = " << *shared << std::endl;

    // Show help message
    std::cout << "Please enter any string to put into shared memory string wrapper (several processes support). Enter '0' to exit..." << std::endl;

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        if (line == "0")
            break;

        // Show the old shared memory string wrapper content
        std::cout << "Old shared memory string wrapper value = " << *shared << std::endl;

        // Write text input into the shared memory string wrapper
        *shared = line;

        // Show the new shared memory string wrapper content
        std::cout << "New shared memory string wrapper value = " << *shared << std::endl;
    }

    return 0;
}
