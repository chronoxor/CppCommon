/*!
    \file timestamp.inl
    \brief Timestamp inline implementation
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

namespace CppCommon {

inline void Timestamp::swap(Timestamp& timestamp) noexcept
{
    using std::swap;
    swap(_timestamp, timestamp._timestamp);
}

inline void swap(Timestamp& timestamp1, Timestamp& timestamp2) noexcept
{
    timestamp1.swap(timestamp2);
}

} // namespace CppCommon

template <>
struct std::hash<CppCommon::Timestamp>
{
    typedef CppCommon::Timestamp argument_type;
    typedef size_t result_type;

    result_type operator() (const argument_type& value) const
    {
        result_type result = 17;
        result = result * 31 + std::hash<int64_t>()(value.total());
        return result;
    }
};
