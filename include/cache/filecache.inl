/*!
    \file filecache.inl
    \brief File cache inline implementation
    \author Ivan Shynkarenka
    \date 14.05.2019
    \copyright MIT License
*/

namespace CppCommon {

inline bool FileCache::empty() const
{
    std::shared_lock<std::shared_mutex> locker(_lock);
    return _entries_by_key.empty();
}

inline size_t FileCache::size() const
{
    std::shared_lock<std::shared_mutex> locker(_lock);
    return _entries_by_key.size();
}

inline void swap(FileCache& cache1, FileCache& cache2) noexcept
{
    cache1.swap(cache2);
}

} // namespace CppCommon
