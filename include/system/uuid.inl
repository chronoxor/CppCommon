/*!
    \file uuid.inl
    \brief Universally unique identifier (UUID) inline implementation
    \author Ivan Shynkarenka
    \date 18.08.2016
    \copyright MIT License
*/

namespace CppCommon {

inline void UUID::swap(UUID& uuid) noexcept
{
    using std::swap;
    swap(_data, uuid._data);
}

inline void swap(UUID& uuid1, UUID& uuid2) noexcept
{
    uuid1.swap(uuid2);
}

} // namespace CppCommon

#if defined(FMT_VERSION)
template <>
struct fmt::formatter<CppCommon::UUID> : formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const CppCommon::UUID& value, FormatContext& ctx) const
    {
        return formatter<string_view>::format(value.string(), ctx);
    }
};
#endif

template <>
struct std::hash<CppCommon::UUID>
{
    typedef CppCommon::UUID argument_type;
    typedef size_t result_type;

    result_type operator() (const argument_type& value) const
    {
        result_type result = 17;
        std::hash<uint8_t> hasher;
        for (size_t i = 0; i < value.data().size(); ++i)
            result = result * 31 + hasher(value.data()[i]);
        return result;
    }
};
