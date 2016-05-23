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
    // Create or open a shared memory type wrapper
    CppCommon::SharedType<int> shared("shared_type_example");
    if (shared.owner())
        std::cout << "SharedType<int> created!" << std::endl;
    else
        std::cout << "SharedType<int> opened! Value = " << *shared << std::endl;

    // Show help message
    std::cout << "Please enter any integer number to put into shared memory type wrapper (several processes support). Enter '0' to exit..." << std::endl;

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        int item = std::stoi(line);

        // Check for exit
        if (item == 0)
            break;

        // Show the old shared memory type wrapper content
        std::cout << "Old shared memory type wrapper value = " << *shared << std::endl;

        // Write new integer value into the shared memory type wrapper
        *shared = item;

        // Show the new shared memory type wrapper content
        std::cout << "New shared memory type wrapper value = " << *shared << std::endl;
    }

    return 0;
}
