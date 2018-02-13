//
// Created by Ivan Shynkarenka on 09.02.2016
//

#include "test.h"

#include "system/source_location.h"

using namespace CppCommon;

TEST_CASE("Source location", "[CppCommon][System]")
{
    auto location = __LOCATION__;

    REQUIRE(location.filename() != nullptr);
    REQUIRE(location.filename() == std::string(__FILE__));
    REQUIRE(location.line() == 13);

    REQUIRE(__LOCATION__.string() == (std::string(__FILE__) + ':' + std::to_string(__LINE__)));
}
