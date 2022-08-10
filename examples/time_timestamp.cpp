/*!
    \file time_timestamp.cpp
    \brief Timestamp example
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#include "time/timestamp.h"

#include <iostream>
#include <thread>

void show(const CppCommon::Timestamp& timestamp)
{
    std::cout << "Timestamp.days() = " << timestamp.days() << std::endl;
    std::cout << "Timestamp.hours() = " << (timestamp.hours() % 24) << std::endl;
    std::cout << "Timestamp.minutes() = " << (timestamp.minutes() % 60) << std::endl;
    std::cout << "Timestamp.seconds() = " << (timestamp.seconds() % 60) << std::endl;
    std::cout << "Timestamp.milliseconds() = " << (timestamp.milliseconds() % 1000) << std::endl;
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "UTC value: " << CppCommon::Timestamp::utc() << std::endl;
        std::cout << "Local value: " << CppCommon::Timestamp::local() << std::endl;
        std::cout << "Nano value: " << CppCommon::Timestamp::nano() << std::endl;
        std::cout << "RDTS value: " << CppCommon::Timestamp::rdts() << std::endl;
        std::this_thread::yield();
    }
    std::cout << std::endl;

    std::cout << "Epoch timestamp:" << std::endl;
    show(CppCommon::EpochTimestamp());

    std::cout << "UTC timestamp:" << std::endl;
    show(CppCommon::UtcTimestamp());

    std::cout << "Local timestamp:" << std::endl;
    show(CppCommon::LocalTimestamp());

    std::cout << "Nano timestamp:" << std::endl;
    show(CppCommon::NanoTimestamp());

    std::cout << "RDTS timestamp:" << std::endl;
    show(CppCommon::RdtsTimestamp());

    return 0;
}
