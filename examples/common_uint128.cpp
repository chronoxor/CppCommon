/*!
    \file common_uint128.cpp
    \brief Unsigned 128-bit integer type example
    \author Ivan Shynkarenka
    \date 13.10.2016
    \copyright MIT License
*/

#include "common/uint128.h"

#include <iostream>

int main(int argc, char** argv)
{
    CppCommon::uint128_t a(0xABCDEFull);
    CppCommon::uint128_t b(0x12345ull, 0xFEDBCA9876543210ull);

    std::cout << a << " * " << b << " = " << (a * b) << std::endl;
    std::cout << b << " / " << a << " = " << (b / a) << std::endl;
    return 0;
}
