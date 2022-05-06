/*!
    \file timezone.inl
    \brief Timezone inline implementation
    \author Ivan Shynkarenka
    \date 18.07.2016
    \copyright MIT License
*/

namespace CppCommon {

inline void Timezone::swap(Timezone& timezone) noexcept
{
    using std::swap;
    swap(_name, timezone._name);
    swap(_offset, timezone._offset);
    swap(_dstoffset, timezone._dstoffset);
}

inline void swap(Timezone& timezone1, Timezone& timezone2) noexcept
{
    timezone1.swap(timezone2);
}

} // namespace CppCommon

template <>
struct std::hash<CppCommon::Timezone>
{
    typedef CppCommon::Timezone argument_type;
    typedef size_t result_type;

    result_type operator() (const argument_type& value) const
    {
        result_type result = 17;
        result = result * 31 + std::hash<std::string>()(value.name());
        result = result * 31 + std::hash<CppCommon::Timespan>()(value.offset());
        result = result * 31 + std::hash<CppCommon::Timespan>()(value.daylight());
        return result;
    }
};
