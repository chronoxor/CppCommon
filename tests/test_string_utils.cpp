//
// Created by Ivan Shynkarenka on 15.05.2018
//

#include "test.h"

#include "string/string_utils.h"

using namespace CppCommon;

TEST_CASE("String utilities", "[CppCommon][String]")
{
    REQUIRE(StringUtils::ToLower("aBcDeFg") == "abcdefg");
    REQUIRE(StringUtils::ToUpper("aBcDeFg") == "ABCDEFG");
}
