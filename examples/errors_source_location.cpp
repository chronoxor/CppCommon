/*!
    \file errors_source_location.cpp
    \brief Source location wrapper example
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#include <iostream>

#include "errors/source_location.h"

int main(int argc, char** argv)
{
    std::cout << "Source location: " << __LOCATION__ << std::endl;
    return 0;
}
