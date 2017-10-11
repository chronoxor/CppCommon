/*!
    \file uint128.inl
    \brief Unsigned 128-bit integer type inline implementation
    \author Ivan Shynkarenka
    \date 11.10.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename TValue>
inline uint128_t::uint128_t(const TValue& value) noexcept
    : _upper(0), _lower(value)
{
    static_assert(std::is_integral<TValue>::value, "Input argument type must be an integer!");
}

template <typename TUpper, typename TLower>
uint128_t::uint128_t(const TUpper& upper, const TLower& lower) noexcept
    : _upper(upper), _lower(lower)
{
    static_assert((std::is_integral<TUpper>::value && std::is_integral<TLower>::value), "Input argument types must be integers!");
}

template <typename TValue>
uint128_t& uint128_t::operator=(const TValue& value) noexcept
{
    static_assert(std::is_integral<TValue>::value, "Input argument type must be an integer!");

    _upper = 0;
    _lower = value;
    return *this;
}

} // namespace CppCommon
