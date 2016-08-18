/*!
    \file system_uuid.cpp
    \brief Universally unique identifier (UUID) example
    \author Ivan Shynkarenka
    \date 19.08.2016
    \copyright MIT License
*/

#include "system/uuid.h"

#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "UUID::Generate(): " << CppCommon::UUID::Generate() << std::endl;
    return 0;
}
