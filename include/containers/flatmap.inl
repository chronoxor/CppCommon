/*!
    \file flatmap.inl
    \brief Flat map container inline implementation
    \author Ivan Shynkarenka
    \date 27.07.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline FlatMap<TKey, TValue, TCompare, TAllocator>::FlatMap(size_t capacity, const TCompare& compare, const TAllocator& allocator)
    : _compare(compare), _container(allocator)
{
    reserve(capacity);
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
template <class InputIterator>
inline FlatMap<TKey, TValue, TCompare, TAllocator>::FlatMap(InputIterator first, InputIterator last, bool unused, size_t capacity, const TCompare& compare, const TAllocator& allocator)
    : FlatMap(capacity, compare, allocator)
{
    insert(first, last);
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline FlatMap<TKey, TValue, TCompare, TAllocator>::FlatMap(const FlatMap& flatmap)
    : FlatMap(flatmap.capacity(), flatmap._compare, flatmap._container.get_allocator())
{
    for (const auto& item : flatmap)
        insert(item);
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline FlatMap<TKey, TValue, TCompare, TAllocator>::FlatMap(const FlatMap& flatmap, size_t capacity)
    : FlatMap(capacity, flatmap._compare, flatmap._container.get_allocator())
{
    for (const auto& item : flatmap)
        insert(item);
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline FlatMap<TKey, TValue, TCompare, TAllocator>& FlatMap<TKey, TValue, TCompare, TAllocator>::operator=(const FlatMap& flatmap)
{
    clear();
    reserve(flatmap.size());
    for (const auto& item : flatmap)
        insert(item);
    return *this;
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator FlatMap<TKey, TValue, TCompare, TAllocator>::find(const TKey& key) noexcept
{
    iterator it = lower_bound(key);
    if ((it != end()) && compare(key, it->first))
        return end();
    return it;
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::const_iterator FlatMap<TKey, TValue, TCompare, TAllocator>::find(const TKey& key) const noexcept
{
    const_iterator it = lower_bound(key);
    if ((it != end()) && compare(key, it->first))
        return end();
    return it;
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator FlatMap<TKey, TValue, TCompare, TAllocator>::lower_bound(const TKey& key) noexcept
{
    return std::lower_bound(begin(), end(), key, [this](auto key1, auto key2) { return this->compare(key1, key2); });
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::const_iterator FlatMap<TKey, TValue, TCompare, TAllocator>::lower_bound(const TKey& key) const noexcept
{
    return std::lower_bound(begin(), end(), key, [this](auto key1, auto key2) { return this->compare(key1, key2); });
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator FlatMap<TKey, TValue, TCompare, TAllocator>::upper_bound(const TKey& key) noexcept
{
    return std::upper_bound(begin(), end(), key, [this](auto key1, auto key2) { return this->compare(key1, key2); });
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::const_iterator FlatMap<TKey, TValue, TCompare, TAllocator>::upper_bound(const TKey& key) const noexcept
{
    return std::upper_bound(begin(), end(), key, [this](auto key1, auto key2) { return this->compare(key1, key2); });
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline std::pair<typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator, typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator> FlatMap<TKey, TValue, TCompare, TAllocator>::equal_range(const TKey& key) noexcept
{
    return std::equal_range(begin(), end(), key, [this](auto key1, auto key2) { return this->compare(key1, key2); });
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline std::pair<typename FlatMap<TKey, TValue, TCompare, TAllocator>::const_iterator, typename FlatMap<TKey, TValue, TCompare, TAllocator>::const_iterator> FlatMap<TKey, TValue, TCompare, TAllocator>::equal_range(const TKey& key) const noexcept
{
    return std::equal_range(begin(), end(), key, [this](auto key1, auto key2) { return this->compare(key1, key2); });
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::mapped_type& FlatMap<TKey, TValue, TCompare, TAllocator>::at(const TKey& key) noexcept
{
    auto it = find(key);
    if (it == end())
        throw std::out_of_range("Item with the given key was not found in the flat map!");

    return it->second;
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline const typename FlatMap<TKey, TValue, TCompare, TAllocator>::mapped_type& FlatMap<TKey, TValue, TCompare, TAllocator>::at(const TKey& key) const noexcept
{
    auto it = find(key);
    if (it == end())
        throw std::out_of_range("Item with the given key was not found in the flat map!");

    return it->second;
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline std::pair<typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator, bool> FlatMap<TKey, TValue, TCompare, TAllocator>::insert(const value_type& item)
{
    return emplace_internal(item.first, item.second);
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline std::pair<typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator, bool> FlatMap<TKey, TValue, TCompare, TAllocator>::insert(value_type&& item)
{
    return emplace_internal(item.first, std::move(item.second));
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator FlatMap<TKey, TValue, TCompare, TAllocator>::insert(const const_iterator& position, const value_type& item)
{
    return emplace_hint_internal(position, item.first, item.second);
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator FlatMap<TKey, TValue, TCompare, TAllocator>::insert(const const_iterator& position, value_type&& item)
{
    return emplace_hint_internal(position, item.first, std::move(item.second));
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
template <class InputIterator>
inline void FlatMap<TKey, TValue, TCompare, TAllocator>::insert(InputIterator first, InputIterator last)
{
    for (auto it = first; it != last; ++it)
        insert(*it);
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
template <typename... Args>
inline std::pair<typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator, bool> FlatMap<TKey, TValue, TCompare, TAllocator>::emplace(Args&&... args)
{
    return emplace_internal(std::forward<Args>(args)...);
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
template <typename... Args>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator FlatMap<TKey, TValue, TCompare, TAllocator>::emplace_hint(const const_iterator& position, Args&&... args)
{
    return emplace_hint_internal(position, std::forward<Args>(args)...);
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline size_t FlatMap<TKey, TValue, TCompare, TAllocator>::erase(const TKey& key)
{
    auto it = find(key);
    if (it == end())
        return 0;

    _container.erase(it);
    return 1;
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator FlatMap<TKey, TValue, TCompare, TAllocator>::erase(const const_iterator& position)
{
    iterator result(position);
    ++result;
    _container.erase(position);
    return result;
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator FlatMap<TKey, TValue, TCompare, TAllocator>::erase(const const_iterator& first, const const_iterator& last)
{
    iterator result(last);
    _container.erase(first, last);
    return result;
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
template <typename... Args>
inline std::pair<typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator, bool> FlatMap<TKey, TValue, TCompare, TAllocator>::emplace_internal(const TKey& key, Args&&... args)
{
    bool found = true;
    iterator it = lower_bound(key);
    if ((it == end()) || compare(key, it->first))
    {
        it = _container.emplace(it, std::make_pair(key, TValue(std::forward<Args>(args)...)));
        found = false;
    }
    return std::make_pair(it, !found);
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
template <typename... Args>
inline typename FlatMap<TKey, TValue, TCompare, TAllocator>::iterator FlatMap<TKey, TValue, TCompare, TAllocator>::emplace_hint_internal(const const_iterator& position, const TKey& key, Args&&... args)
{
    if (((position == begin()) || compare((position - 1)->first, key)) && ((position == end()) || compare(key, position.first)))
        return _container.emplace(position, std::make_pair(key, TValue(std::forward<Args>(args)...)));
    return emplace_internal(key, std::forward<Args>(args)...).first;
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline void FlatMap<TKey, TValue, TCompare, TAllocator>::swap(FlatMap& flatmap) noexcept
{
    using std::swap;
    swap(_compare, flatmap._compare);
    swap(_container, flatmap._container);
}

template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
inline void swap(FlatMap<TKey, TValue, TCompare, TAllocator>& flatmap1, FlatMap<TKey, TValue, TCompare, TAllocator>& flatmap2) noexcept
{
    flatmap1.swap(flatmap2);
}

} // namespace CppCommon
