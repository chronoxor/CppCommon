/*!
    \file time_timestamp.cpp
    \brief Timestamp wrapper example
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#include "time/timestamp.h"

#include <iostream>
#include <thread>

int main(int argc, char** argv)
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Timestamp value: " << CppCommon::Timestamp::current() << std::endl;
        std::this_thread::yield();
    }

    for (int i = 0; i < 10; ++i)
    {
        std::cout << "RDTS value: " << CppCommon::Timestamp::rdts() << std::endl;
        std::this_thread::yield();
    }

    CppCommon::Timestamp timestamp;

    std::cout << "Timestamp.days() = " << timestamp.days() << std::endl;
    std::cout << "Timestamp.hours() = " << (timestamp.hours() % 24) << std::endl;
    std::cout << "Timestamp.minutes() = " << (timestamp.minutes() % 60) << std::endl;
    std::cout << "Timestamp.seconds() = " << (timestamp.seconds() % 60) << std::endl;
    std::cout << "Timestamp.milliseconds() = " << (timestamp.milliseconds() % 1000) << std::endl;

    return 0;
}
