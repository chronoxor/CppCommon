/*!
    \file uuid.inl
    \brief Universally unique identifier (UUID) inline implementation
    \author Ivan Shynkarenka
    \date 18.08.2016
    \copyright MIT License
*/

namespace CppCommon {

inline void swap(UUID& uuid1, UUID& uuid2) noexcept
{
    using std::swap;
    swap(uuid1._data, uuid2._data);
}

} // namespace CppCommon
