/*!
    \file system_timestamp.cpp
    \brief Nanoseconds timestamp accessor example
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#include <iostream>
#include <thread>

#include "system/timestamp.h"

int main(int argc, char** argv)
{
    for (int i = 0; i < 100; ++i)
    {
        std::cout << "Timestamp value: " << CppCommon::timestamp() << std::endl;
        std::this_thread::yield();
    }

    return 0;
}
