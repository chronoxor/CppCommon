/*!
    \file uint256.cpp
    \brief Unsigned 256-bit integer type implementation
    \author Ivan Shynkarenka
    \date 11.10.2017
    \copyright MIT License
*/

#include "common/uint256.h"

namespace CppCommon {

uint256_t operator*(const uint256_t& value1, const uint256_t& value2) noexcept
{
    // Split values into four 32-bit parts
    uint128_t top[4] = { value1.upper().upper(), value1.upper().lower(), value1.lower().upper(), value1.lower().lower() };
    uint128_t bottom[4] = { value2.upper().upper(), value2.upper().lower(), value2.lower().upper(), value2.lower().lower() };
    uint128_t products[4][4];

    // Multiply each component of the values
    for (int y = 3; y > -1; --y)
        for (int x = 3; x > -1; --x)
            products[3 - x][y] = top[x] * bottom[y];

    // First row
    uint128_t fourth64 = uint128_t(products[0][3].lower());
    uint128_t third64  = uint128_t(products[0][2].lower()) + uint128_t(products[0][3].upper());
    uint128_t second64 = uint128_t(products[0][1].lower()) + uint128_t(products[0][2].upper());
    uint128_t first64  = uint128_t(products[0][0].lower()) + uint128_t(products[0][1].upper());

    // Second row
    third64  += uint128_t(products[1][3].lower());
    second64 += uint128_t(products[1][2].lower()) + uint128_t(products[1][3].upper());
    first64  += uint128_t(products[1][1].lower()) + uint128_t(products[1][2].upper());

    // Third row
    second64 += uint128_t(products[2][3].lower());
    first64  += uint128_t(products[2][2].lower()) + uint128_t(products[2][3].upper());

    // Fourth row
    first64  += uint128_t(products[3][3].lower());

    // Combines the values, taking care of carry over
    return uint256_t(first64 << 64, 0) + uint256_t(third64.upper(), third64 << 64) + uint256_t(second64, 0) + uint256_t(fourth64);
}

uint256_t operator<<(const uint256_t& value1, const uint256_t& value2) noexcept
{
    const uint128_t shift = value2._lower;

    if (((bool)value2._upper) || (shift >= 256))
        return 0;
    else if (shift == 128)
        return uint256_t(value1._lower, 0);
    else if (shift == 0)
        return value1;
    else if (shift < 128)
        return uint256_t((value1._upper << shift) + (value1._lower >> (128 - shift)), value1._lower << shift);
    else if ((256 > shift) && (shift > 128))
        return uint256_t(value1._lower << (shift - 128), 0);
    else
        return 0;
}

uint256_t operator>>(const uint256_t& value1, const uint256_t& value2) noexcept
{
    const uint128_t shift = value2._lower;

    if (((bool)value2._upper) || (shift >= 256))
        return 0;
    else if (shift == 128)
        return uint256_t(value1._upper);
    else if (shift == 0)
        return value1;
    else if (shift < 128)
        return uint256_t(value1._upper >> shift, (value1._upper << (128 - shift)) + (value1._lower >> shift));
    else if ((256 > shift) && (shift > 128))
        return uint256_t(value1._upper >> (shift - 128));
    else
        return 0;
}

size_t uint256_t::bits() const noexcept
{
    size_t result = 0;

    if (_upper)
    {
        result = 128;
        uint128_t upper = _upper;
        while (upper)
        {
            upper >>= 1;
            ++result;
        }
    }
    else
    {
        uint128_t lower = _lower;
        while (lower)
        {
            lower >>= 1;
            ++result;
        }
    }

    return result;
}

std::string uint256_t::string(size_t base, size_t length) const
{
    if ((base < 2) || (base > 16))
        throw std::invalid_argument("Base must be in the range [2, 16]");

    std::string out;

    if (!(*this))
        out = "0";
    else
    {
        std::pair<uint256_t, uint256_t> qr(*this, 0);
        do
        {
            qr = uint256_t::divmod(qr.first, uint256_t(base));
            out = "0123456789abcdef"[(uint8_t)qr.second] + out;
        } while (qr.first != 0);
    }

    if (out.size() < length)
        out = std::string(length - out.size(), '0') + out;

    return out;
}

std::wstring uint256_t::wstring(size_t base, size_t length) const
{
    if ((base < 2) || (base > 16))
        throw std::invalid_argument("Base must be in the range [2, 16]");

    std::wstring out;

    if (!(*this))
        out = L"0";
    else
    {
        std::pair<uint256_t, uint256_t> qr(*this, 0);
        do
        {
            qr = uint256_t::divmod(qr.first, uint256_t(base));
            out = L"0123456789abcdef"[(uint8_t)qr.second] + out;
        } while (qr.first != 0);
    }

    if (out.size() < length)
        out = std::wstring(length - out.size(), L'0') + out;

    return out;
}

std::pair<uint256_t, uint256_t> uint256_t::divmod(const uint256_t& x, const uint256_t& y)
{
    if (y == 0)
        throw std::domain_error("Division by 0");
    else if (y == 1)
        return std::pair<uint256_t, uint256_t>(x, 0);
    else if (x == y)
        return std::pair<uint256_t, uint256_t>(1, 0);
    else if ((x == 0) || (x < y))
        return std::pair<uint256_t, uint256_t>(0, x);

    std::pair<uint256_t, uint256_t> result(0, x);
    uint256_t delta = uint256_t(x.bits() - y.bits());
    uint256_t copyd = y << delta;
    uint256_t adder = 1 << delta;

    if (copyd > result.second)
    {
        copyd >>= 1;
        adder >>= 1;
    }

    while (result.second >= y)
    {
        if (result.second >= copyd)
        {
            result.second -= copyd;
            result.first |= adder;
        }
        copyd >>= 1;
        adder >>= 1;
    }

    return result;
}

} // namespace CppCommon
