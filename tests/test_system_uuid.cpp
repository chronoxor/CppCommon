//
// Created by Ivan Shynkarenka on 19.08.2016.
//

#include "catch.hpp"

#include "system/uuid.h"

using namespace CppCommon;

TEST_CASE("UUID common", "[CppCommon][System]")
{
    REQUIRE(UUID().string() == "00000000-0000-0000-0000-000000000000");
    REQUIRE(UUID("01234567-89ab-cdef-FEDC-BA9876543210").string() == "01234567-89AB-CDEF-FEDC-BA9876543210");
    REQUIRE(UUID("{01234567-89ab-cdef-FEDC-BA9876543210}").string() == "01234567-89AB-CDEF-FEDC-BA9876543210");
}

TEST_CASE("UUID generate", "[CppCommon][System]")
{
    UUID uuid = UUID::Generate();
    REQUIRE(uuid.data().size() == 16);

    std::string uuid_str = uuid.string();
    REQUIRE(uuid_str.size() == 36);
    REQUIRE(uuid_str[8] == '-');
    REQUIRE(uuid_str[13] == '-');
    REQUIRE(uuid_str[18] == '-');
    REQUIRE(uuid_str[23] == '-');
}
