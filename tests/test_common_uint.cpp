//
// Created by Ivan Shynkarenka on 11.10.2017
//

#include "catch.hpp"

#include "common/uint256.h"

using namespace CppCommon;

TEST_CASE("uint256: Constructors simple", "[CppCommon][Common]")
{
    uint256_t value(0x0123456789ABCDEFull);
    const uint256_t original = value;

    REQUIRE(uint256_t() == 0);
    REQUIRE(value == original);
    REQUIRE(uint256_t(std::move(value)) == original);
}

TEST_CASE("uint256: Constructors one", "[CppCommon][Common]")
{
    REQUIRE(uint256_t(true).upper() == false);
    REQUIRE(uint256_t(true).lower() == true);
    REQUIRE(uint256_t(false).upper() == false);
    REQUIRE(uint256_t(false).lower() == false);

    REQUIRE(uint256_t((uint8_t)0x01ull).upper() == 0ull);
    REQUIRE(uint256_t((uint16_t)0x0123ull).upper() == 0ull);
    REQUIRE(uint256_t((uint32_t)0x01234567ull).upper() == 0ull);
    REQUIRE(uint256_t((uint64_t)0x0123456789ABCDEFull).upper() == 0ull);

    REQUIRE(uint256_t((uint8_t)0x01ull).lower() == (uint8_t)0x01ull);
    REQUIRE(uint256_t((uint16_t)0x0123ull).lower() == (uint16_t)0x0123ull);
    REQUIRE(uint256_t((uint32_t)0x01234567ull).lower() == (uint32_t)0x01234567ull);
    REQUIRE(uint256_t((uint64_t)0x0123456789ABCDEFull).lower() == (uint64_t)0x0123456789ABCDEFull);
}

TEST_CASE("uint256: Constructors two", "[CppCommon][Common]")
{
    for (uint8_t hi = 0; hi < 2; ++hi)
    {
        for (uint8_t lo = 0; lo < 2; ++lo)
        {
            const uint256_t val(hi, lo);
            REQUIRE(val.upper() == hi);
            REQUIRE(val.lower() == lo);
        }
    }

    REQUIRE(uint256_t((uint8_t)0x01ull, (uint8_t)0x01ull).upper() == (uint8_t)0x01ull);
    REQUIRE(uint256_t((uint16_t)0x0123ull, (uint16_t)0x0123ull).upper() == (uint16_t)0x0123ull);
    REQUIRE(uint256_t((uint32_t)0x01234567ull, (uint32_t)0x01234567ull).upper() == (uint32_t)0x01234567ull);
    REQUIRE(uint256_t((uint64_t)0x0123456789ABCDEFull, (uint64_t)0x0123456789ABCDEFull).upper() == (uint64_t)0x0123456789ABCDEFull);

    REQUIRE(uint256_t((uint8_t)0x01ull, (uint8_t)0x01ull).lower() == (uint8_t)0x01ull);
    REQUIRE(uint256_t((uint16_t)0x0123ull, (uint16_t)0x0123ull).lower() == (uint16_t)0x0123ull);
    REQUIRE(uint256_t((uint32_t)0x01234567ull, (uint32_t)0x01234567ull).lower() == (uint32_t)0x01234567ull);
    REQUIRE(uint256_t((uint64_t)0x0123456789ABCDEFull, (uint64_t)0x0123456789ABCDEFull).lower() == (uint64_t)0x0123456789ABCDEFull);
}

TEST_CASE("uint256: Constructors four", "[CppCommon][Common]")
{
    for(uint8_t hi_hi = 0; hi_hi < 2; ++hi_hi)
    {
        for(uint8_t hi_lo = 0; hi_lo < 2; ++hi_lo)
        {
            for(uint8_t lo_hi = 0; lo_hi < 2; ++lo_hi)
            {
                for(uint8_t lo_lo = 0; lo_lo < 2; ++lo_lo)
                {
                    const uint256_t val(hi_hi, hi_lo, lo_hi, lo_lo);
                    REQUIRE(val.upper().upper() == hi_hi);
                    REQUIRE(val.upper().lower() == hi_lo);
                    REQUIRE(val.lower().upper() == lo_hi);
                    REQUIRE(val.lower().lower() == lo_lo);
                }
            }
        }
    }
}
