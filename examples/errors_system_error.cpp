/*!
    \file errors_system_error.cpp
    \brief System error example
    \author Ivan Shynkarenka
    \date 10.02.2016
    \copyright MIT License
*/

#include "errors/system_error.h"

#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Last system error: " << CppCommon::SystemError::GetLast() << std::endl;
    std::cout << "Last system description: " << CppCommon::SystemError::Description() << std::endl;
    return 0;
}
