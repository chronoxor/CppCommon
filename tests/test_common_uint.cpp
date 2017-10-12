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

TEST_CASE("uint256: Assignment", "[CppCommon][Common]")
{
    const uint256_t t_1 = true;
    const uint256_t f_1 = false;
    const uint256_t u8_1 = 0x01;
    const uint256_t u16_1 = 0x0123;
    const uint256_t u32_1 = 0x01234567;
    const uint256_t u64_1 = 0x0123456789ABCDEF;

    uint256_t t_2 = 0;
    uint256_t f_2 = 0;
    uint256_t u8_2 = 0;
    uint256_t u16_2 = 0;
    uint256_t u32_2 = 0;
    uint256_t u64_2 = 0;

    t_2 = t_1;
    f_2 = f_1;
    u8_2 = u8_1;
    u16_2 = u16_1;
    u32_2 = u32_1;
    u64_2 = u64_1;

    REQUIRE(t_1 == t_2);
    REQUIRE(f_1 == f_2);
    REQUIRE(u8_1 == u8_2);
    REQUIRE(u16_1 == u16_2);
    REQUIRE(u32_1 == u32_2);
    REQUIRE(u64_1 == u64_2);
}

TEST_CASE("uint256: Add", "[CppCommon][Common]")
{
    uint256_t low(0, 1);
    uint256_t high(1, 0);

    REQUIRE(low + low == 2);
    REQUIRE(low + high == uint256_t(1, 1));
    REQUIRE(high + high == uint256_t(2, 0));

    REQUIRE((low += low) == 2);
    REQUIRE((low += high) == uint256_t(1, 2));
    REQUIRE((high += low) == uint256_t(2, 2));
}

TEST_CASE("uint256: Add external", "[CppCommon][Common]")
{
    bool t = true;
    bool f = false;
    uint8_t u8 = 0xAAull;
    uint16_t u16 = 0xAAAAull;
    uint32_t u32 = 0xAAAAAAAAull;
    uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;
    uint128_t u128(0xAAAAAAAAAAAAAAAAull, 0xAAAAAAAAAAAAAAAAull);

    const uint256_t val(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull);

    REQUIRE(t + val == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull));
    REQUIRE(f + val == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull));
    REQUIRE(u8 + val == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F19Aull));
    REQUIRE(u16 + val == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F19B9Aull));
    REQUIRE(u32 + val == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F19B9B9B9Aull));
    REQUIRE(u64 + val == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull, 0x9B9B9B9B9B9B9B9Aull));
    REQUIRE(u128 + val == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull, 0x9B9B9B9B9B9B9B9Bull, 0x9B9B9B9B9B9B9B9Aull));

    REQUIRE(val + t == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull));
    REQUIRE(val + f == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull));
    REQUIRE(val + u8 == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F19Aull));
    REQUIRE(val + u16 == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F19B9Aull));
    REQUIRE(val + u32 == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F19B9B9B9Aull));
    REQUIRE(val + u64 == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull, 0x9B9B9B9B9B9B9B9Aull));
    REQUIRE(val + u128 == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull, 0x9B9B9B9B9B9B9B9Bull, 0x9B9B9B9B9B9B9B9Aull));

    REQUIRE((t += val) == true);
    REQUIRE((f += val) == false);
    REQUIRE((u8 += val) == (uint8_t)0x9Aull);
    REQUIRE((u16 += val) == (uint16_t)0x9B9Aull);
    REQUIRE((u32 += val) == (uint32_t)0x9B9B9B9Aull);
    REQUIRE((u64 += val) == (uint64_t)0x9B9B9B9B9B9B9B9Aull);
    REQUIRE((u128 += val) == uint128_t(0x9B9B9B9B9B9B9B9Bull, 0x9B9B9B9B9B9B9B9Aull));
}

TEST_CASE("uint256: And", "[CppCommon][Common]")
{
    uint256_t t((bool)true);
    uint256_t f((bool)false);
    uint256_t u8((uint8_t)0xAAull);
    uint256_t u16((uint16_t)0xAAAAull);
    uint256_t u32((uint32_t)0xAAAAAAAAull);
    uint256_t u64((uint64_t)0xAAAAAAAAAAAAAAAAull);

    const uint256_t val(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull);

    REQUIRE((t & val) == uint256_t(0));
    REQUIRE((f & val) == uint256_t(0));
    REQUIRE((u8 & val) == uint256_t(0xA0ull));
    REQUIRE((u16 & val) == uint256_t(0xA0A0ull));
    REQUIRE((u32 & val) == uint256_t(0xA0A0A0A0ull));
    REQUIRE((u64 & val) == uint256_t(0xA0A0A0A0A0A0A0A0ull));

    REQUIRE((t &= val) == uint256_t(0x0ull));
    REQUIRE((f &= val) == uint256_t(0x0ull));
    REQUIRE((u8 &= val) == uint256_t(0xA0ull));
    REQUIRE((u16 &= val) == uint256_t(0xA0A0ull));
    REQUIRE((u32 &= val) == uint256_t(0xA0A0A0A0ull));
    REQUIRE((u64 &= val) == uint256_t(0xA0A0A0A0A0A0A0A0ull));
}

TEST_CASE("uint256: And external", "[CppCommon][Common]")
{
    bool t = true;
    bool f = false;
    uint8_t u8 = 0xAAull;
    uint16_t u16 = 0xAAAAull;
    uint32_t u32 = 0xAAAAAAAAull;
    uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;

    const uint256_t val(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull);

    REQUIRE((t & val) == uint256_t(0x0ull));
    REQUIRE((f & val) == uint256_t(0x0ull));
    REQUIRE((u8 & val) == uint256_t(0xA0ull));
    REQUIRE((u16 & val) == uint256_t(0xA0A0ull));
    REQUIRE((u32 & val) == uint256_t(0xA0A0A0A0ull));
    REQUIRE((u64 & val) == uint256_t(0xA0A0A0A0A0A0A0A0ull));

    REQUIRE((t &= val) == false);
    REQUIRE((f &= val) == false);
    REQUIRE((u8 &= val) == (uint8_t)0xA0ull);
    REQUIRE((u16 &= val) == (uint16_t)0xA0A0ull);
    REQUIRE((u32 &= val) == (uint32_t)0xA0A0A0A0ull);
    REQUIRE((u64 &= val) == (uint64_t)0xA0A0A0A0A0A0A0A0ull);

    // Zero
    REQUIRE((uint256_t() & val) == 0);
}
