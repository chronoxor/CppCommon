//
// Created by Ivan Shynkarenka on 19.08.2016
//

#include "test.h"

#include "system/uuid.h"

using namespace CppCommon;

TEST_CASE("UUID common", "[CppCommon][System]")
{
    REQUIRE(UUID().string() == "00000000-0000-0000-0000-000000000000");
    REQUIRE(UUID("01234567-89ab-cdef-FEDC-BA9876543210").string() == "01234567-89AB-CDEF-FEDC-BA9876543210");
    REQUIRE(UUID("{01234567-89ab-cdef-FEDC-BA9876543210}").string() == "01234567-89AB-CDEF-FEDC-BA9876543210");
}

void test_uuid(const UUID& uuid)
{
    REQUIRE(uuid.data().size() == 16);

    std::string uuid_str = uuid.string();
    REQUIRE(uuid_str.size() == 36);
    REQUIRE(uuid_str[8] == '-');
    REQUIRE(uuid_str[13] == '-');
    REQUIRE(uuid_str[18] == '-');
    REQUIRE(uuid_str[23] == '-');

    std::wstring uuid_wstr = uuid.wstring();
    REQUIRE(uuid_wstr.size() == 36);
    REQUIRE(uuid_wstr[8] == L'-');
    REQUIRE(uuid_wstr[13] == L'-');
    REQUIRE(uuid_wstr[18] == L'-');
    REQUIRE(uuid_wstr[23] == L'-');
}

TEST_CASE("UUID generate", "[CppCommon][System]")
{
    test_uuid(UUID::Nil());
    test_uuid(UUID::Sequential());
    test_uuid(UUID::Random());
}
