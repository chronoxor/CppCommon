/*!
    \file memory_memory.cpp
    \brief Memory management example
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#include "memory/memory.h"

#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Total RAM: " << CppCommon::Memory::RamTotal() << " bytes" << std::endl;
    std::cout << "Free RAM: " << CppCommon::Memory::RamFree() << " bytes" << std::endl;
    return 0;
}
