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

#include <mutex>
#include <map>
#include <shared_mutex>
#include <unordered_map>

namespace CppCommon {

//! Memory cache
/*!
    Memory cache is used to cache data in memory with optional timeouts.

    Thread-safe.
*/
template <typename TKey, typename TValue>
class MemCache
{
public:
    MemCache() = default;
    MemCache(const MemCache&) = delete;
    MemCache(MemCache&&) = delete;
    ~MemCache() = default;

    MemCache& operator=(const MemCache&) = delete;
    MemCache& operator=(MemCache&&) = delete;

    //! Check if the memory cache is not empty
    explicit operator bool() const { return !empty(); }

    //! Is the memory cache empty?
    bool empty() const;

    //! Get the memory cache size
    size_t size() const;

    //! Emplace a new cache value with the given timeout into the memory cache
    /*!
        \param key - Key to emplace
        \param value - Value to emplace
        \param timeout - Cache timeout (default is 0 - no timeout)
        \return 'true' if the cache value was emplaced, 'false' if the given key was not emplaced
    */
    bool emplace(TKey&& key, TValue&& value, const Timespan& timeout = Timespan(0));

    //! Insert a new cache value with the given timeout into the memory cache
    /*!
        \param key - Key to insert
        \param value - Value to insert
        \param timeout - Cache timeout (default is 0 - no timeout)
        \return 'true' if the cache value was inserted, 'false' if the given key was not inserted
    */
    bool insert(const TKey& key, const TValue& value, const Timespan& timeout = Timespan(0));

    //! Try to find the cache value by the given key
    /*!
        \param key - Key to find
        \return 'true' if the cache value was found, 'false' if the given key was not found
    */
    bool find(const TKey& key);
    //! Try to find the cache value by the given key
    /*!
        \param key - Key to find
        \param value - Value to find
        \return 'true' if the cache value was found, 'false' if the given key was not found
    */
    bool find(const TKey& key, TValue& value);
    //! Try to find the cache value with timeout by the given key
    /*!
        \param key - Key to find
        \param value - Value to find
        \param timeout - Cache timeout value
        \return 'true' if the cache value was found, 'false' if the given key was not found
    */
    bool find(const TKey& key, TValue& value, Timestamp& timeout);

    //! Remove the cache value with the given key from the memory cache
    /*!
        \param key - Key to remove
        \return 'true' if the cache value was removed, 'false' if the given key was not found
    */
    bool remove(const TKey& key);

    //! Clear the memory cache
    void clear();

    //! Watchdog the memory cache
    void watchdog(const UtcTimestamp& utc = UtcTimestamp());

    //! Swap two instances
    void swap(MemCache& cache) noexcept;
    template <typename UKey, typename UValue>
    friend void swap(MemCache<UKey, UValue>& cache1, MemCache<UKey, UValue>& cache2) noexcept;

private:
    mutable std::shared_mutex _lock;
    Timestamp _timestamp;

    struct MemCacheEntry
    {
        TValue value;
        Timestamp timestamp;
        Timespan timespan;

        MemCacheEntry() = default;
        MemCacheEntry(const TValue& v, const Timestamp& ts = Timestamp(), const Timespan& tp = Timespan()) : value(v), timestamp(ts), timespan(tp) {}
        MemCacheEntry(TValue&& v, const Timestamp& ts = Timestamp(), const Timespan& tp = Timespan()) : value(v), timestamp(ts), timespan(tp) {}
    };

    std::unordered_map<TKey, MemCacheEntry> _entries_by_key;
    std::map<Timestamp, TKey> _entries_by_timestamp;

    bool remove_internal(const TKey& key);
};

/*! \example cache_memcache.cpp Memory cache example */

} // namespace CppCommon

#include "memcache.inl"

#endif // CPPCOMMON_CACHE_MEMCACHE_H
