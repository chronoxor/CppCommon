/*!
    \file memcache.h
    \brief Memory cache definition
    \author Ivan Shynkarenka
    \date 14.05.2019
    \copyright MIT License
*/

#ifndef CPPCOMMON_CACHE_MEMCACHE_H
#define CPPCOMMON_CACHE_MEMCACHE_H

#include "time/timespan.h"
#include "time/timestamp.h"

#include <map>
#include <shared_mutex>
#include <unordered_map>

namespace CppCommon {

//! Memory cache
/*!
    Memory cache is used to cache data in memory with optional
    timeouts.

    Thread-safe.
*/
template <typename TKey, typename TValue>
class MemCache
{
public:
    MemCache() = default;
    MemCache(const MemCache&) = default;
    MemCache(MemCache&&) noexcept = default;
    ~MemCache() = default;

    MemCache& operator=(const MemCache&) = default;
    MemCache& operator=(MemCache&&) noexcept = default;

    //! Check if the memory cache is not empty
    explicit operator bool() const noexcept { return !empty(); }

    //! Is the memory cache empty?
    bool empty() const noexcept;

    //! Get the memory cache size
    size_t size() const noexcept;

    //! Insert a new cache value into the memory cache
    /*!
        \param key - Key to insert
        \param value - Value to insert
    */
    void insert(const TKey& key, const TValue& value) noexcept;
    //! Insert a new cache value with the given timeout into the memory cache
    /*!
        \param key - Key to insert
        \param value - Value to insert
        \param timeout - Cache timeout
    */
    void insert(const TKey& key, const TValue& value, const Timespan& timeout) noexcept;

    //! Emplace a new cache value into the memory cache
    /*!
        \param key - Key to emplace
        \param value - Value to emplace
    */
    void emplace(TKey&& key, TValue&& value) noexcept;
    //! Emplace a new cache value with the given timeout into the memory cache
    /*!
        \param key - Key to emplace
        \param value - Value to emplace
        \param timeout - Cache timeout
    */
    void emplace(TKey&& key, TValue&& value, const Timespan& timeout) noexcept;

    //! Try to find the cache value by the given key
    /*!
        \param key - Key to find
        \return 'true' if the cache value was found, 'false' if the given key was not found
    */
    bool find(const TKey& key) noexcept;
    //! Try to find the cache value by the given key
    /*!
        \param key - Key to find
        \param value - Value to find
        \return 'true' if the cache value was found, 'false' if the given key was not found
    */
    bool find(const TKey& key, TValue& value) noexcept;
    //! Try to find the cache value with timeout by the given key
    /*!
        \param key - Key to find
        \param value - Value to find
        \param timeout - Cache timeout value
        \return 'true' if the cache value was found, 'false' if the given key was not found
    */
    bool find(const TKey& key, TValue& value, Timestamp& timeout) noexcept;

    //! Remove a cache value with the given key from the memory cache
    /*!
        \param key - Key to remove
        \return 'true' if the cache value was removed, 'false' if the given key was not found
    */
    bool remove(const TKey& key) noexcept;

    //! Clear the memory cache
    void clear() noexcept;

    //! Watchdog the memory cache
    void watchdog(const UtcTimestamp& utc = UtcTimestamp()) noexcept;

    //! Swap two instances
    void swap(MemCache& cache) noexcept;
    friend void swap(MemCache& cache1, MemCache& cache2) noexcept;

private:
    std::shared_mutex _lock;
    Timestamp _timestamp;

    struct MemCacheEntry
    {
        Timestamp timestamp;
        Timespan timespan;
        TValue value;

        MemCacheEntry(const TValue& value, const Timestamp& timestamp = Timestamp(), const Timespan& timespan = Timespan()) : timestamp(timestamp), timespan(timespan), value(value) {}
        MemCacheEntry(TValue&& value, const Timestamp& timestamp = Timestamp(), const Timespan& timespan = Timespan()) : timestamp(timestamp), timespan(timespan), value(value) {}
    };

    std::unordered_map<TKey, MemCacheEntry> _entries_by_key;
    std::map<Timestamp, TKey> _entries_by_timestamp;

    bool remove_internal(const TKey& key) noexcept;
};

/*! \example cache_memcache.cpp Memory cache example */

} // namespace CppCommon

#include "memcache.inl"

#endif // CPPCOMMON_CACHE_MEMCACHE_H
