/*!
    \file time_rdts.cpp
    \brief Read Time Stamp Counter example
    \author Ivan Shynkarenka
    \date 22.01.2016
    \copyright MIT License
*/

#include "time/rdts.h"

#include <iostream>
#include <thread>

int main(int argc, char** argv)
{
    for (int i = 0; i < 100; ++i)
    {
        std::cout << "RDTS value: " << CppCommon::rdts() << std::endl;
        std::this_thread::yield();
    }

    return 0;
}
