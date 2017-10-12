/*!
    \file uint128.inl
    \brief Unsigned 128-bit integer type inline implementation
    \author Ivan Shynkarenka
    \date 11.10.2017
    \copyright MIT License
*/

namespace CppCommon {

inline uint128_t::uint128_t() noexcept
    : _upper(0), _lower(0)
{
}

template <typename T>
inline uint128_t::uint128_t(const T& value) noexcept
    : _upper(0), _lower(value)
{
    static_assert(std::is_integral<T>::value, "Input argument type must be an integer!");
}

template <typename TUpper, typename TLower>
inline uint128_t::uint128_t(const TUpper& upper, const TLower& lower) noexcept
    : _upper(upper), _lower(lower)
{
    static_assert((std::is_integral<TUpper>::value && std::is_integral<TLower>::value), "Input argument types must be integers!");
}

template <typename T>
inline uint128_t& uint128_t::operator=(const T& value) noexcept
{
    static_assert(std::is_integral<T>::value, "Input argument type must be an integer!");

    _upper = 0;
    _lower = value;
    return *this;
}

inline uint128_t operator+(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return uint128_t(value1._upper + value2._upper + (((value1._lower + value2._lower) < value1._lower) ? 1 : 0), value1._lower + value2._lower);
}

inline uint128_t operator-(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return uint128_t(value1._upper - value2._upper - (((value1._lower - value2._lower) > value1._lower) ? 1 : 0), value1._lower - value2._lower);
}

inline uint128_t operator*(const uint128_t& value1, const uint128_t& value2) noexcept
{
    // Split values into four 32-bit parts
    uint64_t top[4] = { value1._upper >> 32, value1._upper & 0xFFFFFFFF, value1._lower >> 32, value1._lower & 0xFFFFFFFF };
    uint64_t bottom[4] = { value2._upper >> 32, value2._upper & 0xFFFFFFFF, value2._lower >> 32, value2._lower & 0xFFFFFFFF };
    uint64_t products[4][4];

    // Multiply each component of the values
    for (int y = 3; y > -1; --y)
        for (int x = 3; x > -1; --x)
            products[3 - x][y] = top[x] * bottom[y];

    // First row
    uint64_t fourth32 = (products[0][3] & 0xFFFFFFFF);
    uint64_t third32  = (products[0][2] & 0xFFFFFFFF) + (products[0][3] >> 32);
    uint64_t second32 = (products[0][1] & 0xFFFFFFFF) + (products[0][2] >> 32);
    uint64_t first32  = (products[0][0] & 0xFFFFFFFF) + (products[0][1] >> 32);

    // Second row
    third32  += (products[1][3] & 0xFFFFFFFF);
    second32 += (products[1][2] & 0xFFFFFFFF) + (products[1][3] >> 32);
    first32  += (products[1][1] & 0xFFFFFFFF) + (products[1][2] >> 32);

    // Third row
    second32 += (products[2][3] & 0xFFFFFFFF);
    first32  += (products[2][2] & 0xFFFFFFFF) + (products[2][3] >> 32);

    // Fourth row
    first32  += (products[3][3] & 0xFFFFFFFF);

    // Move carry to the next digit
    third32  += fourth32 >> 32;
    second32 += third32  >> 32;
    first32  += second32 >> 32;

    // Remove carry from the current digit
    fourth32 &= 0xFFFFFFFF;
    third32  &= 0xFFFFFFFF;
    second32 &= 0xFFFFFFFF;
    first32  &= 0xFFFFFFFF;

    // Combine components
    return uint128_t((first32 << 32) | second32, (third32 << 32) | fourth32);
}

inline uint128_t operator/(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return uint128_t::divmod(value1, value2).first;
}

inline uint128_t operator%(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return uint128_t::divmod(value1, value2).second;
}

inline uint128_t operator&(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return uint128_t(value1._upper & value2._upper, value1._lower & value2._lower);
}

inline uint128_t operator|(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return uint128_t(value1._upper | value2._upper, value1._lower | value2._lower);
}

inline uint128_t operator^(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return uint128_t(value1._upper ^ value2._upper, value1._lower ^ value2._lower);
}

inline bool operator==(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return ((value1._upper == value2._upper) && (value1._lower == value2._lower));
}

inline bool operator!=(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return ((value1._upper != value2._upper) || (value1._lower != value2._lower));
}

inline bool operator<(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return (value1._upper == value2._upper) ? (value1._lower < value2._lower) : (value1._upper < value2._upper);
}

inline bool operator>(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return (value1._upper == value2._upper) ? (value1._lower > value2._lower) : (value1._upper > value2._upper);
}

inline bool operator<=(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return ((value1 < value2) || (value1 == value2));
}

inline bool operator>=(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return ((value1 > value2) || (value1 == value2));
}

inline bool operator&&(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return ((value1._upper == value2._upper) && (value1._lower == value2._lower));
}

inline bool operator||(const uint128_t& value1, const uint128_t& value2) noexcept
{
    return ((value1._upper != value2._upper) || (value1._lower != value2._lower));
}

inline uint128_t operator<<(const uint128_t& value1, const uint128_t& value2) noexcept
{
    const uint64_t shift = value2._lower;

    if (((bool)value2._upper) || (shift >= 128))
        return 0;
    else if (shift == 64)
        return uint128_t(value1._lower, 0);
    else if (shift == 0)
        return value1;
    else if (shift < 64)
        return uint128_t((value1._upper << shift) + (value1._lower >> (64 - shift)), value1._lower << shift);
    else if ((128 > shift) && (shift > 64))
        return uint128_t(value1._lower << (shift - 64), 0);
    else
        return 0;
}

inline uint128_t operator>>(const uint128_t& value1, const uint128_t& value2) noexcept
{
    const uint64_t shift = value2._lower;

    if (((bool)value2._upper) || (shift >= 128))
        return 0;
    else if (shift == 64)
        return uint128_t(0, value1._upper);
    else if (shift == 0)
        return value1;
    else if (shift < 64)
        return uint128_t(value1._upper >> shift, (value1._upper << (64 - shift)) + (value1._lower >> shift));
    else if ((128 > shift) && (shift > 64))
        return uint128_t(0, (value1._upper >> (shift - 64)));
    else
        return 0;
}

inline size_t uint128_t::bits() const noexcept
{
    size_t result = 0;

    if (_upper)
    {
        result = 64;
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

inline std::string uint128_t::string(size_t base, size_t length) const
{
    if ((base < 2) || (base > 16))
        throw std::invalid_argument("Base must be in the range [2, 16]");

    std::string out = "";

    if (!(*this))
        out = "0";
    else
    {
        std::pair<uint128_t, uint128_t> qr(*this, 0);
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

inline std::pair<uint128_t, uint128_t> uint128_t::divmod(const uint128_t& x, const uint128_t& y)
{
    if (y == 0)
        throw std::domain_error("Division by 0");
    else if (y == 1)
        return std::pair<uint128_t, uint128_t>(x, 0);
    else if (x == y)
        return std::pair<uint128_t, uint128_t>(1, 0);
    else if ((x == 0) || (x < y))
        return std::pair<uint128_t, uint128_t>(0, x);

    std::pair<uint128_t, uint128_t> result(0, 0);

    for (size_t i = x.bits(); i > 0; --i)
    {
        result.first <<= 1;
        result.second <<= 1;

        if ((x >> (i - 1u)) & 1)
            ++result.second;

        if (result.second >= y)
        {
            result.second -= y;
            ++result.first;
        }
    }

    return result;
}

inline std::ostream& operator<<(std::ostream& os, const uint128_t& value)
{
    if (os.flags() & os.oct)
        os << value.string(8);
    else if (os.flags() & os.dec)
        os << value.string(10);
    else if (os.flags() & os.hex)
        os << value.string(16);
    return os;
}

inline void uint128_t::swap(uint128_t& value) noexcept
{
    using std::swap;
    swap(_upper, value._upper);
    swap(_lower, value._lower);
}

inline void swap(uint128_t& value1, uint128_t& value2) noexcept
{
    value1.swap(value2);
}

} // namespace CppCommon
