/*!
    \file errors_exceptions.cpp
    \brief Exceptions handling example
    \author Ivan Shynkarenka
    \date 07.03.2016
    \copyright MIT License
*/

#include "errors/exceptions.h"

#include <iostream>

void GenerateSEH()
{
    int* p = nullptr;
#pragma warning(push)
#pragma warning(disable: 6011)
    *p = 0;
#pragma warning(pop)
}

void GenerateCustomException(int type, bool thread)
{
    switch (type)
    {
        case 0:
            break;
        case 1:
            GenerateSEH();
            break;
    }
}

int main(int argc, char** argv)
{
    std::cout << "0 - no exception" << std::endl;
    std::cout << "1 - SEH exception" << std::endl;
    std::cout << "2 - terminate" << std::endl;
    std::cout << "3 - unexpected" << std::endl;
    std::cout << "4 - pure virtual method call" << std::endl;
    std::cout << "5 - invalid parameter" << std::endl;
    std::cout << "6 - new operator fault" << std::endl;
    std::cout << "7 - throw C++ exception" << std::endl;
    std::cout << "8 - RaiseException()" << std::endl;
    std::cout << "9 - SIGABRT" << std::endl;
    std::cout << "10 - SIGFPE" << std::endl;
    std::cout << "11 - SIGILL" << std::endl;
    std::cout << "12 - SIGINT" << std::endl;
    std::cout << "13 - SIGSEGV" << std::endl;
    std::cout << "14 - SIGTERM" << std::endl;
    std::cout << "Choose an exception type: ";

    std::string line;
    if (getline(std::cin, line))
    {
        int type = std::stoi(line);
        GenerateCustomException(type, true);
    }

    return 0;
}
