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

inline bool uint256_t::operator&&(const uint128_t& value) const noexcept
{
    return ((bool)*this && value);
}

inline bool uint256_t::operator&&(const uint256_t& value) const noexcept
{
    return ((_upper == value._upper) && (_lower == value._lower));
}

inline bool uint256_t::operator||(const uint128_t& value) const noexcept
{
    return ((bool)*this || value);
}

inline bool uint256_t::operator||(const uint256_t& value) const noexcept
{
    return ((_upper != value._upper) || (_lower != value._lower));
}

template <typename T>
inline bool uint256_t::operator&&(const T& value) const noexcept
{
    return ((bool)*this && value);
}

template <typename T>
inline bool uint256_t::operator||(const T& value) const noexcept
{
    return ((bool)*this || value);
}

template <typename T>
inline bool operator&&(const T& value1, const uint256_t& value2) noexcept
{
    return (!value2.upper() && ((uint64_t)value1 == value2.lower()));
}

template <typename T>
inline bool operator||(const T& value1, const uint256_t& value2) noexcept
{
    return (value2.upper() || ((uint64_t)value1 != value2.lower()));
}

inline uint256_t& uint256_t::operator&=(const uint128_t& value) noexcept
{
    _upper &= 0;
    _lower &= value;
    return *this;
}

inline uint256_t& uint256_t::operator&=(const uint256_t& value) noexcept
{
    _upper &= value._upper;
    _lower &= value._lower;
    return *this;
}

inline uint256_t& uint256_t::operator|=(const uint128_t& value) noexcept
{
    _upper |= 0;
    _lower |= value;
    return *this;
}

inline uint256_t& uint256_t::operator|=(const uint256_t& value) noexcept
{
    _upper |= value._upper;
    _lower |= value._lower;
    return *this;
}

inline uint256_t& uint256_t::operator^=(const uint128_t& value) noexcept
{
    _upper ^= 0;
    _lower ^= value;
    return *this;
}

inline uint256_t& uint256_t::operator^=(const uint256_t& value) noexcept
{
    _upper ^= value._upper;
    _lower ^= value._lower;
    return *this;
}

template <typename T>
inline uint256_t& uint256_t::operator&=(const T& value) noexcept
{
    _upper &= 0;
    _lower &= (uint128_t)value;
    return *this;
}

template <typename T>
inline uint256_t& uint256_t::operator|=(const T& value) noexcept
{
    _upper |= 0;
    _lower |= (uint128_t)value;
    return *this;
}

template <typename T>
inline uint256_t& uint256_t::operator^=(const T& value) noexcept
{
    _upper ^= 0;
    _lower ^= (uint128_t)value;
    return *this;
}

template <typename T>
inline T& operator&=(T& value1, const uint256_t& value2) noexcept
{
    return value1 = (T)(value2 & value1);
}

template <typename T>
inline T& operator|=(T& value1, const uint256_t& value2) noexcept
{
    return value1 = (T)(value2 | value1);
}

template <typename T>
inline T& operator^=(T& value1, const uint256_t& value2) noexcept
{
    return value1 = (T)(value2 ^ value1);
}

inline uint256_t uint256_t::operator&(const uint128_t& value) const noexcept
{
    return uint256_t(0, _lower & value);
}

inline uint256_t uint256_t::operator&(const uint256_t& value) const noexcept
{
    return uint256_t(_upper & value._upper, _lower & value._lower);
}

inline uint256_t uint256_t::operator|(const uint128_t& value) const noexcept
{
    return uint256_t(0, _lower | value);
}

inline uint256_t uint256_t::operator|(const uint256_t& value) const noexcept
{
    return uint256_t(_upper | value._upper, _lower | value._lower);
}

inline uint256_t uint256_t::operator^(const uint128_t& value) const noexcept
{
    return uint256_t(0, _lower ^ value);
}

inline uint256_t uint256_t::operator^(const uint256_t& value) const noexcept
{
    return uint256_t(_upper ^ value._upper, _lower ^ value._lower);
}

template <typename T>
inline uint256_t uint256_t::operator&(const T& value) const noexcept
{
    return uint256_t(0, _lower & (uint128_t)value);
}

template <typename T>
inline uint256_t uint256_t::operator|(const T& value) const noexcept
{
    return uint256_t(0, _lower | (uint128_t)value);
}

template <typename T>
inline uint256_t uint256_t::operator^(const T& value) const noexcept
{
    return uint256_t(0, _lower ^ (uint128_t)value);
}

template <typename T>
inline uint256_t operator&(const T& value1, const uint256_t& value2) noexcept
{
    return value2 & value1;
}

template <typename T>
inline uint256_t operator|(const T& value1, const uint256_t& value2) noexcept
{
    return value2 | value1;
}

template <typename T>
inline uint256_t operator^(const T& value1, const uint256_t& value2) noexcept
{
    return value2 ^ value1;
}

inline uint256_t& uint256_t::operator<<=(const uint128_t& value) noexcept
{
    return *this <<= uint256_t(value);
}

inline uint256_t& uint256_t::operator<<=(const uint256_t& value) noexcept
{
    *this = *this << value;
    return *this;
}

inline uint256_t& uint256_t::operator>>=(const uint128_t& value) noexcept
{
    return *this >>= uint256_t(value);
}

inline uint256_t& uint256_t::operator>>=(const uint256_t& value) noexcept
{
    *this = *this >> value;
    return *this;
}

template <typename T>
inline uint256_t& uint256_t::operator<<=(const T& value) noexcept
{
    *this = *this << uint256_t(value);
    return *this;
}

template <typename T>
inline uint256_t& uint256_t::operator>>=(const T& value) noexcept
{
    *this = *this >> uint256_t(value);
    return *this;
}

template <typename T>
inline T& operator<<=(T& value1, const uint256_t& value2) noexcept
{
    value1 = (T)(uint256_t(value1) << value2);
    return value1;
}

template <typename T>
inline T& operator>>=(T& value1, const uint256_t& value2) noexcept
{
    value1 = (T)(uint256_t(value1) >> value2);
    return value1;
}

inline uint256_t uint256_t::operator<<(const uint128_t& value) const noexcept
{
    return *this << uint256_t(value);
}

inline uint256_t uint256_t::operator<<(const uint256_t& value) const noexcept
{
    const uint128_t shift = value._lower;

    if (((bool)value._upper) || (shift >= 256))
        return 0;
    else if (shift == 128)
        return uint256_t(_lower, 0);
    else if (shift == 0)
        return *this;
    else if (shift < 128)
        return uint256_t((_upper << shift) + (_lower >> (128 - shift)), _lower << shift);
    else if ((256 > shift) && (shift > 128))
        return uint256_t(_lower << (shift - 128), 0);
    else
        return 0;
}

inline uint256_t uint256_t::operator>>(const uint128_t& value) const noexcept
{
    return *this >> uint256_t(value);
}

inline uint256_t uint256_t::operator>>(const uint256_t& value) const noexcept
{
    const uint128_t shift = value._lower;

    if (((bool)value._upper) || (shift >= 256))
        return 0;
    else if (shift == 128)
        return uint256_t(_upper);
    else if (shift == 0)
        return *this;
    else if (shift < 128)
        return uint256_t(_upper >> shift, (_upper << (128 - shift)) + (_lower >> shift));
    else if ((256 > shift) && (shift > 128))
        return uint256_t(_upper >> (shift - 128));
    else
        return 0;
}

template <typename T>
inline uint256_t uint256_t::operator<<(const T& value) const noexcept
{
    return *this << uint256_t(value);
}

template <typename T>
inline uint256_t uint256_t::operator>>(const T& value) const noexcept
{
    return *this >> uint256_t(value);
}

template <typename T>
inline uint256_t operator<<(const T& value1, const uint256_t& value2) noexcept
{
    return uint256_t(value1) << value2;
}

template <typename T>
inline uint256_t operator>>(const T& value1, const uint256_t& value2) noexcept
{
    return uint256_t(value1) >> value2;
}

} // namespace CppCommon
