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

inline bool uint128_t::operator&&(const uint128_t& value) const noexcept
{
    return ((_upper == value._upper) && (_lower == value._lower));
}

inline bool uint128_t::operator||(const uint128_t& value) const noexcept
{
    return ((_upper != value._upper) || (_lower != value._lower));
}

template <typename T>
inline bool uint128_t::operator&&(const T& value) const noexcept
{
    return ((bool)*this && value);
}

template <typename T>
inline bool uint128_t::operator||(const T& value) const noexcept
{
    return ((bool)*this || value);
}

template <typename T>
inline bool operator&&(const T& value1, const uint128_t& value2) noexcept
{
    return (!value2.upper() && ((uint64_t)value1 == value2.lower()));
}

template <typename T>
inline bool operator||(const T& value1, const uint128_t& value2) noexcept
{
    return (value2.upper() || ((uint64_t)value1 != value2.lower()));
}

inline uint128_t& uint128_t::operator&=(const uint128_t& value) noexcept
{
    _upper &= value._upper;
    _lower &= value._lower;
    return *this;
}

inline uint128_t& uint128_t::operator|=(const uint128_t& value) noexcept
{
    _upper |= value._upper;
    _lower |= value._lower;
    return *this;
}

inline uint128_t& uint128_t::operator^=(const uint128_t& value) noexcept
{
    _upper ^= value._upper;
    _lower ^= value._lower;
    return *this;
}

template <typename T>
inline uint128_t& uint128_t::operator&=(const T& value) noexcept
{
    _upper &= 0;
    _lower &= (uint64_t)value;
    return *this;
}

template <typename T>
inline uint128_t& uint128_t::operator|=(const T& value) noexcept
{
    _upper |= 0;
    _lower |= (uint64_t)value;
    return *this;
}

template <typename T>
inline uint128_t& uint128_t::operator^=(const T& value) noexcept
{
    _upper ^= 0;
    _lower ^= (uint64_t)value;
    return *this;
}

template <typename T>
inline T& operator&=(T& value1, const uint128_t& value2) noexcept
{
    return value1 = (T)(value2 & value1);
}

template <typename T>
inline T& operator|=(T& value1, const uint128_t& value2) noexcept
{
    return value1 = (T)(value2 | value1);
}

template <typename T>
inline T& operator^=(T& value1, const uint128_t& value2) noexcept
{
    return value1 = (T)(value2 ^ value1);
}

inline uint128_t uint128_t::operator&(const uint128_t& value) const noexcept
{
    return uint128_t(_upper & value._upper, _lower & value._lower);
}

inline uint128_t uint128_t::operator|(const uint128_t& value) const noexcept
{
    return uint128_t(_upper | value._upper, _lower | value._lower);
}

inline uint128_t uint128_t::operator^(const uint128_t& value) const noexcept
{
    return uint128_t(_upper ^ value._upper, _lower ^ value._lower);
}

template <typename T>
inline uint128_t uint128_t::operator&(const T& value) const noexcept
{
    return uint128_t(0, _lower & (uint64_t)value);
}

template <typename T>
inline uint128_t uint128_t::operator|(const T& value) const noexcept
{
    return uint128_t(0, _lower | (uint64_t)value);
}

template <typename T>
inline uint128_t uint128_t::operator^(const T& value) const noexcept
{
    return uint128_t(0, _lower ^ (uint64_t)value);
}

template <typename T>
inline uint128_t operator&(const T& value1, const uint128_t& value2) noexcept
{
    return value2 & value1;
}

template <typename T>
inline uint128_t operator|(const T& value1, const uint128_t& value2) noexcept
{
    return value2 | value1;
}

template <typename T>
inline uint128_t operator^(const T& value1, const uint128_t& value2) noexcept
{
    return value2 ^ value1;
}

inline uint128_t& uint128_t::operator<<=(const uint128_t& value) noexcept
{
    *this = *this << value;
    return *this;
}

inline uint128_t& uint128_t::operator>>=(const uint128_t& value) noexcept
{
    *this = *this >> value;
    return *this;
}

template <typename T>
inline uint128_t& uint128_t::operator<<=(const T& value) noexcept
{
    *this = *this << uint128_t(value);
    return *this;
}

template <typename T>
inline uint128_t& uint128_t::operator>>=(const T& value) noexcept
{
    *this = *this >> uint128_t(value);
    return *this;
}

template <typename T>
inline T& operator<<=(T& value1, const uint128_t& value2) noexcept
{
    value1 = (T)(uint128_t(value1) << value2);
    return value1;
}

template <typename T>
inline T& operator>>=(T& value1, const uint128_t& value2) noexcept
{
    value1 = (T)(uint128_t(value1) >> value2);
    return value1;
}

inline uint128_t uint128_t::operator<<(const uint128_t& value) const noexcept
{
    const uint64_t shift = value._lower;

    if (((bool)value._upper) || (shift >= 128))
        return 0;
    else if (shift == 64)
        return uint128_t(_lower, 0);
    else if (shift == 0)
        return *this;
    else if (shift < 64)
        return uint128_t((_upper << shift) + (_lower >> (64 - shift)), _lower << shift);
    else if ((128 > shift) && (shift > 64))
        return uint128_t(_lower << (shift - 64), 0);
    else
        return 0;
}

inline uint128_t uint128_t::operator>>(const uint128_t& value) const noexcept
{
    const uint64_t shift = value._lower;

    if (((bool)value._upper) || (shift >= 128))
        return 0;
    else if (shift == 64)
        return uint128_t(0, _upper);
    else if (shift == 0)
        return *this;
    else if (shift < 64)
        return uint128_t(_upper >> shift, (_upper << (64 - shift)) + (_lower >> shift));
    else if ((128 > shift) && (shift > 64))
        return uint128_t(0, (_upper >> (shift - 64)));
    else
        return 0;
}

template <typename T>
inline uint128_t uint128_t::operator<<(const T& value) const noexcept
{
    return *this << uint128_t(value);
}

template <typename T>
inline uint128_t uint128_t::operator>>(const T& value) const noexcept
{
    return *this >> uint128_t(value);
}

template <typename T>
inline uint128_t operator<<(const T& value1, const uint128_t& value2) noexcept
{
    return uint128_t(value1) << value2;
}

template <typename T>
inline uint128_t operator>>(const T& value1, const uint128_t& value2) noexcept
{
    return uint128_t(value1) >> value2;
}

} // namespace CppCommon
