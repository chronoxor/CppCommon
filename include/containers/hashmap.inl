/*!
    \file hashmap.inl
    \brief Hash map container inline implementation
    \author Ivan Shynkarenka
    \date 11.07.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline HashMap<TKey, TValue, THash, TEqual>::HashMap(size_t capacity, const TKey& blank) noexcept
    : _size(0), _blank(blank)
{
    size_t reserve = 1;
    while (reserve < capacity)
        reserve <<= 1;
    _buckets.reserve(reserve, std::make_pair(_blank, TValue()));
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
template <class InputIterator>
inline HashMap<TKey, TValue, THash, TEqual>::HashMap(InputIterator first, InputIterator last) noexcept
{
    for (InputIterator it = first; it != last; ++it)
        push(*it);
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline HashMapIterator<T> HashMap<TKey, TValue, THash, TEqual>::begin() noexcept
{
    return HashMapIterator<T>(_buckets.data());
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline HashMapConstIterator<T> HashMap<TKey, TValue, THash, TEqual>::begin() const noexcept
{
    return HashMapConstIterator<T>(_buckets.data());
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline HashMapIterator<T> HashMap<TKey, TValue, THash, TEqual>::end() noexcept
{
    return HashMapIterator<T>(nullptr);
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline HashMapConstIterator<T> HashMap<TKey, TValue, THash, TEqual>::end() const noexcept
{
    return HashMapConstIterator<T>(nullptr);
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline HashMap<T>& HashMap<TKey, TValue, THash, TEqual>::push(T& item) noexcept
{
    item.next = _top;
    _top = &item;
    ++_size;
    return *this;
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline T* HashMap<TKey, TValue, THash, TEqual>::pop() noexcept
{
    if (_top == nullptr)
        return nullptr;

    T* result = _top;
    _top = result->next;
    result->next = nullptr;
    --_size;
    return result;
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline void HashMap<TKey, TValue, THash, TEqual>::reverse() noexcept
{
    T* current = _top;
    T* prev = nullptr;
    T* next = nullptr;

    while (current != nullptr)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    _top = prev;
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline void HashMap<TKey, TValue, THash, TEqual>::swap(HashMap& HashMap) noexcept
{
    using std::swap;
    swap(_size, HashMap._size);
    swap(_top, HashMap._top);
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline void swap(HashMap<T>& HashMap1, HashMap<T>& HashMap2) noexcept
{
    HashMap1.swap(HashMap2);
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
HashMapIterator<T>& HashMapIterator<T>::operator++() noexcept
{
    if (_current != nullptr)
        _current = _current->next;
    return *this;
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline HashMapIterator<T> HashMapIterator<T>::operator++(int) noexcept
{
    HashMapIterator<T> result(*this);
    operator++();
    return result;
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
T& HashMapIterator<T>::operator*() noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
T* HashMapIterator<T>::operator->() noexcept
{
    return _current;
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
void HashMapIterator<T>::swap(HashMapIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
void swap(HashMapIterator<T>& it1, HashMapIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
HashMapConstIterator<T>& HashMapConstIterator<T>::operator++() noexcept
{
    if (_current != nullptr)
        _current = _current->next;
    return *this;
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
inline HashMapConstIterator<T> HashMapConstIterator<T>::operator++(int) noexcept
{
    HashMapConstIterator<T> result(*this);
    operator++();
    return result;
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
const T& HashMapConstIterator<T>::operator*() const noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
const T* HashMapConstIterator<T>::operator->() const noexcept
{
    return _current;
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
void HashMapConstIterator<T>::swap(HashMapConstIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename TKey, typename TValue, typename THash, typename TEqual>
void swap(HashMapConstIterator<T>& it1, HashMapConstIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

} // namespace CppCommon
