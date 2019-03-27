/*!
    \file threads_named_mutex.cpp
    \brief Named mutex synchronization primitive example
    \author Ivan Shynkarenka
    \date 15.04.2016
    \copyright MIT License
*/

#include "threads/named_mutex.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::string help = "Please enter '+' to lock and '-' to unlock the named mutex (several processes support). Enter '0' to exit...";

    // Show help message
    std::cout << help << std::endl;

    // Create named mutex
    CppCommon::NamedMutex mutex("named_mutex_example");

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        if (line == "+")
        {
            if (mutex.TryLock())
                std::cout << "Mutex successfully locked!" << std::endl;
            else
                std::cout << "Failed to lock mutex!" << std::endl;
        }
        else if (line == "-")
        {
            try
            {
                mutex.Unlock();
                std::cout << "Mutex successfully unlocked!" << std::endl;
            }
            catch (const CppCommon::SystemException&)
            {
                std::cout << "Failed to unlock mutex!" << std::endl;
            }
        }
        else if (line == "0")
            break;
        else
            std::cout << help << std::endl;
    }

    return 0;
}
