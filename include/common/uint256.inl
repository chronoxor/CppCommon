/*!
    \file uint256.inl
    \brief Unsigned 256-bit integer type inline implementation
    \author Ivan Shynkarenka
    \date 11.10.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename TValue>
inline uint256_t::uint256_t(const TValue& value) noexcept
    : _upper(0), _lower(value)
{
    static_assert(std::is_integral<TValue>::value, "Input argument type must be an integer!");
}

template <typename TUpper, typename TLower>
uint256_t::uint256_t(const TUpper& upper, const TLower& lower) noexcept
    : _upper(upper), _lower(lower)
{
    static_assert((std::is_integral<TUpper>::value && std::is_integral<TLower>::value), "Input argument types must be integers!");
}


template <typename TUpperUpper, typename TUpperLower, typename TLowerUpper, typename TLowerLower>
uint256_t::uint256_t(const TUpperUpper& upper_upper, const TUpperLower& upper_lower, const TLowerUpper& lower_upper, const TLowerLower& lower_lower) noexcept
    : _upper(upper_upper, upper_lower), _lower(lower_upper, lower_lower)
{
    static_assert((std::is_integral<TUpperUpper>::value && std::is_integral<TUpperLower>::value && std::is_integral<TLowerUpper>::value && std::is_integral<TLowerLower>::value), "Input argument types must be integers!");
}

template <typename TValue>
uint256_t& uint256_t::operator=(const TValue& value) noexcept
{
    static_assert(std::is_integral<TValue>::value, "Input argument type must be an integer!");

    _upper = 0;
    _lower = value;
    return *this;
}

} // namespace CppCommon
