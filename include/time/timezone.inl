/*!
    \file timezone.inl
    \brief Time zone inline implementation
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
