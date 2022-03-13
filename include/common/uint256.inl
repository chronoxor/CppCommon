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

inline uint256_t::uint256_t(int8_t value) noexcept
    : _upper(0), _lower(value)
{
}

inline uint256_t::uint256_t(uint8_t value) noexcept
    : _upper(0), _lower(value)
{
}

inline uint256_t::uint256_t(int16_t value) noexcept
    : _upper(0), _lower(value)
{
}

inline uint256_t::uint256_t(uint16_t value) noexcept
    : _upper(0), _lower(value)
{
}

inline uint256_t::uint256_t(int32_t value) noexcept
    : _upper(0), _lower(value)
{
}

inline uint256_t::uint256_t(uint32_t value) noexcept
    : _upper(0), _lower(value)
{
}

inline uint256_t::uint256_t(int64_t value) noexcept
    : _upper(0), _lower(value)
{
}

inline uint256_t::uint256_t(uint64_t value) noexcept
    : _upper(0), _lower(value)
{
}

inline uint256_t::uint256_t(uint128_t value) noexcept
    : _upper(0), _lower(value)
{
}

template <typename T>
inline uint256_t::uint256_t(const T& value) noexcept
    : _upper(0), _lower(value)
{
    static_assert((std::is_integral<T>::value || std::is_same<T, uint128_t>::value || std::is_same<T, uint256_t>::value), "Input argument type must be an integer!");
}

template <typename TUpper, typename TLower>
inline uint256_t::uint256_t(const TUpper& upper, const TLower& lower) noexcept
    : _upper(upper), _lower(lower)
{
    static_assert(((std::is_integral<TUpper>::value || std::is_same<TUpper, uint128_t>::value || std::is_same<TUpper, uint256_t>::value) && (std::is_integral<TLower>::value || std::is_same<TLower, uint128_t>::value || std::is_same<TLower, uint256_t>::value)), "Input argument types must be integers!");
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
    static_assert((std::is_integral<T>::value || std::is_same<T, uint128_t>::value || std::is_same<T, uint256_t>::value), "Input argument type must be an integer!");

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

inline uint256_t operator/(const uint256_t& value1, const uint256_t& value2)
{
    return uint256_t::divmod(value1, value2).first;
}

inline uint256_t operator%(const uint256_t& value1, const uint256_t& value2)
{
    return uint256_t::divmod(value1, value2).second;
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
    return ((bool)value1 && (bool)value2);
}

inline bool operator||(const uint256_t& value1, const uint256_t& value2) noexcept
{
    return ((bool)value1 || (bool)value2);
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

inline std::wostream& operator<<(std::wostream& os, const uint256_t& value)
{
    if (os.flags() & os.oct)
        os << value.wstring(8);
    else if (os.flags() & os.dec)
        os << value.wstring(10);
    else if (os.flags() & os.hex)
        os << value.wstring(16);
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

#if defined(FMT_VERSION)
template <>
struct fmt::formatter<CppCommon::uint256_t> : formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const CppCommon::uint256_t& value, FormatContext& ctx) const
    {
        return formatter<string_view>::format(value.string(10), ctx);
    }
};
#endif

template <>
struct std::hash<CppCommon::uint256_t>
{
    typedef CppCommon::uint256_t argument_type;
    typedef size_t result_type;

    result_type operator() (const argument_type& value) const
    {
        result_type result = 17;
        std::hash<CppCommon::uint128_t> hasher;
        result = result * 31 + hasher(value.upper());
        result = result * 31 + hasher(value.lower());
        return result;
    }
};
