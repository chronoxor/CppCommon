//
// Created by Ivan Shynkarenka on 09.02.2016.
//

#include "catch.hpp"

#include "system/source_location.h"

using namespace CppCommon;

TEST_CASE("Source location wrapper", "[CppCommon][System]")
{
    auto location = __LOCATION__;

    REQUIRE(location.filename() != nullptr);
    REQUIRE(location.filename() == std::string(__FILE__));
    REQUIRE(location.line() == 13);

    REQUIRE(__LOCATION__.to_string() == (std::string(__FILE__) + ':' + std::to_string(__LINE__)));
}
