/*!
    \file uint256.inl
    \brief Unsigned 256-bit integer type inline implementation
    \author Ivan Shynkarenka
    \date 11.10.2017
    \copyright MIT License
*/

namespace CppCommon {

inline uint256_t::uint256_t() noexcept
    : _upper(0), _lower(0)
{
}

template <typename T>
inline uint256_t::uint256_t(const T& value) noexcept
    : _upper(0), _lower(value)
{
    static_assert(std::is_integral<T>::value, "Input argument type must be an integer!");
}

template <typename TUpper, typename TLower>
inline uint256_t::uint256_t(const TUpper& upper, const TLower& lower) noexcept
    : _upper(upper), _lower(lower)
{
    static_assert((std::is_integral<TUpper>::value && std::is_integral<TLower>::value), "Input argument types must be integers!");
}


template <typename TUpperUpper, typename TUpperLower, typename TLowerUpper, typename TLowerLower>
inline uint256_t::uint256_t(const TUpperUpper& upper_upper, const TUpperLower& upper_lower, const TLowerUpper& lower_upper, const TLowerLower& lower_lower) noexcept
    : _upper(upper_upper, upper_lower), _lower(lower_upper, lower_lower)
{
    static_assert((std::is_integral<TUpperUpper>::value && std::is_integral<TUpperLower>::value && std::is_integral<TLowerUpper>::value && std::is_integral<TLowerLower>::value), "Input argument types must be integers!");
}

template <typename T>
inline uint256_t& uint256_t::operator=(const T& value) noexcept
{
    static_assert(std::is_integral<T>::value, "Input argument type must be an integer!");

    _upper = 0;
    _lower = value;
    return *this;
}

inline uint256_t operator+(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return uint256_t(value1._upper + value2._upper + (((value1._lower + value2._lower) < value1._lower) ? 1 : 0), value1._lower + value2._lower);
}

inline uint256_t operator-(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return uint256_t(value1._upper - value2._upper - (((value1._lower - value2._lower) > value1._lower) ? 1 : 0), value1._lower - value2._lower);
}

inline uint256_t operator*(const uint256_t& value1, const uint256_t& value2) noexcept
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

    // Move carry to the next digit
    third64  += fourth64 >> 32;
    second64 += third64  >> 32;
    first64  += second64 >> 32;

    // Combines the values, taking care of carry over
    return uint256_t(first64 << 64, 0) + uint256_t(third64.upper(), third64 << 64) + uint256_t(second64, 0) + uint256_t(fourth64);
}

inline uint256_t operator/(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return uint128_t::divmod(value1, value2).first;
}

inline uint256_t operator%(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return uint128_t::divmod(value1, value2).second;
}

inline uint256_t operator&(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return uint256_t(value1._upper & value2._upper, value1._lower & value2._lower);
}

inline uint256_t operator|(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return uint256_t(value1._upper | value2._upper, value1._lower | value2._lower);
}

inline uint256_t operator^(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return uint256_t(value1._upper ^ value2._upper, value1._lower ^ value2._lower);
}

inline bool operator==(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return ((value1._upper == value2._upper) && (value1._lower == value2._lower));
}

inline bool operator!=(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return ((value1._upper != value2._upper) || (value1._lower != value2._lower));
}

inline bool operator<(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return (value1._upper == value2._upper) ? (value1._lower < value2._lower) : (value1._upper < value2._upper);
}

inline bool operator>(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return (value1._upper == value2._upper) ? (value1._lower > value2._lower) : (value1._upper > value2._upper);
}

inline bool operator<=(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return ((value1 < value2) || (value1 == value2));
}

inline bool operator>=(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return ((value1 > value2) || (value1 == value2));
}

inline bool operator&&(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return ((value1._upper == value2._upper) && (value1._lower == value2._lower));
}

inline bool operator||(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return ((value1._upper != value2._upper) || (value1._lower != value2._lower));
}

inline uint256_t operator<<(const uint256_t& value1, const uint256_t& value2) noexcept
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

inline uint256_t operator>>(const uint256_t& value1, const uint256_t& value2) noexcept
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

inline size_t uint256_t::bits() const noexcept
{
    size_t result = 0;

    if (_upper)
    {
        result = 128;
        uint64_t upper = _upper;
        while (upper)
        {
            upper >>= 1;
            ++result;
        }
    }
    else
    {
        uint64_t lower = _lower;
        while (lower)
        {
            lower >>= 1;
            ++result;
        }
    }

    return result;
}

inline std::string uint256_t::string(size_t base, size_t length) const
{
    if ((base < 2) || (base > 16))
        throw std::invalid_argument("Base must be in the range [2, 16]");

    std::string out = "";

    if (!(*this))
        out = "0";
    else
    {
        std::pair<uint256_t, uint256_t> qr(*this, 0);
        do
        {
            qr = uint128_t::divmod(qr.first, base);
            out = "0123456789abcdef"[(size_t)qr.second] + out;
        } while (qr.first != 0);
    }

    if (out.size() < length)
        out = std::string(length - out.size(), '0') + out;

    return out;
}

inline std::pair<uint256_t, uint256_t> uint256_t::divmod(const uint256_t& x, const uint256_t& y)
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
    size_t delta = x.bits() - y.bits();
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

inline std::ostream& operator<<(std::ostream& os, const uint256_t& value)
{
    if (os.flags() & os.oct)
        os << value.string(8);
    else if (os.flags() & os.dec)
        os << value.string(10);
    else if (os.flags() & os.hex)
        os << value.string(16);
    return os;
}

inline void uint256_t::swap(uint256_t& value) noexcept
{
    using std::swap;
    swap(_upper, value._upper);
    swap(_lower, value._lower);
}

inline void swap(uint256_t& value1, uint256_t& value2) noexcept
{
    value1.swap(value2);
}

} // namespace CppCommon
