/*!
    \file system_console.cpp
    \brief Console management example
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#include "system/console.h"

#include <iostream>
#include <iomanip>

int main(int argc, char** argv)
{
    for (int i = (int)CppCommon::Color::BLACK; i <= (int)CppCommon::Color::WHITE; ++i)
    {
        for (int j = (int)CppCommon::Color::BLACK; j <= (int)CppCommon::Color::WHITE; ++j)
        {
            std::cout << std::make_pair((CppCommon::Color)i, (CppCommon::Color)j);
            std::cout << " ";
            std::cout << std::setfill('0') << std::setw(2) << i;
            std::cout << " / ";
            std::cout << std::setfill('0') << std::setw(2) << j;
            std::cout << " ";
            CppCommon::Console::SetColor(CppCommon::Color::WHITE, CppCommon::Color::BLACK);
        }
        std::cout << std::endl;
    }
    return 0;
}
