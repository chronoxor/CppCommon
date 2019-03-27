/*!
    \file threads_named_rw_lock.cpp
    \brief Named read/write lock synchronization primitive example
    \author Ivan Shynkarenka
    \date 25.05.2016
    \copyright MIT License
*/

#include "threads/named_rw_lock.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::string help = "Please enter '+' or '*' to read/write lock and '-' or '/' to read/write unlock the named lock (several processes support). Enter '0' to exit...";

    // Show help message
    std::cout << help << std::endl;

    // Create named read/write lock
    CppCommon::NamedRWLock lock("named_rw_lock_example");

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        if (line == "+")
        {
            if (lock.TryLockRead())
                std::cout << "Successfully locked for read!" << std::endl;
            else
                std::cout << "Failed to lock for read!" << std::endl;
        }
        else if (line == "*")
        {
            if (lock.TryLockWrite())
                std::cout << "Successfully locked for write!" << std::endl;
            else
                std::cout << "Failed to lock for write!" << std::endl;
        }
        else if (line == "-")
        {
            try
            {
                lock.UnlockRead();
                std::cout << "Successfully unlocked reader!" << std::endl;
            }
            catch (const CppCommon::SystemException&)
            {
                std::cout << "Failed to unlock reader!" << std::endl;
            }
        }
        else if (line == "/")
        {
            try
            {
                lock.UnlockWrite();
                std::cout << "Successfully unlocked writer!" << std::endl;
            }
            catch (const CppCommon::SystemException&)
            {
                std::cout << "Failed to unlock writer!" << std::endl;
            }
        }
        else if (line == "0")
            break;
        else
            std::cout << help << std::endl;
    }

    return 0;
}
