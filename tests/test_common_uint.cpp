//
// Created by Ivan Shynkarenka on 11.10.2017
//

#include "test.h"

#include "common/uint256.h"

#include <limits>
#include <map>
#include <sstream>

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
    const uint256_t u8_1 = uint256_t(0x01ull);
    const uint256_t u16_1 = uint256_t(0x0123ull);
    const uint256_t u32_1 = uint256_t(0x01234567ull);
    const uint256_t u64_1 = uint256_t(0x0123456789ABCDEFull);

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

TEST_CASE("uint256: Multiply (128-bit)", "[CppCommon][Common]")
{
    uint128_t val(0xFEDBCA9876543210ull);

    REQUIRE(val * val == uint128_t(0xFDB8E2BACBFE7CEFull, 0x010E6CD7A44A4100ull));

    const uint128_t zero = 0;
    REQUIRE(val * zero == zero);
    REQUIRE(zero * val == zero);

    const uint128_t one = 1;
    REQUIRE(val * one == val);
    REQUIRE(one * val == val);
}

TEST_CASE("uint256: Multiply (256-bit)", "[CppCommon][Common]")
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

TEST_CASE("uint256: Multiply external (128-bit)", "[CppCommon][Common]")
{
    bool t = true;
    bool f = false;
    uint8_t u8 = 0xAAull;
    uint16_t u16 = 0xAAAAull;
    uint32_t u32 = 0xAAAAAAAAull;
    uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;
    uint128_t u128(0xAAAAAAAAAAAAAAAAull, 0xAAAAAAAAAAAAAAAAull);

    const uint128_t val(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull);

    REQUIRE(t * val == uint128_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull));
    REQUIRE(f * val == uint128_t(0x0000000000000000ull, 0x0000000000000000ull));
    REQUIRE(u8 * val == uint128_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFF60ull));
    REQUIRE(u16 * val == uint128_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFF5F60ull));
    REQUIRE(u32 * val == uint128_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFF5F5F5F60ull));
    REQUIRE(u64 * val == uint128_t(0xFFFFFFFFFFFFFFFFull, 0x5F5F5F5F5F5F5F60ull));
    REQUIRE(u128 * val == uint128_t(0x5F5F5F5F5F5F5F5Full, 0x5F5F5F5F5F5F5F60ull));

    REQUIRE((t *= val) == true);
    REQUIRE((f *= val) == false);
    REQUIRE((u8 *= val) == (uint8_t)0x60ull);
    REQUIRE((u16 *= val) == (uint16_t)0x5F60ull);
    REQUIRE((u32 *= val) == (uint32_t)0x5F5F5F60ull);
    REQUIRE((u64 *= val) == (uint64_t)0x5F5F5F5F5F5F5F60ull);
    REQUIRE((u128 *= val) == uint128_t(0x5F5F5F5F5F5F5F5Full, 0x5F5F5F5F5F5F5F60ull));
}

TEST_CASE("uint256: Multiply external (256-bit)", "[CppCommon][Common]")
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

TEST_CASE("uint256: Divide (128-bit)", "[CppCommon][Common]")
{
    const uint128_t big(0xFEDBCA9876543210ull);
    const uint128_t small(0xFFFFull);
    const uint128_t res_val(0xFEDCC9753FC9ull);

    REQUIRE(small / small == 1);
    REQUIRE(small / big == 0);

    REQUIRE(big / big == 1);

    // Div 0
    REQUIRE_THROWS_AS(uint128_t(1) / uint128_t(0), std::domain_error);
}

TEST_CASE("uint256: Divide (256-bit)", "[CppCommon][Common]")
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

TEST_CASE("uint256: Divide external (128-bit)", "[CppCommon][Common]")
{
    bool t = true;
    bool f = false;
    uint8_t u8 = 0xAAull;
    uint16_t u16 = 0xAAAAull;
    uint32_t u32 = 0xAAAAAAAAull;
    uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;
    uint128_t u128(0xAAAAAAAAAAAAAAAAull, 0xAAAAAAAAAAAAAAAAull);

    const uint128_t val(0x7Bull);

    REQUIRE(t / val == false);
    REQUIRE(f / val == false);
    REQUIRE(u8 / val == uint128_t(0x1ull));
    REQUIRE(u16 / val == uint128_t(0x163ull));
    REQUIRE(u32 / val == uint128_t(0x163356Bull));
    REQUIRE(u64 / val == uint128_t(0x163356B88AC0DE0ull));
    REQUIRE(u128 / val == uint128_t(0x163356B88AC0DE0ull, 0x163356B88AC0DE01ull));

    REQUIRE((t /= val) == false);
    REQUIRE((f /= val) == false);
    REQUIRE((u8 /= val) == (uint8_t)0x1ull);
    REQUIRE((u16 /= val) == (uint16_t)0x163ull);
    REQUIRE((u32 /= val) == (uint32_t)0x163356Bull);
    REQUIRE((u64 /= val) == (uint64_t)0x163356B88AC0DE0ull);
    REQUIRE((u128 /= val) == uint128_t(0x163356B88AC0DE0ull, 0x163356B88AC0DE01ull));
}

TEST_CASE("uint256: Divide external (256-bit)", "[CppCommon][Common]")
{
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
}

TEST_CASE("uint256: Modulo", "[CppCommon][Common]")
{
    // Has remainder
    const uint256_t val(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    const uint256_t val_mod(0xFEDCBA9876543210ull);
    REQUIRE(val % val_mod == uint256_t(0x63794F9D55C8D29Full));

    // No remainder
    const uint256_t val_0(0xFEDCBA9876543210ull, 0, 0, 0);
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

TEST_CASE("uint256: Or", "[CppCommon][Common]")
{
    uint256_t t((bool)true);
    uint256_t f((bool)false);
    uint256_t u8((uint8_t)0xAAull);
    uint256_t u16((uint16_t)0xAAAAull);
    uint256_t u32((uint32_t)0xAAAAAAAAull);
    uint256_t u64((uint64_t)0xAAAAAAAAAAAAAAAAull);

    const uint256_t val(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull);

    REQUIRE((t | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull));
    REQUIRE((f | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull));
    REQUIRE((u8 | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0FAull));
    REQUIRE((u16 | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0FAFAull));
    REQUIRE((u32 | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0FAFAFAFAull));
    REQUIRE((u64 | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xFAFAFAFAFAFAFAFAull));

    REQUIRE((t |= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull));
    REQUIRE((f |= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull));
    REQUIRE((u8 |= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0FAull));
    REQUIRE((u16 |= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0FAFAull));
    REQUIRE((u32 |= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0FAFAFAFAull));
    REQUIRE((u64 |= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xFAFAFAFAFAFAFAFAull));

    // Zero
    REQUIRE((uint256_t() | val) == val);
}

TEST_CASE("uint256: Or external", "[CppCommon][Common]")
{
    bool t = true;
    bool f = false;
    uint8_t u8 = 0xAAull;
    uint16_t u16 = 0xAAAAull;
    uint32_t u32 = 0xAAAAAAAAull;
    uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;

    const uint256_t val(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull);

    REQUIRE((t | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull));
    REQUIRE((f | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull));
    REQUIRE((u8 | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0FAull));
    REQUIRE((u16 | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0FAFAull));
    REQUIRE((u32 | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0FAFAFAFAull));
    REQUIRE((u64 | val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xFAFAFAFAFAFAFAFAull));

    REQUIRE((t |= val) == true);
    REQUIRE((f |= val) == true);
    REQUIRE((u8 |= val) == (uint8_t)0xFAull);
    REQUIRE((u16 |= val) == (uint16_t)0xFAFAull);
    REQUIRE((u32 |= val) == (uint32_t)0xFAFAFAFAull);
    REQUIRE((u64 |= val) == (uint64_t)0xFAFAFAFAFAFAFAFAull);

    // Zero
    REQUIRE((uint256_t() | val) == val);
}

TEST_CASE("uint256: Xor", "[CppCommon][Common]")
{
    uint256_t t((bool)true);
    uint256_t f((bool)false);
    uint256_t u8((uint8_t)0xAAull);
    uint256_t u16((uint16_t)0xAAAAull);
    uint256_t u32((uint32_t)0xAAAAAAAAull);
    uint256_t u64((uint64_t)0xAAAAAAAAAAAAAAAA);

    const uint256_t val(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull);

    REQUIRE((t ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull));
    REQUIRE((f ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull));
    REQUIRE((u8 ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F05Aull));
    REQUIRE((u16 ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F05A5Aull));
    REQUIRE((u32 ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F05A5A5A5Aull));
    REQUIRE((u64 ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0x5A5A5A5A5A5A5A5Aull));

    REQUIRE((t ^= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull));
    REQUIRE((f ^= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull));
    REQUIRE((u8 ^= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F05Aull));
    REQUIRE((u16 ^= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F05A5Aull));
    REQUIRE((u32 ^= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F05A5A5A5Aull));
    REQUIRE((u64 ^= val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0x5A5A5A5A5A5A5A5Aull));

    // Zero
    REQUIRE((uint256_t() ^ val) == val);
}

TEST_CASE("uint256: Xor external", "[CppCommon][Common]")
{
    bool t = true;
    bool f = false;
    uint8_t u8 = 0xAAull;
    uint16_t u16 = 0xAAAAull;
    uint32_t u32 = 0xAAAAAAAAull;
    uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;

    const uint256_t val(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull);

    REQUIRE((t ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F1ull));
    REQUIRE((f ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F0F0ull));
    REQUIRE((u8 ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F0F05Aull));
    REQUIRE((u16 ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F0F0F05A5Aull));
    REQUIRE((u32 ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0xF0F0F0F05A5A5A5Aull));
    REQUIRE((u64 ^ val) == uint256_t(0xF0F0F0F0F0F0F0F0ull, 0x5A5A5A5A5A5A5A5Aull));

    REQUIRE((t ^= val) == true);
    REQUIRE((f ^= val) == true);
    REQUIRE((u8 ^= val) == (uint8_t)0x5Aull);
    REQUIRE((u16 ^= val) == (uint16_t)0x5A5Aull);
    REQUIRE((u32 ^= val) == (uint32_t)0x5A5A5A5Aull);
    REQUIRE((u64 ^= val) == (uint64_t)0x5A5A5A5A5A5A5A5Aull);

    // Zero
    REQUIRE((uint256_t() ^ val) == val);
}

TEST_CASE("uint256: Left shift", "[CppCommon][Common]")
{
    uint256_t val(0x1);
    uint64_t exp_val = 1;
    for (size_t i = 0; i < 64; ++i)
        REQUIRE((val << i) == (exp_val << i));

    uint256_t zero(0);
    for (size_t i = 0; i < 64; ++i)
        REQUIRE((zero << i) == 0);

    for (size_t i = 0; i < 63; ++i)
        REQUIRE((val <<= 1) == (exp_val <<= 1));

    for (size_t i = 0; i < 63; ++i)
        REQUIRE((zero <<= 1) == 0);
}

TEST_CASE("uint256: Left shift external", "[CppCommon][Common]")
{
    bool t = true;
    bool f = false;
    uint8_t u8 = 0xFFull;
    uint16_t u16 = 0xFFFFull;
    uint32_t u32 = 0xFFFFFFFFull;
    uint64_t u64 = 0xFFFFFFFFFFFFFFFFull;
    uint128_t u128(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);

    const uint256_t zero(0);
    const uint256_t one(1);

    REQUIRE((t << zero) == t);
    REQUIRE((f << zero) == f);
    REQUIRE((u8 << zero) == u8);
    REQUIRE((u16 << zero) == u16);
    REQUIRE((u32 << zero) == u32);
    REQUIRE((u64 << zero) == u64);
    REQUIRE((u128 << zero) == u128);

    REQUIRE((t <<= zero) == t);
    REQUIRE((f <<= zero) == f);
    REQUIRE((u8 <<= zero) == u8);
    REQUIRE((u16 <<= zero) == u16);
    REQUIRE((u32 <<= zero) == u32);
    REQUIRE((u64 <<= zero) == u64);
    REQUIRE((u128 <<= zero) == u128);

    REQUIRE((t << one) == (uint256_t(t) << 1));
    REQUIRE((f << one) == (uint256_t(f) << 1));
    REQUIRE((u8 << one) == (uint256_t(u8) << 1));
    REQUIRE((u16 << one) == (uint256_t(u16) << 1));
    REQUIRE((u32 << one) == (uint256_t(u32) << 1));
    REQUIRE((u64 << one) == (uint256_t(u64) << 1));
    REQUIRE((u128 << one) == (uint256_t(u128) << 1));

    REQUIRE((t <<= one) == true);
    REQUIRE((f <<= one) == false);
    REQUIRE((u8 <<= one) == (uint8_t)0xFEull);
    REQUIRE((u16 <<= one) == (uint16_t)0xFFFEull);
    REQUIRE((u32 <<= one) == (uint32_t)0xFFFFFFFEull);
    REQUIRE((u64 <<= one) == (uint64_t)0xFFFFFFFFFFFFFFFEull);
    REQUIRE((u128 <<= one) == uint128_t (0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFEull));

    REQUIRE((u8 << uint256_t(7)) == uint256_t(0x7F00ull));
    REQUIRE((u16 << uint256_t(15)) == uint256_t(0x7FFF0000ull));
    REQUIRE((u32 << uint256_t(31)) == uint256_t(0x7FFFFFFF00000000ull));
    REQUIRE((u64 << uint256_t(63)) == uint256_t(0x0000000000000000ull, 0x0000000000000000ull, 0x7FFFFFFFFFFFFFFFull, 0x0000000000000000ull));
    REQUIRE((u128 << uint256_t(127)) == uint256_t(0x7FFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0x0000000000000000ull, 0x0000000000000000ull));

    REQUIRE((u8 <<= uint256_t(7)) == (uint8_t)0);
    REQUIRE((u16 <<= uint256_t(15)) == (uint16_t)0);
    REQUIRE((u32 <<= uint256_t(31)) == (uint32_t)0);
    REQUIRE((u64 <<= uint256_t(63)) == (uint64_t)0);
    REQUIRE((u128 <<= uint256_t(127)) == (uint128_t)0);
}

TEST_CASE("uint256: Right shift", "[CppCommon][Common]")
{
    uint256_t val(0xFFFFFFFFFFFFFFFFull);
    uint64_t exp = 0xFFFFFFFFFFFFFFFFull;
    for (size_t i = 0; i < 64; ++i)
        REQUIRE((val >> i) == (exp >> i));

    uint256_t zero(0);
    for (size_t i = 0; i < 64; ++i)
        REQUIRE((zero >> i) == 0);

    for (size_t i = 0; i < 64; ++i)
        REQUIRE((val >>= 1) == (exp >>= 1));

    for (size_t i = 0; i < 64; ++i)
        REQUIRE((zero >>= 1) == 0);
}

TEST_CASE("uint256: Right shift external", "[CppCommon][Common]")
{
    bool t = true;
    bool f = false;
    uint8_t u8 = 0xFFull;
    uint16_t u16 = 0xFFFFull;
    uint32_t u32 = 0xFFFFFFFFull;
    uint64_t u64 = 0xFFFFFFFFFFFFFFFFull;

    const uint256_t zero(0);
    const uint256_t one(1);

    REQUIRE((t >> zero) == one);
    REQUIRE((f >> zero) == zero);
    REQUIRE((u8 >> zero) == u8);
    REQUIRE((u16 >> zero) == u16);
    REQUIRE((u32 >> zero) == u32);
    REQUIRE((u64 >> zero) == u64);

    REQUIRE((t >>= zero) == t);
    REQUIRE((f >>= zero) == f);
    REQUIRE((u8 >>= zero) == u8);
    REQUIRE((u16 >>= zero) == u16);
    REQUIRE((u32 >>= zero) == u32);
    REQUIRE((u64 >>= zero) == u64);

    REQUIRE((t >> one) == (uint256_t(t) >> 1));
    REQUIRE((f >> one) == (uint256_t(f) >> 1));
    REQUIRE((u8 >> one) == (uint256_t(u8) >> 1));
    REQUIRE((u16 >> one) == (uint256_t(u16) >> 1));
    REQUIRE((u32 >> one) == (uint256_t(u32) >> 1));
    REQUIRE((u64 >> one) == (uint256_t(u64) >> 1));

    REQUIRE((t >>= one) == false);
    REQUIRE((f >>= one) == false);
    REQUIRE((u8 >>= one) == (uint8_t)0x7Full);
    REQUIRE((u16 >>= one) == (uint16_t)0x7FFFull);
    REQUIRE((u32 >>= one) == (uint32_t)0x7FFFFFFFull);
    REQUIRE((u64 >>= one) == (uint64_t)0x7FFFFFFFFFFFFFFFull);

    REQUIRE((u8 >> uint256_t(7)) == zero);
    REQUIRE((u16 >> uint256_t(15)) == zero);
    REQUIRE((u32 >> uint256_t(31)) == zero);
    REQUIRE((u64 >> uint256_t(63)) == zero);

    REQUIRE((u8 >>= uint256_t(7)) == (uint8_t)0);
    REQUIRE((u16 >>= uint256_t(15)) == (uint16_t)0);
    REQUIRE((u32 >>= uint256_t(31)) == (uint32_t)0);
    REQUIRE((u64 >>= uint256_t(63)) == (uint64_t)0);
}

TEST_CASE("uint256: Equals", "[CppCommon][Common]")
{
    REQUIRE((uint256_t(0xDEADBEEFull) == uint256_t(0xDEADBEEFull)) == true);
    REQUIRE(!(uint256_t(0xDEADBEEFull) == uint256_t(0xFEE1BAADull)) == true);
}

TEST_CASE("uint256: Equals external", "[CppCommon][Common]")
{
    const bool t = true;
    const bool f = false;
    const uint8_t u8 = 0xAAull;
    const uint16_t u16 = 0xAAAAull;
    const uint32_t u32 = 0xAAAAAAAAull;
    const uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;

    REQUIRE(t == uint256_t(t));
    REQUIRE(f == uint256_t(f));
    REQUIRE(u8 == uint256_t(u8));
    REQUIRE(u16 == uint256_t(u16));
    REQUIRE(u32 == uint256_t(u32));
    REQUIRE(u64 == uint256_t(u64));
}

TEST_CASE("uint256: Not equals", "[CppCommon][Common]")
{
    REQUIRE(!(uint256_t(0xDEADBEEFull) != uint256_t(0xDEADBEEFull)) == true);
    REQUIRE((uint256_t(0xDEADBEEFull) != uint256_t(0xFEE1BAADull)) == true);
}

TEST_CASE("uint256: Not equals external", "[CppCommon][Common]")
{
    const bool t = true;
    const bool f = false;
    const uint8_t u8 = 0xAAull;
    const uint16_t u16 = 0xAAAAull;
    const uint32_t u32 = 0xAAAAAAAAull;
    const uint64_t u64 = 0xAAAAAAAAAAAAAAAAull;

    REQUIRE((t != uint256_t(f)) == true);
    REQUIRE((f != uint256_t(t)) == true);
    REQUIRE((u8 != uint256_t(u64)) == true);
    REQUIRE((u16 != uint256_t(u32)) == true);
    REQUIRE((u32 != uint256_t(u16)) == true);
    REQUIRE((u64 != uint256_t(u8)) == true);

    REQUIRE((t != uint256_t(t)) == false);
    REQUIRE((f != uint256_t(f)) == false);
    REQUIRE((u8 != uint256_t(u8)) == false);
    REQUIRE((u16 != uint256_t(u16)) == false);
    REQUIRE((u32 != uint256_t(u32)) == false);
    REQUIRE((u64 != uint256_t(u64)) == false);
}

TEST_CASE("uint256: Less than", "[CppCommon][Common]")
{
    const uint256_t big(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    const uint256_t small(0x0000000000000000ull, 0x0000000000000000ull);

    REQUIRE((small < small) == false);
    REQUIRE((small < big) == true);

    REQUIRE((big < small) == false);
    REQUIRE((big < big) == false);
}

TEST_CASE("uint256: Less than external", "[CppCommon][Common]")
{
    const unsigned small = std::numeric_limits<unsigned>::min();
    const unsigned big = std::numeric_limits<unsigned>::max();

    const uint256_t int_small(small);
    const uint256_t int_big(big);

    REQUIRE((small < int_small) == false);
    REQUIRE((small < int_big) == true);

    REQUIRE((big < int_small) == false);
    REQUIRE((big < int_big) == false);
}

TEST_CASE("uint256: Greater than", "[CppCommon][Common]")
{
    const uint256_t big(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    const uint256_t small(0x0000000000000000ull, 0x0000000000000000ull);

    REQUIRE((small > small) == false);
    REQUIRE((small > big) == false);

    REQUIRE((big > small) == true);
    REQUIRE((big > big) == false);
}

TEST_CASE("uint256: Greater than external", "[CppCommon][Common]")
{
    const unsigned small = std::numeric_limits<unsigned>::min();
    const unsigned big = std::numeric_limits<unsigned>::max();

    const uint256_t int_small(small);
    const uint256_t int_big(big);

    REQUIRE((small > int_small) == false);
    REQUIRE((small > int_big) == false);

    REQUIRE((big > int_small) == true);
    REQUIRE((big > int_big) == false);
}

TEST_CASE("uint256: Less than or equals", "[CppCommon][Common]")
{
    const uint256_t big(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    const uint256_t small(0x0000000000000000ull, 0x0000000000000000ull);

    REQUIRE((small <= small) == true);
    REQUIRE((small <= big) == true);

    REQUIRE((big <= small) == false);
    REQUIRE((big <= big) == true);
}

TEST_CASE("uint256: Less than or equals external", "[CppCommon][Common]")
{
    const unsigned small = std::numeric_limits<unsigned>::min();
    const unsigned big = std::numeric_limits<unsigned>::max();

    const uint256_t int_small(small);
    const uint256_t int_big(big);

    REQUIRE((small <= int_small) == true);
    REQUIRE((small <= int_big) == true);

    REQUIRE((big <= int_small) == false);
    REQUIRE((big <= int_big) == true);
}

TEST_CASE("uint256: Greater or equals than", "[CppCommon][Common]")
{
    const uint256_t big(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    const uint256_t small(0x0000000000000000ull, 0x0000000000000000ull);

    REQUIRE((small >= small) == true);
    REQUIRE((small >= big) == false);

    REQUIRE((big >= small) == true);
    REQUIRE((big >= big) == true);
}

TEST_CASE("uint256: Greater than or equals external", "[CppCommon][Common]")
{
    const unsigned small = std::numeric_limits<unsigned>::min();
    const unsigned big = std::numeric_limits<unsigned>::max();

    const uint256_t int_small(small);
    const uint256_t int_big(big);

    REQUIRE((small >= int_small) == true);
    REQUIRE((small >= int_big) == false);

    REQUIRE((big >= int_small) == true);
    REQUIRE((big >= int_big) == true);
}

TEST_CASE("uint256: Logical Not", "[CppCommon][Common]")
{
    REQUIRE(!uint256_t(0xFFFFFFFF) == false);
}

TEST_CASE("uint256: Logical And", "[CppCommon][Common]")
{
    const uint256_t A(0xFFFFFFFF);
    const uint256_t B(0x00000000);

    REQUIRE((A && A) == true);
    REQUIRE((A && B) == false);
}

TEST_CASE("uint256: Logical Or", "[CppCommon][Common]")
{
    const uint256_t A(0xFFFFFFFF);
    const uint256_t B(0x00000000);

    REQUIRE((A || A) == true);
    REQUIRE((A || B) == true);
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

TEST_CASE("uint256: String", "[CppCommon][Common]")
{
    const std::map<size_t, std::string> tests =
    {
        std::make_pair(2, "10000100000101011000010101101100"),
        std::make_pair(3, "12201102210121112101"),
        std::make_pair(4, "2010011120111230"),
        std::make_pair(5, "14014244043144"),
        std::make_pair(6, "1003520344444"),
        std::make_pair(7, "105625466632"),
        std::make_pair(8, "20405302554"),
        std::make_pair(9, "5642717471"),
        std::make_pair(10, "2216002924"),
        std::make_pair(11, "a3796a883"),
        std::make_pair(12, "51a175124"),
        std::make_pair(13, "294145645"),
        std::make_pair(14, "170445352"),
        std::make_pair(15, "ce82d6d4"),
        std::make_pair(16, "8415856c")
    };

    // Number of leading 0s
    const size_t leading = 5;

    // Make sure all of the test strings create the ASCII version of the string
    const uint256_t original(2216002924);
    for(auto test : tests)
        REQUIRE(original.string(test.first) == test.second);

    // Add leading zeros
    for(size_t base = 2; base <= 16; ++base)
        REQUIRE(original.string(base, tests.at(base).size() + leading) == (std::string(leading, '0') + tests.at(base)));
}

TEST_CASE("uint256: Output stream", "[CppCommon][Common]")
{
    const uint256_t value(0xFEDCBA9876543210ull);

    // Write out octal uint256_t
    std::stringstream oct; oct << std::oct << value;
    REQUIRE(oct.str() == "1773345651416625031020");

    // Write out decimal uint256_t
    std::stringstream dec; dec << std::dec << value;
    REQUIRE(dec.str() == "18364758544493064720");

    // Write out hexadecimal uint256_t
    std::stringstream hex; hex << std::hex << value;
    REQUIRE(hex.str() == "fedcba9876543210");

    // Zero
    std::stringstream zero; zero << uint256_t();
    REQUIRE(zero.str() == "0");
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
