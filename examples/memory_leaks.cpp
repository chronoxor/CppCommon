/*!
    \file memory_leaks.cpp
    \brief Memory leaks detection example
    \author Ivan Shynkarenka
    \date 26.02.2018
    \copyright MIT License
*/

#include "memory/memory_leaks.h"

#include <iostream>

int main(int argc, char** argv)
{
    [[maybe_unused]] int* ptr = nullptr;

    // Make some memory leaks
    ptr = new int;
    ptr = new int[100];

    return 0;
}
