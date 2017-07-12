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
    : _hash(hash), _equal(equal), _size(0), _blank(blank), _buckets(allocator)
{
    size_t reserve = 1;
    while (reserve < capacity)
        reserve <<= 1;
    _buckets.reserve(reserve, std::make_pair(_blank, TValue()));
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
template <class InputIterator>
inline HashMap<TKey, TValue, THash, TEqual, TAllocator>::HashMap(InputIterator first, InputIterator last, size_t capacity, const TKey& blank, const THash& hash, const TEqual& equal, const TAllocator& allocator)
    : _hash(hash), _equal(equal), _size(0), _blank(blank), _buckets(allocator)
{
    for (InputIterator it = first; it != last; ++it)
        insert(*it);
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
inline const_iterator HashMap<TKey, TValue, THash, TEqual, TAllocator>::cend() const noexcept
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
inline void HashMap<TKey, TValue, THash, TEqual, TAllocator>::swap(HashMap& HashMap) noexcept
{
    using std::swap;
    swap(_hash, HashMap._hash);
    swap(_equal, HashMap._equal);
    swap(_size, HashMap._size);
    swap(_blank, HashMap._blank);
    swap(_buckets, HashMap._buckets);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline void swap(HashMap<TKey, TValue, THash, TEqual, TAllocator>& HashMap1, HashMap<TKey, TValue, THash, TEqual, TAllocator>& HashMap2) noexcept
{
    HashMap1.swap(HashMap2);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
HashMapIterator<T>& HashMapIterator<T>::operator++() noexcept
{
    if (_current != nullptr)
        _current = _current->next;
    return *this;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline HashMapIterator<T> HashMapIterator<T>::operator++(int) noexcept
{
    HashMapIterator<T> result(*this);
    operator++();
    return result;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
T& HashMapIterator<T>::operator*() noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
T* HashMapIterator<T>::operator->() noexcept
{
    return _current;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
void HashMapIterator<T>::swap(HashMapIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
void swap(HashMapIterator<T>& it1, HashMapIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
HashMapConstIterator<T>& HashMapConstIterator<T>::operator++() noexcept
{
    if (_current != nullptr)
        _current = _current->next;
    return *this;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
inline HashMapConstIterator<T> HashMapConstIterator<T>::operator++(int) noexcept
{
    HashMapConstIterator<T> result(*this);
    operator++();
    return result;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
const T& HashMapConstIterator<T>::operator*() const noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
const T* HashMapConstIterator<T>::operator->() const noexcept
{
    return _current;
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
void HashMapConstIterator<T>::swap(HashMapConstIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
void swap(HashMapConstIterator<T>& it1, HashMapConstIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

} // namespace CppCommon
