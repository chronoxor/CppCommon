/*!
    \file time_time.cpp
    \brief Time example
    \author Ivan Shynkarenka
    \date 13.07.2016
    \copyright MIT License
*/

#include "time/time.h"

#include <iostream>

void show(const CppCommon::Time& time)
{
    std::cout << "Time.year() = " << time.year() << std::endl;
    std::cout << "Time.month() = " << time.month() << std::endl;
    std::cout << "Time.day() = " << time.day() << std::endl;
    std::cout << "Time.hour() = " << time.hour() << std::endl;
    std::cout << "Time.minute() = " << time.minute() << std::endl;
    std::cout << "Time.second() = " << time.second() << std::endl;
    std::cout << "Time.millisecond() = " << time.millisecond() << std::endl;
    std::cout << "Time.microsecond() = " << time.microsecond() << std::endl;
    std::cout << "Time.nanosecond() = " << time.nanosecond() << std::endl;
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    std::cout << "UTC time:" << std::endl;
    show(CppCommon::UtcTime());

    std::cout << "Local time:" << std::endl;
    show(CppCommon::LocalTime());

    std::cout << "Min time:" << std::endl;
    show(CppCommon::UtcTime(CppCommon::Timestamp(0)));

    std::cout << "Max time:" << std::endl;
    show(CppCommon::UtcTime(CppCommon::Timestamp(0xFFFFFFFFFFFFFFFF)));

    return 0;
}
