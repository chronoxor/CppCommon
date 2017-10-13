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

TEST_CASE("uint256: Unary plus", "[CppCommon][Common]")
{
    const uint256_t value(0x12345ull);

    REQUIRE(+value == value);
}

TEST_CASE("uint256: Unary minus", "[CppCommon][Common]")
{
    const uint256_t val(1);
    const uint256_t neg = -val;

    REQUIRE(-val == neg);
    REQUIRE(-neg == val);
    REQUIRE(neg == uint256_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull));
}

TEST_CASE("uint256: Increment", "[CppCommon][Common]")
{
    uint256_t value(0);

    REQUIRE(++value == 1);
    REQUIRE(value++ == 1);
    REQUIRE(++value == 3);
}

TEST_CASE("uint256: Decrement", "[CppCommon][Common]")
{
    uint256_t value(0);

    REQUIRE(--value == uint256_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull));
    REQUIRE(value-- == uint256_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull));
    REQUIRE(--value == uint256_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xfffffffffffffffdull));
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
    REQUIRE((f += val) == true);
    REQUIRE((u8 += val) == (uint8_t)0x9Aull);
    REQUIRE((u16 += val) == (uint16_t)0x9B9Aull);
    REQUIRE((u32 += val) == (uint32_t)0x9B9B9B9Aull);
    REQUIRE((u64 += val) == (uint64_t)0x9B9B9B9B9B9B9B9Aull);
    REQUIRE((u128 += val) == uint128_t(0x9B9B9B9B9B9B9B9Bull, 0x9B9B9B9B9B9B9B9Aull));
}

TEST_CASE("uint256: Subtract", "[CppCommon][Common]")
{
    uint256_t big(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    const uint256_t small(0x0000000000000000ull, 0x0000000000000001ull);

    REQUIRE(small - small == 0);
    REQUIRE(small - big == uint256_t(0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000002ull));
    REQUIRE(big - small == uint256_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFEull));
    REQUIRE(big - big == 0);
}

TEST_CASE("uint256: Subtract external", "[CppCommon][Common]")
{
    bool t = true;
    bool f = false;
    uint8_t u8  = 0xAAull;
    uint16_t u16 = 0xAAAAull;
    uint32_t u32 = 0xAAAAAAAAull;
    uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;
    uint128_t u128(0xAAAAAAAAAAAAAAAAull, 0xAAAAAAAAAAAAAAAAull);

    const uint256_t val(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull);

    REQUIRE(t - val == uint256_t(0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F11ull));
    REQUIRE(f - val == uint256_t(0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F10ull));
    REQUIRE(u8 - val == uint256_t(0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0FBAull));
    REQUIRE(u16 - val == uint256_t(0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0FB9BAull));
    REQUIRE(u32 - val == uint256_t(0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0FB9B9B9BAull));
    REQUIRE(u64 - val == uint256_t(0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0xB9B9B9B9B9B9B9BAull));
    REQUIRE(u128 - val == uint256_t(0x0F0F0F0F0F0F0F0Full, 0x0F0F0F0F0F0F0F0Full, 0xB9B9B9B9B9B9B9B9ull, 0xB9B9B9B9B9B9B9BAull));

    REQUIRE((t -= val) == true);
    REQUIRE((f -= val) == true);
    REQUIRE((u8 -= val) == (uint8_t)0xBAull);
    REQUIRE((u16 -= val) == (uint16_t)0xB9BAull);
    REQUIRE((u32 -= val) == (uint32_t)0xB9B9B9BAull);
    REQUIRE((u64 -= val) == (uint64_t)0xB9B9B9B9B9B9B9BAull);
    REQUIRE((u128 -= val) == uint128_t(0xB9B9B9B9B9B9B9B9ull, 0xB9B9B9B9B9B9B9BAull));
}

TEST_CASE("uint256: Multiply", "[CppCommon][Common]")
{
    uint256_t val(0xFEDBCA9876543210ull);

    REQUIRE(val * val == uint256_t(0x0000000000000000ull, 0x0000000000000000ull, 0xFDB8E2BACBFE7CEFull, 0x010E6CD7A44A4100ull));

    const uint256_t zero = 0;
    REQUIRE(val * zero == zero);
    REQUIRE(zero * val == zero);

    const uint256_t one = 1;
    REQUIRE(val * one == val);
    REQUIRE(one * val == val);
}

TEST_CASE("uint256: Multiply external", "[CppCommon][Common]")
{
    bool t = true;
    bool f = false;
    uint8_t u8 = 0xAAull;
    uint16_t u16 = 0xAAAAull;
    uint32_t u32 = 0xAAAAAAAAull;
    uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;
    uint128_t u128(0xAAAAAAAAAAAAAAAAull, 0xAAAAAAAAAAAAAAAAull);

    const uint256_t val(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull);

    REQUIRE(t * val == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull));
    REQUIRE(f * val == uint256_t(0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull));
    REQUIRE(u8 * val == uint256_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFF60ull));
    REQUIRE(u16 * val == uint256_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFF5F60ull));
    REQUIRE(u32 * val == uint256_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFF5F5F5F60ull));
    REQUIRE(u64 * val == uint256_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0x5F5F5F5F5F5F5F60ull));
    REQUIRE(u128 * val == uint256_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0x5F5F5F5F5F5F5F5Full, 0x5F5F5F5F5F5F5F60ull));

    REQUIRE((t *= val) == true);
    REQUIRE((f *= val) == false);
    REQUIRE((u8 *= val) == (uint8_t)0x60ull);
    REQUIRE((u16 *= val) == (uint16_t)0x5F60ull);
    REQUIRE((u32 *= val) == (uint32_t)0x5F5F5F60ull);
    REQUIRE((u64 *= val) == (uint64_t)0x5F5F5F5F5F5F5F60ull);
    REQUIRE((u128 *= val) == uint128_t(0x5F5F5F5F5F5F5F5Full, 0x5F5F5F5F5F5F5F60ull));
}

TEST_CASE("uint256: Divide", "[CppCommon][Common]")
{
    const uint256_t big(0xFEDBCA9876543210ull);
    const uint256_t small(0xFFFFull);
    const uint256_t res_val(0xFEDCC9753FC9ull);

    REQUIRE(small / small == 1);
    REQUIRE(small / big == 0);

    REQUIRE(big / big == 1);

    // Div 0
    REQUIRE_THROWS_AS(uint256_t(1) / uint256_t(0), std::domain_error);
}

TEST_CASE("uint256: Divide external", "[CppCommon][Common]")
{
    /*
    bool t = true;
    bool f = false;
    uint8_t u8 = 0xAAull;
    uint16_t u16 = 0xAAAAull;
    uint32_t u32 = 0xAAAAAAAAull;
    uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;
    uint128_t u128(0xAAAAAAAAAAAAAAAAull, 0xAAAAAAAAAAAAAAAAull);

    const uint256_t val(0x7Bull);

    REQUIRE(t / val == false);
    REQUIRE(f / val == false);
    REQUIRE(u8 / val == uint256_t(0x1ull));
    REQUIRE(u16 / val == uint256_t(0x163ull));
    REQUIRE(u32 / val == uint256_t(0x163356Bull));
    REQUIRE(u64 / val == uint256_t(0x163356B88AC0DE0ull));
    REQUIRE(u128 / val == uint256_t(0x0000000000000000ull, 0x0000000000000000ull, 0x163356B88AC0DE0ull, 0x163356B88AC0DE01ull));

    REQUIRE((t /= val) == false);
    REQUIRE((f /= val) == false);
    REQUIRE((u8 /= val) == (uint8_t)0x1ull);
    REQUIRE((u16 /= val) == (uint16_t)0x163ull);
    REQUIRE((u32 /= val) == (uint32_t)0x163356Bull);
    REQUIRE((u64 /= val) == (uint64_t)0x163356B88AC0DE0ull);
    REQUIRE((u128 /= val) == uint128_t(0x163356B88AC0DE0ull, 0x163356B88AC0DE01ull));
    */
}

TEST_CASE("uint256: Modulo", "[CppCommon][Common]")
{
    // Has remainder
    const uint256_t val(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    const uint256_t val_mod(0xFEDCBA9876543210ull);
    REQUIRE(val % val_mod == uint256_t(0x63794F9D55C8D29F));

    // No remainder
    const uint256_t val_0(0xFEDCBA9876543210, 0, 0, 0);
    REQUIRE(val_0 % val_mod == 0);

    // Mod 0
    REQUIRE_THROWS_AS(uint256_t(1) % uint256_t(0), std::domain_error);
}

TEST_CASE("uint256: Modulo external", "[CppCommon][Common]")
{
    bool t = true;
    bool f = false;
    uint8_t u8 = 0xAAull;
    uint16_t u16 = 0xAAAAull;
    uint32_t u32 = 0xAAAAAAAAull;
    uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;
    uint128_t u128(0xAAAAAAAAAAAAAAAAull, 0xAAAAAAAAAAAAAAAAull);

    // Prime
    const uint256_t val(0xD03ull);

    REQUIRE(t % val == true);
    REQUIRE(f % val == false);
    REQUIRE(u8 % val == uint256_t(0xAAull));
    REQUIRE(u16 % val == uint256_t(0x183ull));
    REQUIRE(u32 % val == uint256_t(0x249ull));
    REQUIRE(u64 % val == uint256_t(0xC7Full));
    REQUIRE(u128 % val == uint256_t(0x9FBull));

    REQUIRE((t %= val) == true);
    REQUIRE((f %= val) == false);
    REQUIRE((u8 %= val) == (uint8_t)0xAAull);
    REQUIRE((u16 %= val) == (uint16_t)0x183ull);
    REQUIRE((u32 %= val) == (uint32_t)0x249ull);
    REQUIRE((u64 %= val) == (uint64_t)0xC7Full);
    REQUIRE((u128 %= val) == (uint256_t)0x9FBull);
}

TEST_CASE("uint256: Invert", "[CppCommon][Common]")
{
    for (size_t hi_hi = 0; hi_hi < 2; ++hi_hi)
    {
        for (size_t hi_lo = 0; hi_lo < 2; ++hi_lo)
        {
            for (size_t lo_hi = 0; lo_hi < 2; ++lo_hi)
            {
                for (size_t lo_lo = 0; lo_lo < 2; ++lo_lo)
                {
                    const uint256_t val = ~uint256_t(hi_hi ? 0xFFFFFFFFFFFFFFFFull : 0x0000000000000000ull,
                                                     hi_lo ? 0xFFFFFFFFFFFFFFFFull : 0x0000000000000000ull,
                                                     lo_hi ? 0xFFFFFFFFFFFFFFFFull : 0x0000000000000000ull,
                                                     lo_lo ? 0xFFFFFFFFFFFFFFFFull : 0x0000000000000000ull);

                    REQUIRE(val.upper().upper() == (hi_hi ? 0x0000000000000000ull : 0xFFFFFFFFFFFFFFFFull));
                    REQUIRE(val.upper().lower() == (hi_lo ? 0x0000000000000000ull : 0xFFFFFFFFFFFFFFFFull));
                    REQUIRE(val.lower().upper() == (lo_hi ? 0x0000000000000000ull : 0xFFFFFFFFFFFFFFFFull));
                    REQUIRE(val.lower().lower() == (lo_lo ? 0x0000000000000000ull : 0xFFFFFFFFFFFFFFFFull));
                }
            }
        }
    }
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

TEST_CASE("uint256: Bits", "[CppCommon][Common]")
{
    uint256_t value = 1;
    for (size_t i = 0; i < 127; ++i)
    {
        REQUIRE(value.bits() == i + 1);
        value <<= 1;
    }

    REQUIRE(uint256_t(0).bits() == 0);
}

TEST_CASE("uint256: Data", "[CppCommon][Common]")
{
    const uint256_t value(0xFEDCBA9876543210ull, 0x0123456789ABCDEFull, 0xFEDCBA9876543210ull, 0x0123456789ABCDEFull);

    REQUIRE(value.upper().upper() == 0xFEDCBA9876543210ull);
    REQUIRE(value.upper().lower() == 0x0123456789ABCDEFull);
    REQUIRE(value.lower().upper() == 0xFEDCBA9876543210ull);
    REQUIRE(value.lower().lower() == 0x0123456789ABCDEFull);
}

TEST_CASE("uint256: Type cast", "[CppCommon][Common]")
{
    const uint256_t val(0xAAAAAAAAAAAAAAAAull, 0xAAAAAAAAAAAAAAAAull);

    REQUIRE(static_cast<bool>(uint256_t(true)) == true);
    REQUIRE(static_cast<bool>(uint256_t(false)) == false);
    REQUIRE(static_cast<uint8_t>(val) == (uint8_t)0xAAull);
    REQUIRE(static_cast<uint16_t>(val) == (uint16_t)0xAAAAull);
    REQUIRE(static_cast<uint32_t>(val) == (uint32_t)0xAAAAAAAAull);
    REQUIRE(static_cast<uint64_t>(val) == (uint64_t)0xAAAAAAAAAAAAAAAAull);
}

TEST_CASE("uint256: Type traits", "[CppCommon][Common]")
{
    REQUIRE(std::is_arithmetic<uint128_t>::value == true);
    REQUIRE(std::is_integral<uint128_t>::value == true);
    REQUIRE(std::is_unsigned<uint128_t>::value == true);

    REQUIRE(std::is_arithmetic<uint256_t>::value == true);
    REQUIRE(std::is_integral<uint256_t>::value == true);
    REQUIRE(std::is_unsigned<uint256_t>::value == true);
}
