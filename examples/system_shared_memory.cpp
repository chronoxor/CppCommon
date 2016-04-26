/*!
    \file system_shared_memory.cpp
    \brief Shared memory manager example
    \author Ivan Shynkarenka
    \date 19.04.2016
    \copyright MIT License
*/

#include "system/shared_memory.h"

#include <algorithm>
#include <cstring>
#include <iostream>

int main(int argc, char** argv)
{
    // Create or open a shared memory buffer
    CppCommon::SharedMemory buffer("shared_memory_example", 1024);
    if (buffer.owner())
        std::cout << "SharedMemory created!" << std::endl;
    else
        std::cout << "SharedMemory opened! Value = " << (char*)buffer.ptr() << std::endl;

    // Show help message
    std::cout << "Please enter anything to put into shared memory (several processes support). Enter '0' to exit..." << std::endl;

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        if (line == "0")
            break;

        // Show the old shared memory buffer content
        std::cout << "Old shared memory value = " << (char*)buffer.ptr() << std::endl;

        // Write text input into the shared memory buffer
        std::memcpy(buffer.ptr(), line.c_str(), std::min(line.size() + 1, buffer.size()));

        // Show the new shared memory buffer content
        std::cout << "New shared memory value = " << (char*)buffer.ptr() << std::endl;
    }

    return 0;
}
