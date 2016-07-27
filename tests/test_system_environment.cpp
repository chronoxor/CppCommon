//
// Created by Ivan Shynkarenka on 27.07.2016.
//

#include "catch.hpp"

#include "system/environment.h"

using namespace CppCommon;

TEST_CASE("Environment management", "[CppCommon][System]")
{
    REQUIRE((Environment::Is32BitOS() || Environment::Is64BitOS()));
    REQUIRE((Environment::Is32BitProcess() || Environment::Is64BitProcess()));
    REQUIRE((Environment::IsDebug() || Environment::IsRelease()));
    REQUIRE(Environment::OSVersion().length() > 0);
}
