/*!
    \file system_source_location.cpp
    \brief Source location example
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#include "system/source_location.h"

#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Source location: " << __LOCATION__ << std::endl;
    return 0;
}
