/*!
    \file time_timestamp.cpp
    \brief Nanoseconds timestamp accessor example
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#include "time/timestamp.h"

#include <iostream>
#include <thread>

int main(int argc, char** argv)
{
    for (int i = 0; i < 100; ++i)
    {
        std::cout << "Timestamp value: " << CppCommon::timestamp() << std::endl;
        std::this_thread::yield();
    }

    return 0;
}
