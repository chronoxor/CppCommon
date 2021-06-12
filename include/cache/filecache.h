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
#include <mutex>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>

namespace CppCommon {

//! File cache
/*!
    File cache is used to cache files in memory with optional timeouts.

    Thread-safe.
*/
class FileCache
{
public:
    //! File cache insert handler type
    typedef std::function<bool (FileCache& cache, const std::string& key, const std::string& value, const Timespan& timeout)> InsertHandler;

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

    //! Try to find the cache value by the given key
    /*!
        \param key - Key to find
        \return 'true' if the cache value was found, 'false' if the given key was not found
    */
    std::pair<bool, std::string_view> find(const std::string& key);
    //! Try to find the cache value with timeout by the given key
    /*!
        \param key - Key to find
        \param timeout - Cache timeout value
        \return 'true' if the cache value was found, 'false' if the given key was not found
    */
    std::pair<bool, std::string_view> find(const std::string& key, Timestamp& timeout);

    //! Remove the cache value with the given key from the file cache
    /*!
        \param key - Key to remove
        \return 'true' if the cache value was removed, 'false' if the given key was not found
    */
    bool remove(const std::string& key);

    //! Insert a new cache path with the given timeout into the file cache
    /*!
        \param path - Path to insert
        \param prefix - Cache prefix (default is "/")
        \param timeout - Cache timeout (default is 0 - no timeout)
        \param handler - Cache insert handler (default is 'return cache.insert(key, value, timeout)')
        \return 'true' if the cache path was setup, 'false' if failed to setup the cache path
    */
    bool insert_path(const CppCommon::Path& path, const std::string& prefix = "/", const Timespan& timeout = Timespan(0), const InsertHandler& handler = [](FileCache& cache, const std::string& key, const std::string& value, const Timespan& timeout){ return cache.insert(key, value, timeout); });

    //! Try to find the cache path
    /*!
        \param path - Path to find
        \return 'true' if the cache path was found, 'false' if the given path was not found
    */
    bool find_path(const CppCommon::Path& path);
    //! Try to find the cache path with timeout
    /*!
        \param path - Path to find
        \param timeout - Cache timeout value
        \return 'true' if the cache path was found, 'false' if the given path was not found
    */
    bool find_path(const CppCommon::Path& path, Timestamp& timeout);

    //! Remove the cache path from the file cache
    /*!
        \param path - Path to remove
        \return 'true' if the cache path was removed, 'false' if the given path was not found
    */
    bool remove_path(const CppCommon::Path& path);

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
        std::string value;
        Timestamp timestamp;
        Timespan timespan;

        MemCacheEntry() = default;
        MemCacheEntry(const std::string& v, const Timestamp& ts = Timestamp(), const Timespan& tp = Timespan()) : value(v), timestamp(ts), timespan(tp) {}
        MemCacheEntry(std::string&& v, const Timestamp& ts = Timestamp(), const Timespan& tp = Timespan()) : value(v), timestamp(ts), timespan(tp) {}
    };

    struct FileCacheEntry
    {
        std::string prefix;
        InsertHandler handler;
        Timestamp timestamp;
        Timespan timespan;

        FileCacheEntry() = default;
        FileCacheEntry(const std::string& pfx, const InsertHandler& h, const Timestamp& ts = Timestamp(), const Timespan& tp = Timespan()) : prefix(pfx), handler(h), timestamp(ts), timespan(tp) {}
    };

    std::unordered_map<std::string, MemCacheEntry> _entries_by_key;
    std::map<Timestamp, std::string> _entries_by_timestamp;
    std::map<CppCommon::Path, FileCacheEntry> _paths_by_key;
    std::map<Timestamp, CppCommon::Path> _paths_by_timestamp;

    bool remove_internal(const std::string& key);
    bool insert_path_internal(const CppCommon::Path& path, const std::string& prefix, const Timespan& timeout, const InsertHandler& handler);
    bool remove_path_internal(const CppCommon::Path& path);
};

/*! \example cache_filecache.cpp File cache example */

} // namespace CppCommon

#include "filecache.inl"

#endif // CPPCOMMON_CACHE_FILECACHE_H
