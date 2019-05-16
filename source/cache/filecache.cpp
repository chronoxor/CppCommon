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

    // Update the cache entry
    if (timeout.total() > 0)
    {
        Timestamp current = UtcTimestamp();
        _timestamp = (current <= _timestamp) ? _timestamp + 1 : current;
        _entries_by_key.insert(std::make_pair(key, FileCacheEntry(std::move(value), _timestamp + timeout)));
        _entries_by_timeout.insert(std::make_pair(_timestamp + timeout, key));
    }
    else
        _entries_by_key.emplace(std::make_pair(std::move(key), FileCacheEntry(std::move(value))));

    return true;
}

bool FileCache::insert(const std::string& key, const std::string& value, const Timespan& timeout)
{
    std::unique_lock<std::shared_mutex> locker(_lock);

    // Try to find and remove the previous key
    remove_internal(key);

    // Update the cache entry
    if (timeout.total() > 0)
    {
        Timestamp current = UtcTimestamp();
        _timestamp = (current <= _timestamp) ? _timestamp + 1 : current;
        _entries_by_key.insert(std::make_pair(key, FileCacheEntry(value, _timestamp + timeout)));
        _entries_by_timeout.insert(std::make_pair(_timestamp + timeout, key));
    }
    else
        _entries_by_key.insert(std::make_pair(key, FileCacheEntry(value)));

    return true;
}

bool FileCache::setup(const CppCommon::Path& path, const std::string& prefix, const Timespan& timeout, const InsertHandler& handler)
{
    // Setup the cache path
    if (!setup_internal(path, prefix, timeout, handler))
        return false;

    std::unique_lock<std::shared_mutex> locker(_lock);

    // Update the cache path timeout
    if (timeout.total() > 0)
    {
        Timestamp current = UtcTimestamp();
        _timestamp = (current <= _timestamp) ? _timestamp + 1 : current;
        _paths_by_timeout.insert(std::make_pair(_timestamp + timeout, std::make_tuple(path, prefix, timeout, handler)));
    }

    return true;
}

bool FileCache::setup_internal(const CppCommon::Path& path, const std::string& prefix, const Timespan& timeout, const InsertHandler& handler)
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
                // Recursively setup sub-directory
                if (!setup_internal(entry, key, timeout, handler))
                    return false;
            }
            else
            {
                try
                {
                    // Load the cache file content
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

    timeout = it->second.timeout;
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
    if (it->second.timeout.total() > 0)
        _entries_by_timeout.erase(it->second.timeout);

    // Erase cache entry
    _entries_by_key.erase(it);

    return true;
}

void FileCache::clear()
{
    std::unique_lock<std::shared_mutex> locker(_lock);

    // Clear all cache entries
    _entries_by_key.clear();
    _entries_by_timeout.clear();
}

void FileCache::watchdog(const UtcTimestamp& utc)
{
    // Watchdog for cache entries
    auto it_entry_by_timeout = _entries_by_timeout.begin();
    while (it_entry_by_timeout != _entries_by_timeout.end())
    {
        // Check for the cache entry timeout
        if (it_entry_by_timeout->first <= utc)
        {
            // Erase the cache entry with timeout
            auto it_entry_by_key = _entries_by_key.find(it_entry_by_timeout->second);
            _entries_by_key.erase(it_entry_by_key);
            _entries_by_timeout.erase(it_entry_by_timeout);
            it_entry_by_timeout = _entries_by_timeout.begin();
            continue;
        }
        else
            break;
    }

    // Watchdog for cache paths
    auto it_path_by_timeout = _paths_by_timeout.begin();
    while (it_path_by_timeout != _paths_by_timeout.end())
    {
        // Check for the cache entry timeout
        if (it_path_by_timeout->first <= utc)
        {
            // Update the cache path with timeout
            auto path = std::get<0>(it_path_by_timeout->second);
            auto prefix = std::get<1>(it_path_by_timeout->second);
            auto timeout = std::get<2>(it_path_by_timeout->second);
            auto handler = std::get<3>(it_path_by_timeout->second);
            _paths_by_timeout.erase(it_path_by_timeout);
            setup(path, prefix, timeout, handler);
            it_path_by_timeout = _paths_by_timeout.begin();
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
    swap(_entries_by_timeout, cache._entries_by_timeout);
    swap(_paths_by_timeout, cache._paths_by_timeout);
}

} // namespace CppCommon
