/*!
    \file memcache.inl
    \brief Memory cache inline implementation
    \author Ivan Shynkarenka
    \date 14.05.2019
    \copyright MIT License
*/

namespace CppCommon {

template <typename TKey, typename TValue>
inline bool MemCache<TKey, TValue>::empty() const
{
    std::shared_lock<std::shared_mutex> locker(_lock);
    return _entries_by_key.empty();
}

template <typename TKey, typename TValue>
inline size_t MemCache<TKey, TValue>::size() const
{
    std::shared_lock<std::shared_mutex> locker(_lock);
    return _entries_by_key.size();
}

template <typename TKey, typename TValue>
inline bool MemCache<TKey, TValue>::emplace(TKey&& key, TValue&& value, const Timespan& timeout)
{
    std::unique_lock<std::shared_mutex> locker(_lock);

    // Try to find and remove the previous key
    remove_internal(key);

    // Update the cache entry
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

template <typename TKey, typename TValue>
inline bool MemCache<TKey, TValue>::insert(const TKey& key, const TValue& value, const Timespan& timeout)
{
    std::unique_lock<std::shared_mutex> locker(_lock);

    // Try to find and remove the previous key
    remove_internal(key);

    // Update the cache entry
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

template <typename TKey, typename TValue>
inline bool MemCache<TKey, TValue>::find(const TKey& key)
{
    std::shared_lock<std::shared_mutex> locker(_lock);

    // Try to find the given key
    auto it = _entries_by_key.find(key);
    if (it == _entries_by_key.end())
        return false;

    return true;
}

template <typename TKey, typename TValue>
inline bool MemCache<TKey, TValue>::find(const TKey& key, TValue& value)
{
    std::shared_lock<std::shared_mutex> locker(_lock);

    // Try to find the given key
    auto it = _entries_by_key.find(key);
    if (it == _entries_by_key.end())
        return false;

    value = it->second.value;
    return true;
}

template <typename TKey, typename TValue>
inline bool MemCache<TKey, TValue>::find(const TKey& key, TValue& value, Timestamp& timeout)
{
    std::shared_lock<std::shared_mutex> locker(_lock);

    // Try to find the given key
    auto it = _entries_by_key.find(key);
    if (it == _entries_by_key.end())
        return false;

    value = it->second.value;
    timeout = it->second.timestamp + it->second.timespan;
    return true;
}

template <typename TKey, typename TValue>
inline bool MemCache<TKey, TValue>::remove(const TKey& key)
{
    std::unique_lock<std::shared_mutex> locker(_lock);

    return remove_internal(key);
}

template <typename TKey, typename TValue>
inline bool MemCache<TKey, TValue>::remove_internal(const TKey& key)
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

template <typename TKey, typename TValue>
inline void MemCache<TKey, TValue>::clear()
{
    std::unique_lock<std::shared_mutex> locker(_lock);

    // Clear all cache entries
    _entries_by_key.clear();
    _entries_by_timestamp.clear();
}

template <typename TKey, typename TValue>
inline void MemCache<TKey, TValue>::watchdog(const UtcTimestamp& utc)
{
    std::unique_lock<std::shared_mutex> locker(_lock);

    // Watchdog for cache entries
    auto it_entry_by_timestamp = _entries_by_timestamp.begin();
    while (it_entry_by_timestamp != _entries_by_timestamp.end())
    {
        // Check for the cache entry timeout
        auto it_entry_by_key = _entries_by_key.find(it_entry_by_timestamp->second);
        if ((it_entry_by_key->second.timestamp + it_entry_by_key->second.timespan) <= utc)
        {
            // Erase the cache entry with timeout
            _entries_by_key.erase(it_entry_by_key);
            _entries_by_timestamp.erase(it_entry_by_timestamp);
            it_entry_by_timestamp = _entries_by_timestamp.begin();
            continue;
        }
        else
            break;
    }
}

template <typename TKey, typename TValue>
inline void MemCache<TKey, TValue>::swap(MemCache& cache) noexcept
{
    std::unique_lock<std::shared_mutex> locker1(_lock);
    std::unique_lock<std::shared_mutex> locker2(cache._lock);

    using std::swap;
    swap(_timestamp, cache._timestamp);
    swap(_entries_by_key, cache._entries_by_key);
    swap(_entries_by_timestamp, cache._entries_by_timestamp);
}

template <typename TKey, typename TValue>
inline void swap(MemCache<TKey, TValue>& cache1, MemCache<TKey, TValue>& cache2) noexcept
{
    cache1.swap(cache2);
}

} // namespace CppCommon
