/*!
    \file time_timespan.cpp
    \brief Timespan example
    \author Ivan Shynkarenka
    \date 11.07.2016
    \copyright MIT License
*/

#include "time/timespan.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::cout << "Please enter timespan milliseconds as an integer number..." << std::endl;

    std::string line;
    getline(std::cin, line);
    int milliseconds = std::stoi(line);

    CppCommon::Timespan timespan = CppCommon::Timespan::milliseconds(milliseconds);

    std::cout << "Timespan.days() = " << timespan.days() << std::endl;
    std::cout << "Timespan.hours() = " << (timespan.hours() % 24) << std::endl;
    std::cout << "Timespan.minutes() = " << (timespan.minutes() % 60) << std::endl;
    std::cout << "Timespan.seconds() = " << (timespan.seconds() % 60) << std::endl;
    std::cout << "Timespan.milliseconds() = " << (timespan.milliseconds() % 1000) << std::endl;

    return 0;
}
