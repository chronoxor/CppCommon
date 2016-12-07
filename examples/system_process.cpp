/*!
    \file system_process.cpp
    \brief Process example
    \author Ivan Shynkarenka
    \date 01.12.2016
    \copyright MIT License
*/

#include "system/process.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        std::cout << "Executing child process..." << std::endl;
        CppCommon::Process child = CppCommon::Process::Execute(argv[1]);
        int result = child.Wait();
        std::cout << "Executed child process! Result = " << result << std::endl;
        return 0;
    }

    std::cout << "Current process Id: " << CppCommon::Process::CurrentProcessId() << std::endl;
    std::cout << "Parent process Id: " << CppCommon::Process::ParentProcessId() << std::endl;
    return 123;
}
