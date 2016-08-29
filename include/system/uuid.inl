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
