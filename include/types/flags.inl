/*!
    \file flags.inl
    \brief Enum-based flags inline implementation
    \author Ivan Shynkarenka
    \date 25.08.2016
    \copyright MIT License
*/

template <typename TEnum>
constexpr auto operator&(TEnum value1, TEnum value2) noexcept -> typename std::enable_if<CppCommon::IsEnumFlags<TEnum>::value, CppCommon::Flags<TEnum>>::type
{
    return CppCommon::Flags<TEnum>(value1) & value2;
}

template <typename TEnum>
constexpr auto operator|(TEnum value1, TEnum value2) noexcept -> typename std::enable_if<CppCommon::IsEnumFlags<TEnum>::value, CppCommon::Flags<TEnum>>::type
{
    return CppCommon::Flags<TEnum>(value1) | value2;
}

template <typename TEnum>
constexpr auto operator^(TEnum value1, TEnum value2) noexcept -> typename std::enable_if<CppCommon::IsEnumFlags<TEnum>::value, CppCommon::Flags<TEnum>>::type
{
    return CppCommon::Flags<TEnum>(value1) ^ value2;
}
