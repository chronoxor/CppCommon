/*!
    \file common_flags.cpp
    \brief Enum-based flags example
    \author Ivan Shynkarenka
    \date 25.08.2016
    \copyright MIT License
*/

#include "common/flags.h"

#include <iostream>

enum class MyFlags
{
    None  = 0x0,
    One   = 0x1,
    Two   = 0x2,
    Three = 0x4,
    Four  = 0x8
};

// Register enum as flags to enable AND/OR/XOR logical operators with enum values!
ENUM_FLAGS(MyFlags)

int main(int argc, char** argv)
{
    auto mask = MyFlags::One | MyFlags::Two | MyFlags::Three;
    if (mask & MyFlags::One)
        std::cout << "MyFlags::One is set" << std::endl;
    if (mask & MyFlags::Two)
        std::cout << "MyFlags::Two is set" << std::endl;
    if (mask & MyFlags::Three)
        std::cout << "MyFlags::Three is set" << std::endl;
    if (mask & MyFlags::Four)
        std::cout << "MyFlags::Four is set" << std::endl;
    return 0;
}
