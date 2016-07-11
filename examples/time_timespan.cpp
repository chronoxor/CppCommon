/*!
    \file time_timespan.cpp
    \brief Time span example
    \author Ivan Shynkarenka
    \date 11.07.2016
    \copyright MIT License
*/

#include "time/timespan.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::cout << "Please enter time span milliseconds as an integer number..." << std::endl;

    std::string line;
    getline(std::cin, line);
    int milliseconds = std::stoi(line);

    CppCommon::TimeSpan span(milliseconds * 1000000ll);

    std::cout << "span.days() = " << span.days() << std::endl;
    std::cout << "span.hours() = " << span.hours() << std::endl;
    std::cout << "span.minutes() = " << span.minutes() << std::endl;
    std::cout << "span.seconds() = " << span.seconds() << std::endl;
    std::cout << "span.milliseconds() = " << span.milliseconds() << std::endl;

    return 0;
}
