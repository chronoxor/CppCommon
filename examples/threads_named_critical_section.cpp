/*!
    \file threads_critical_section.cpp
    \brief Named critical section synchronization primitive example
    \author Ivan Shynkarenka
    \date 26.05.2016
    \copyright MIT License
*/

#include "threads/named_critical_section.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::string help = "Please enter '+' to lock and '-' to unlock the named critical section (several processes support). Enter '0' to exit...";

    // Show help message
    std::cout << help << std::endl;

    // Create named critical section
    CppCommon::NamedCriticalSection lock("named_cs_example");

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        if (line == "+")
        {
            if (lock.TryLock())
                std::cout << "Critical section successfully locked!" << std::endl;
            else
                std::cout << "Failed to lock critical section!" << std::endl;
        }
        else if (line == "-")
        {
            try
            {
                lock.Unlock();
                std::cout << "Critical section successfully unlocked!" << std::endl;
            }
            catch (const CppCommon::SystemException&)
            {
                std::cout << "Failed to unlock critical section!" << std::endl;
            }
        }
        else if (line == "0")
            break;
        else
            std::cout << help << std::endl;
    }

    return 0;
}
