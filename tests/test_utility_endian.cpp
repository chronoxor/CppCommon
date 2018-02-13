//
// Created by Ivan Shynkarenka on 21.07.2017
//

#include "test.h"

#include "system/environment.h"
#include "utility/endian.h"

using namespace CppCommon;

TEST_CASE("Endian", "[CppCommon][Utility]")
{
    // Compile time returned endian value should be equal to calculated environment one!
    REQUIRE((Endian::IsBigEndian() == Environment::IsBigEndian()));
    REQUIRE((Endian::IsLittleEndian() == Environment::IsLittleEndian()));
}
