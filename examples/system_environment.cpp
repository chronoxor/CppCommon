/*!
    \file system_environment.cpp
    \brief Environment management example
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#include "system/environment.h"

#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "32 bit OS: " << (CppCommon::Environment::Is32BitOS() ? "true" : "false") << std::endl;
    std::cout << "64 bit OS: " << (CppCommon::Environment::Is64BitOS() ? "true" : "false") << std::endl;
    std::cout << "32 bit process: " << (CppCommon::Environment::Is32BitProcess() ? "true" : "false") << std::endl;
    std::cout << "64 bit process: " << (CppCommon::Environment::Is64BitProcess() ? "true" : "false") << std::endl;
    std::cout << "Debug build: " << (CppCommon::Environment::IsDebug() ? "true" : "false") << std::endl;
    std::cout << "Release build: " << (CppCommon::Environment::IsRelease() ? "true" : "false") << std::endl;
    std::cout << "OS version: " << CppCommon::Environment::OSVersion() << std::endl;
    return 0;
}
