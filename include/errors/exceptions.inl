/*!
    \file include/errors/exceptions.inl
    \brief Exceptions inline implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#if defined(FMT_VERSION)
template <>
struct fmt::formatter<CppCommon::Exception> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const CppCommon::Exception& value, FormatContext& ctx) const
    {
        return fmt::formatter<string_view>::format(value.string(), ctx);
    }
};
#endif
