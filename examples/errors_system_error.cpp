/*!
    \file errors_system_error.cpp
    \brief System error wrapper example
    \author Ivan Shynkarenka
    \date 10.02.2016
    \copyright MIT License
*/

#include "errors/system_error.h"

#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Last system error: " << CppCommon::SystemError::GetLast() << std::endl;
    std::cout << "Last system message: " << CppCommon::SystemError::Convert(CppCommon::SystemError::GetLast()) << std::endl;
    return 0;
}
