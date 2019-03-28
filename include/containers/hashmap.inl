/*!
    \file hashmap.inl
    \brief Hash map container inline implementation
    \author Ivan Shynkarenka
    \date 11.07.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline HashMap<TKey, TValue, THash, TEqual, TAllocator>::HashMap(size_t capacity, const TKey& blank, const THash& hash, const TEqual& equal, const TAllocator& allocator)
    : _hash(hash), _equal(equal), _blank(blank), _size(0), _buckets(allocator)
{
    size_t reserve = 1;
    while (reserve < capacity)
        reserve <<= 1;
    _buckets.resize(reserve, std::make_pair(_blank, TValue()));
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
template <class InputIterator>
inline HashMap<TKey, TValue, THash, TEqual, TAllocator>::HashMap(InputIterator first, InputIterator last, bool unused, size_t capacity, const TKey& blank, const THash& hash, const TEqual& equal, const TAllocator& allocator)
    : HashMap(capacity, blank, hash, equal, allocator)
{
    for (auto it = first; it != last; ++it)
        insert(*it);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline HashMap<TKey, TValue, THash, TEqual, TAllocator>::HashMap(const HashMap& hashmap)
    : HashMap(hashmap.bucket_count(), hashmap._blank, hashmap._hash, hashmap._equal, hashmap._buckets.get_allocator())
{
    for (const auto& item : hashmap)
        insert(item);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline HashMap<TKey, TValue, THash, TEqual, TAllocator>::HashMap(const HashMap& hashmap, size_t capacity)
    : HashMap(capacity, hashmap._blank, hashmap._hash, hashmap._equal, hashmap._buckets.get_allocator())
{
    for (const auto& item : hashmap)
        insert(item);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline HashMap<TKey, TValue, THash, TEqual, TAllocator>& HashMap<TKey, TValue, THash, TEqual, TAllocator>::operator=(const HashMap& hashmap)
{
    clear();
    reserve(hashmap.size());
    for (const auto& item : hashmap)
        insert(item);
    return *this;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::begin() noexcept
{
    return iterator(this);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::const_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::begin() const noexcept
{
    return const_iterator(this);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::const_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::cbegin() const noexcept
{
    return const_iterator(this);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::end() noexcept
{
    return iterator(nullptr);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::const_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::end() const noexcept
{
    return const_iterator(nullptr);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::const_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::cend() const noexcept
{
    return const_iterator(nullptr);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::reverse_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::rbegin() noexcept
{
    return reverse_iterator(this);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::const_reverse_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::rbegin() const noexcept
{
    return const_reverse_iterator(this);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::const_reverse_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::crbegin() const noexcept
{
    return const_reverse_iterator(this);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::reverse_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::rend() noexcept
{
    return reverse_iterator(nullptr);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::const_reverse_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::rend() const noexcept
{
    return const_reverse_iterator(nullptr);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::const_reverse_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::crend() const noexcept
{
    return const_reverse_iterator(nullptr);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::find(const TKey& key) noexcept
{
    assert(!key_equal(key, _blank) && "Cannot find a blank key!");

    for (size_t index = key_to_index(key);; index = next_index(index))
    {
        if (key_equal(_buckets[index].first, key))
            return iterator(this, index);
        if (key_equal(_buckets[index].first, _blank))
            return end();
    }
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::const_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::find(const TKey& key) const noexcept
{
    assert(!key_equal(key, _blank) && "Cannot find a blank key!");

    for (size_t index = key_to_index(key);; index = next_index(index))
    {
        if (key_equal(_buckets[index].first, key))
            return const_iterator(this, index);
        if (key_equal(_buckets[index].first, _blank))
            return end();
    }
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline std::pair<typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::iterator, typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::iterator> HashMap<TKey, TValue, THash, TEqual, TAllocator>::equal_range(const TKey& key) noexcept
{
    return std::make_pair(find(key), end());
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline std::pair<typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::const_iterator, typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::const_iterator> HashMap<TKey, TValue, THash, TEqual, TAllocator>::equal_range(const TKey& key) const noexcept
{
    return std::make_pair(find(key), end());
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::mapped_type& HashMap<TKey, TValue, THash, TEqual, TAllocator>::at(const TKey& key) noexcept
{
    auto it = find(key);
    if (it == end())
        throw std::out_of_range("Item with the given key was not found in the hash map!");

    return it->second;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline const typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::mapped_type& HashMap<TKey, TValue, THash, TEqual, TAllocator>::at(const TKey& key) const noexcept
{
    auto it = find(key);
    if (it == end())
        throw std::out_of_range("Item with the given key was not found in the hash map!");

    return it->second;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline std::pair<typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::iterator, bool> HashMap<TKey, TValue, THash, TEqual, TAllocator>::insert(const value_type& item)
{
    return emplace_internal(item.first, item.second);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline std::pair<typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::iterator, bool> HashMap<TKey, TValue, THash, TEqual, TAllocator>::insert(value_type&& item)
{
    return emplace_internal(item.first, std::move(item.second));
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
template <typename... Args>
inline std::pair<typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::iterator, bool> HashMap<TKey, TValue, THash, TEqual, TAllocator>::emplace(Args&&... args)
{
    return emplace_internal(std::forward<Args>(args)...);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline size_t HashMap<TKey, TValue, THash, TEqual, TAllocator>::erase(const TKey& key)
{
    auto it = find(key);
    if (it == end())
        return 0;

    erase_internal(it._index);
    return 1;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline void HashMap<TKey, TValue, THash, TEqual, TAllocator>::erase(const const_iterator& position)
{
    erase_internal(position._index);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
template <typename... Args>
inline std::pair<typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::iterator, bool> HashMap<TKey, TValue, THash, TEqual, TAllocator>::emplace_internal(const TKey& key, Args&&... args)
{
    assert(!key_equal(key, _blank) && "Cannot emplace a blank key!");

    reserve(_size + 1);

    for (size_t index = key_to_index(key);; index = next_index(index))
    {
        if (key_equal(_buckets[index].first, key))
            return std::make_pair(iterator(this, index), false);
        if (key_equal(_buckets[index].first, _blank))
        {
            _buckets[index].first = key;
            _buckets[index].second = TValue(std::forward<Args>(args)...);
            ++_size;
            return std::make_pair(iterator(this, index), true);
        }
    }
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline void HashMap<TKey, TValue, THash, TEqual, TAllocator>::erase_internal(size_t index)
{
    size_t current = index;
    for (index = next_index(current);; index = next_index(index))
    {
        if (key_equal(_buckets[index].first, _blank))
        {
            _buckets[current].first = _blank;
            --_size;
            return;
        }

        // Move buckets with the same key hash closer to the first suitable position in the hash map
        size_t base = key_to_index(_buckets[index].first);
        if (diff(current, base) < diff(index, base))
        {
            _buckets[current] = _buckets[index];
            current = index;
        }
    }
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline size_t HashMap<TKey, TValue, THash, TEqual, TAllocator>::key_to_index(const TKey& key) const noexcept
{
    size_t mask = _buckets.size() - 1;
    return _hash(key) & mask;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline size_t HashMap<TKey, TValue, THash, TEqual, TAllocator>::next_index(size_t index) const noexcept
{
    size_t mask = _buckets.size() - 1;
    return (index + 1) & mask;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline size_t HashMap<TKey, TValue, THash, TEqual, TAllocator>::diff(size_t index1, size_t index2) const noexcept
{
    size_t mask = _buckets.size() - 1;
    return (_buckets.size() + (index1 - index2)) & mask;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline void HashMap<TKey, TValue, THash, TEqual, TAllocator>::rehash(size_t capacity)
{
    capacity = std::max(capacity, 2 * size());
    HashMap<TKey, TValue, THash, TEqual, TAllocator> temp(*this, capacity);
    swap(temp);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline void HashMap<TKey, TValue, THash, TEqual, TAllocator>::reserve(size_t count)
{
    if (_buckets.size() < 2 * count)
        rehash(2 * count);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline void HashMap<TKey, TValue, THash, TEqual, TAllocator>::clear() noexcept
{
    _size = 0;
    for (auto& bucket : _buckets)
        bucket.first = _blank;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline void HashMap<TKey, TValue, THash, TEqual, TAllocator>::swap(HashMap& hashmap) noexcept
{
    using std::swap;
    swap(_hash, hashmap._hash);
    swap(_equal, hashmap._equal);
    swap(_blank, hashmap._blank);
    swap(_size, hashmap._size);
    swap(_buckets, hashmap._buckets);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline void swap(HashMap<TKey, TValue, THash, TEqual, TAllocator>& hashmap1, HashMap<TKey, TValue, THash, TEqual, TAllocator>& hashmap2) noexcept
{
    hashmap1.swap(hashmap2);
}

template <class TContainer, typename TKey, typename TValue>
inline HashMapIterator<TContainer, TKey, TValue>::HashMapIterator(TContainer* container) noexcept : _container(container), _index(0)
{
    if (_container != nullptr)
    {
        if (_container->size() == 0)
        {
            // Hash map is empty
            _container = nullptr;
            _index = 0;
            return;
        }
        else
        {
            for (size_t i = 0; i < _container->_buckets.size(); ++i)
            {
                if (!_container->key_equal(_container->_buckets[i].first, _container->_blank))
                {
                    _index = i;
                    return;
                }
            }

            assert(false && "Non empty hash map has no valid items!");
        }
    }
}

template <class TContainer, typename TKey, typename TValue>
HashMapIterator<TContainer, TKey, TValue>& HashMapIterator<TContainer, TKey, TValue>::operator++() noexcept
{
    if (_container != nullptr)
    {
        for (size_t i = _index + 1; i < _container->_buckets.size(); ++i)
        {
            if (!_container->key_equal(_container->_buckets[i].first, _container->_blank))
            {
                _index = i;
                return *this;
            }
        }

        // End of the hash map
        _container = nullptr;
        _index = 0;
    }

    return *this;
}

template <class TContainer, typename TKey, typename TValue>
inline HashMapIterator<TContainer, TKey, TValue> HashMapIterator<TContainer, TKey, TValue>::operator++(int) noexcept
{
    HashMapIterator<TContainer, TKey, TValue> result(*this);
    operator++();
    return result;
}

template <class TContainer, typename TKey, typename TValue>
typename HashMapIterator<TContainer, TKey, TValue>::reference HashMapIterator<TContainer, TKey, TValue>::operator*() noexcept
{
    assert(((_container != nullptr) && (_index < _container->_buckets.size())) && "Iterator must be valid!");

    return _container->_buckets[_index];
}

template <class TContainer, typename TKey, typename TValue>
typename HashMapIterator<TContainer, TKey, TValue>::pointer HashMapIterator<TContainer, TKey, TValue>::operator->() noexcept
{
    return ((_container != nullptr) && (_index < _container->_buckets.size())) ? &_container->_buckets[_index] : nullptr;
}

template <class TContainer, typename TKey, typename TValue>
void HashMapIterator<TContainer, TKey, TValue>::swap(HashMapIterator& it) noexcept
{
    using std::swap;
    swap(_container, it._container);
    swap(_index, it._index);
}

template <class TContainer, typename TKey, typename TValue>
void swap(HashMapIterator<TContainer, TKey, TValue>& it1, HashMapIterator<TContainer, TKey, TValue>& it2) noexcept
{
    it1.swap(it2);
}

template <class TContainer, typename TKey, typename TValue>
inline HashMapConstIterator<TContainer, TKey, TValue>::HashMapConstIterator(const TContainer* container) noexcept : _container(container), _index(0)
{
    if (_container != nullptr)
    {
        if (_container->size() == 0)
        {
            // Hash map is empty
            _container = nullptr;
            _index = 0;
            return;
        }
        else
        {
            for (size_t i = 0; i < _container->_buckets.size(); ++i)
            {
                if (!_container->key_equal(_container->_buckets[i].first, _container->_blank))
                {
                    _index = i;
                    return;
                }
            }

            assert(false && "Non empty hash map has no valid items!");
        }
    }
}

template <class TContainer, typename TKey, typename TValue>
HashMapConstIterator<TContainer, TKey, TValue>& HashMapConstIterator<TContainer, TKey, TValue>::operator++() noexcept
{
    if (_container != nullptr)
    {
        for (size_t i = _index + 1; i < _container->_buckets.size(); ++i)
        {
            if (!_container->key_equal(_container->_buckets[i].first, _container->_blank))
            {
                _index = i;
                return *this;
            }
        }

        // End of the hash map
        _container = nullptr;
        _index = 0;
    }

    return *this;
}

template <class TContainer, typename TKey, typename TValue>
inline HashMapConstIterator<TContainer, TKey, TValue> HashMapConstIterator<TContainer, TKey, TValue>::operator++(int) noexcept
{
    HashMapConstIterator<TContainer, TKey, TValue> result(*this);
    operator++();
    return result;
}

template <class TContainer, typename TKey, typename TValue>
typename HashMapConstIterator<TContainer, TKey, TValue>::const_reference HashMapConstIterator<TContainer, TKey, TValue>::operator*() const noexcept
{
    assert(((_container != nullptr) && (_index < _container->_buckets.size())) && "Iterator must be valid!");

    return _container->_buckets[_index];
}

template <class TContainer, typename TKey, typename TValue>
typename HashMapConstIterator<TContainer, TKey, TValue>::const_pointer HashMapConstIterator<TContainer, TKey, TValue>::operator->() const noexcept
{
    return ((_container != nullptr) && (_index < _container->_buckets.size())) ? &_container->_buckets[_index] : nullptr;
}

template <class TContainer, typename TKey, typename TValue>
void HashMapConstIterator<TContainer, TKey, TValue>::swap(HashMapConstIterator& it) noexcept
{
    using std::swap;
    swap(_container, it._container);
    swap(_index, it._index);
}

template <class TContainer, typename TKey, typename TValue>
void swap(HashMapConstIterator<TContainer, TKey, TValue>& it1, HashMapConstIterator<TContainer, TKey, TValue>& it2) noexcept
{
    it1.swap(it2);
}

template <class TContainer, typename TKey, typename TValue>
inline HashMapReverseIterator<TContainer, TKey, TValue>::HashMapReverseIterator(TContainer* container) noexcept : _container(container), _index(0)
{
    if (_container != nullptr)
    {
        if (_container->size() == 0)
        {
            // Hash map is empty
            _container = nullptr;
            _index = 0;
            return;
        }
        else
        {
            for (size_t i = _container->_buckets.size(); i-- > 0;)
            {
                if (!_container->key_equal(_container->_buckets[i].first, _container->_blank))
                {
                    _index = i;
                    return;
                }
            }

            assert(false && "Non empty hash map has no valid items!");
        }
    }
}

template <class TContainer, typename TKey, typename TValue>
HashMapReverseIterator<TContainer, TKey, TValue>& HashMapReverseIterator<TContainer, TKey, TValue>::operator++() noexcept
{
    if (_container != nullptr)
    {
        for (size_t i = _index; i-- > 0;)
        {
            if (!_container->key_equal(_container->_buckets[i].first, _container->_blank))
            {
                _index = i;
                return *this;
            }
        }

        // End of the hash map
        _container = nullptr;
        _index = 0;
    }

    return *this;
}

template <class TContainer, typename TKey, typename TValue>
inline HashMapReverseIterator<TContainer, TKey, TValue> HashMapReverseIterator<TContainer, TKey, TValue>::operator++(int) noexcept
{
    HashMapReverseIterator<TContainer, TKey, TValue> result(*this);
    operator++();
    return result;
}

template <class TContainer, typename TKey, typename TValue>
typename HashMapReverseIterator<TContainer, TKey, TValue>::reference HashMapReverseIterator<TContainer, TKey, TValue>::operator*() noexcept
{
    assert(((_container != nullptr) && (_index < _container->_buckets.size())) && "Iterator must be valid!");

    return _container->_buckets[_index];
}

template <class TContainer, typename TKey, typename TValue>
typename HashMapReverseIterator<TContainer, TKey, TValue>::pointer HashMapReverseIterator<TContainer, TKey, TValue>::operator->() noexcept
{
    return ((_container != nullptr) && (_index < _container->_buckets.size())) ? &_container->_buckets[_index] : nullptr;
}

template <class TContainer, typename TKey, typename TValue>
void HashMapReverseIterator<TContainer, TKey, TValue>::swap(HashMapReverseIterator& it) noexcept
{
    using std::swap;
    swap(_container, it._container);
    swap(_index, it._index);
}

template <class TContainer, typename TKey, typename TValue>
void swap(HashMapReverseIterator<TContainer, TKey, TValue>& it1, HashMapReverseIterator<TContainer, TKey, TValue>& it2) noexcept
{
    it1.swap(it2);
}

template <class TContainer, typename TKey, typename TValue>
inline HashMapConstReverseIterator<TContainer, TKey, TValue>::HashMapConstReverseIterator(const TContainer* container) noexcept : _container(container), _index(0)
{
    if (_container != nullptr)
    {
        if (_container->size() == 0)
        {
            // Hash map is empty
            _container = nullptr;
            _index = 0;
            return;
        }
        else
        {
            for (size_t i = _container->_buckets.size(); i-- > 0;)
            {
                if (!_container->key_equal(_container->_buckets[i].first, _container->_blank))
                {
                    _index = i;
                    return;
                }
            }

            assert(false && "Non empty hash map has no valid items!");
        }
    }
}

template <class TContainer, typename TKey, typename TValue>
HashMapConstReverseIterator<TContainer, TKey, TValue>& HashMapConstReverseIterator<TContainer, TKey, TValue>::operator++() noexcept
{
    if (_container != nullptr)
    {
        for (size_t i = _index; i-- > 0;)
        {
            if (!_container->key_equal(_container->_buckets[i].first, _container->_blank))
            {
                _index = i;
                return *this;
            }
        }

        // End of the hash map
        _container = nullptr;
        _index = 0;
    }

    return *this;
}

template <class TContainer, typename TKey, typename TValue>
inline HashMapConstReverseIterator<TContainer, TKey, TValue> HashMapConstReverseIterator<TContainer, TKey, TValue>::operator++(int) noexcept
{
    HashMapConstReverseIterator<TContainer, TKey, TValue> result(*this);
    operator++();
    return result;
}

template <class TContainer, typename TKey, typename TValue>
typename HashMapConstReverseIterator<TContainer, TKey, TValue>::const_reference HashMapConstReverseIterator<TContainer, TKey, TValue>::operator*() const noexcept
{
    assert(((_container != nullptr) && (_index < _container->_buckets.size())) && "Iterator must be valid!");

    return _container->_buckets[_index];
}

template <class TContainer, typename TKey, typename TValue>
typename HashMapConstReverseIterator<TContainer, TKey, TValue>::const_pointer HashMapConstReverseIterator<TContainer, TKey, TValue>::operator->() const noexcept
{
    return ((_container != nullptr) && (_index < _container->_buckets.size())) ? &_container->_buckets[_index] : nullptr;
}

template <class TContainer, typename TKey, typename TValue>
void HashMapConstReverseIterator<TContainer, TKey, TValue>::swap(HashMapConstReverseIterator& it) noexcept
{
    using std::swap;
    swap(_container, it._container);
    swap(_index, it._index);
}

template <class TContainer, typename TKey, typename TValue>
void swap(HashMapConstReverseIterator<TContainer, TKey, TValue>& it1, HashMapConstReverseIterator<TContainer, TKey, TValue>& it2) noexcept
{
    it1.swap(it2);
}

} // namespace CppCommon
