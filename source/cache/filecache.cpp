/*!
    \file filecache.cpp
    \brief File cache implementation
    \author Ivan Shynkarenka
    \date 14.05.2019
    \copyright MIT License
*/

#include "cache/filecache.h"

namespace CppCommon {

bool FileCache::emplace(std::string&& key, std::string&& value, const Timespan& timeout)
{
    std::unique_lock<std::shared_mutex> locker(_lock);

    // Try to find and remove the previous key
    remove_internal(key);

    if (timeout.total() > 0)
    {
        Timestamp current = UtcTimestamp();
        _timestamp = (current <= _timestamp) ? _timestamp + 1 : current;
        _entries_by_key.insert(std::make_pair(key, MemCacheEntry(std::move(value), _timestamp, timeout)));
        _entries_by_timestamp.insert(std::make_pair(_timestamp, key));
    }
    else
        _entries_by_key.emplace(std::make_pair(std::move(key), MemCacheEntry(std::move(value))));

    return true;
}

bool FileCache::insert(const std::string& key, const std::string& value, const Timespan& timeout)
{
    std::unique_lock<std::shared_mutex> locker(_lock);

    // Try to find and remove the previous key
    remove_internal(key);

    if (timeout.total() > 0)
    {
        Timestamp current = UtcTimestamp();
        _timestamp = (current <= _timestamp) ? _timestamp + 1 : current;
        _entries_by_key.insert(std::make_pair(key, MemCacheEntry(value, _timestamp, timeout)));
        _entries_by_timestamp.insert(std::make_pair(_timestamp, key));
    }
    else
        _entries_by_key.insert(std::make_pair(key, MemCacheEntry(value)));

    return true;
}

bool FileCache::insert_path(const CppCommon::Path& path, const std::string& prefix, const Timespan& timeout, const std::function<bool (FileCache& cache, const std::string& key, const std::string& value, const Timespan& timeout)>& handler)
{
    try
    {
        const std::string key_prefix = (prefix.empty() || (prefix == "/")) ? "/" : (prefix + "/");

        // Iterate through all directory entries
        for (const auto& entry : CppCommon::Directory(path))
        {
            const std::string key = key_prefix + CppCommon::Encoding::URLDecode(entry.filename().string());

            if (entry.IsDirectory())
            {
                // Recursively load sub-directory
                if (!insert_path(entry, key, timeout, handler))
                    return false;
            }
            else
            {
                try
                {
                    // Load file content
                    auto content = CppCommon::File::ReadAllBytes(entry);
                    std::string value(content.begin(), content.end());
                    if (!handler(*this, key, value, timeout))
                        return false;
                }
                catch (const CppCommon::FileSystemException&) { return false; }
            }
        }

        return true;
    }
    catch (const CppCommon::FileSystemException&) { return false; }
}

std::pair<bool, std::string_view> FileCache::find(const std::string& key)
{
    std::shared_lock<std::shared_mutex> locker(_lock);

    // Try to find the given key
    auto it = _entries_by_key.find(key);
    if (it == _entries_by_key.end())
        return std::make_pair(false, std::string_view());

    return std::make_pair(true, std::string_view(it->second.value));
}

std::pair<bool, std::string_view> FileCache::find(const std::string& key, Timestamp& timeout)
{
    std::shared_lock<std::shared_mutex> locker(_lock);

    // Try to find the given key
    auto it = _entries_by_key.find(key);
    if (it == _entries_by_key.end())
        return std::make_pair(false, std::string_view());

    timeout = Timestamp((it->second.timestamp + it->second.timespan).total());
    return std::make_pair(true, std::string_view(it->second.value));
}

bool FileCache::remove(const std::string& key)
{
    std::unique_lock<std::shared_mutex> locker(_lock);

    return remove_internal(key);
}

bool FileCache::remove_internal(const std::string& key)
{
    // Try to find the given key
    auto it = _entries_by_key.find(key);
    if (it == _entries_by_key.end())
        return false;

    // Try to erase cache entry by timestamp
    if (it->second.timestamp.total() > 0)
        _entries_by_timestamp.erase(it->second.timestamp);

    // Erase cache entry
    _entries_by_key.erase(it);

    return true;
}

void FileCache::clear()
{
    std::unique_lock<std::shared_mutex> locker(_lock);

    // Clear all cache entries
    _entries_by_key.clear();
    _entries_by_timestamp.clear();
}

void FileCache::watchdog(const UtcTimestamp& utc)
{
    auto it_by_timestamp = _entries_by_timestamp.begin();
    while (it_by_timestamp != _entries_by_timestamp.end())
    {
        auto it_by_key = _entries_by_key.find(it_by_timestamp->second);

        // Check for the cache entry timeout
        if ((it_by_key->second.timestamp + it_by_key->second.timespan) <= utc)
        {
            _entries_by_key.erase(it_by_key);
            _entries_by_timestamp.erase(it_by_timestamp);
            it_by_timestamp = _entries_by_timestamp.begin();
            continue;
        }
        else
            break;
    }
}

void FileCache::swap(FileCache& cache) noexcept
{
    std::unique_lock<std::shared_mutex> locker1(_lock);
    std::unique_lock<std::shared_mutex> locker2(cache._lock);

    using std::swap;
    swap(_timestamp, cache._timestamp);
    swap(_entries_by_key, cache._entries_by_key);
    swap(_entries_by_timestamp, cache._entries_by_timestamp);
}

} // namespace CppCommon
