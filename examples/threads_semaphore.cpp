/*!
    \file threads_semaphore.cpp
    \brief Semaphore synchronization primitive example
    \author Ivan Shynkarenka
    \date 12.04.2016
    \copyright MIT License
*/

#include "threads/semaphore.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::string help = "Please enter '+' to lock and '-' to unlock the semaphore. Enter '0' to exit...";

    // Show help message
    std::cout << help << std::endl;

    // Assume we have four resources
    int resources = 4;

    // Create semaphore for our resources
    CppCommon::Semaphore semaphore(resources);

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        if (line == "+")
        {
            if (semaphore.TryLock())
                std::cout << "Semaphore successfully locked!" << std::endl;
            else
                std::cout << "Failed to lock semaphore! Semaphore resources exceeded..." << std::endl;
        }
        else if (line == "-")
        {
            try
            {
                semaphore.Unlock();
                std::cout << "Semaphore successfully unlocked!" << std::endl;
            }
            catch (const CppCommon::SystemException&)
            {
                std::cout << "Failed to unlock semaphore! Semaphore is fully unlocked..." << std::endl;
            }
        }
        else if (line == "0")
            break;
        else
            std::cout << help << std::endl;
    }

    return 0;
}
