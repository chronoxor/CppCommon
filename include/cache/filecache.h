/*!
    \file filecache.h
    \brief File cache definition
    \author Ivan Shynkarenka
    \date 14.05.2019
    \copyright MIT License
*/

#ifndef CPPCOMMON_CACHE_FILECACHE_H
#define CPPCOMMON_CACHE_FILECACHE_H

#include "filesystem/directory.h"
#include "filesystem/file.h"
#include "filesystem/path.h"
#include "time/timespan.h"
#include "time/timestamp.h"

#include <functional>
#include <map>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <unordered_map>

namespace CppCommon {

//! File cache
/*!
    File cache is used to cache files in memory with optional
    timeouts.

    Thread-safe.
*/
class FileCache
{
public:
    FileCache() = default;
    FileCache(const FileCache&) = delete;
    FileCache(FileCache&&) = delete;
    ~FileCache() = default;

    FileCache& operator=(const FileCache&) = delete;
    FileCache& operator=(FileCache&&) = delete;

    //! Check if the file cache is not empty
    explicit operator bool() const { return !empty(); }

    //! Is the file cache empty?
    bool empty() const;

    //! Get the file cache size
    size_t size() const;

    //! Emplace a new cache value with the given timeout into the file cache
    /*!
        \param key - Key to emplace
        \param value - Value to emplace
        \param timeout - Cache timeout (default is 0 - no timeout)
        \return 'true' if the cache value was emplaced, 'false' if the given key was not emplaced
    */
    bool emplace(std::string&& key, std::string&& value, const Timespan& timeout = Timespan(0));

    //! Insert a new cache value with the given timeout into the file cache
    /*!
        \param key - Key to insert
        \param value - Value to insert
        \param timeout - Cache timeout (default is 0 - no timeout)
        \return 'true' if the cache value was inserted, 'false' if the given key was not inserted
    */
    bool insert(const std::string& key, const std::string& value, const Timespan& timeout = Timespan(0));

    //! Insert a new cache path with the given timeout into the file cache
    /*!
        \param path - Path to insert
        \param prefix - Cache prefix (default is "/")
        \param timeout - Cache timeout (default is 0 - no timeout)
        \param handler - Cache insert handler (default is 'return cache.insert(key, value, timeout)')
        \return 'true' if the cache value was inserted, 'false' if the given key was not inserted
    */
    bool insert_path(const CppCommon::Path& path, const std::string& prefix = "/", const Timespan& timeout = Timespan(0), const std::function<bool (FileCache& cache, const std::string& key, const std::string& value, const Timespan& timeout)>& handler = [](FileCache& cache, const std::string& key, const std::string& value, const Timespan& timeout){ return cache.insert(key, value, timeout); });

    //! Try to find the cache value by the given key
    /*!
        \param key - Key to find
        \return 'true' if the cache value was found, 'false' if the given key was not found
    */
    std::pair<bool, std::string_view> find(const std::string& key);
    //! Try to find the cache value with timeout by the given key
    /*!
        \param key - Key to find
        \param value - Value to find
        \param timeout - Cache timeout value
        \return 'true' if the cache value was found, 'false' if the given key was not found
    */
    std::pair<bool, std::string_view> find(const std::string& key, Timestamp& timeout);

    //! Remove a cache value with the given key from the file cache
    /*!
        \param key - Key to remove
        \return 'true' if the cache value was removed, 'false' if the given key was not found
    */
    bool remove(const std::string& key);

    //! Clear the memory cache
    void clear();

    //! Watchdog the file cache
    void watchdog(const UtcTimestamp& utc = UtcTimestamp());

    //! Swap two instances
    void swap(FileCache& cache) noexcept;
    friend void swap(FileCache& cache1, FileCache& cache2) noexcept;

private:
    mutable std::shared_mutex _lock;
    Timestamp _timestamp;

    struct MemCacheEntry
    {
        Timestamp timestamp;
        Timespan timespan;
        std::string value;

        MemCacheEntry(const std::string& v, const Timestamp& stamp = Timestamp(), const Timespan& span = Timespan()) : timestamp(stamp), timespan(span), value(v) {}
        MemCacheEntry(std::string&& v, const Timestamp& stamp = Timestamp(), const Timespan& span = Timespan()) : timestamp(stamp), timespan(span), value(v) {}
    };

    std::unordered_map<std::string, MemCacheEntry> _entries_by_key;
    std::map<Timestamp, std::string> _entries_by_timestamp;
    std::map<Timestamp, CppCommon::Path> _path_by_timestamp;

    bool remove_internal(const std::string& key);
};

/*! \example cache_filecache.cpp File cache example */

} // namespace CppCommon

#include "filecache.inl"

#endif // CPPCOMMON_CACHE_FILECACHE_H
