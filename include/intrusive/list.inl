/*!
    \file list.inl
    \brief Intrusive list container inline implementation
    \author Ivan Shynkarenka
    \date 20.06.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T>
template <class InputIterator>
inline List<T>::List(InputIterator first, InputIterator last)
{
    for (InputIterator it = first; it != last; ++it)
        PushBack(*it);
}

template <typename T>
inline ListIterator<T> List<T>::begin() noexcept
{
    return ListIterator<T>(front());
}

template <typename T>
inline ListConstIterator<T> List<T>::begin() const noexcept
{
    return ListConstIterator<T>(front());
}

template <typename T>
inline ListIterator<T> List<T>::end() noexcept
{
    return ListIterator<T>(nullptr);
}

template <typename T>
inline ListConstIterator<T> List<T>::end() const noexcept
{
    return ListConstIterator<T>(nullptr);
}

template <typename T>
inline ListReverseIterator<T> List<T>::rbegin() noexcept
{
    return ListReverseIterator<T>(back());
}

template <typename T>
inline ListReverseConstIterator<T> List<T>::rbegin() const noexcept
{
    return ListReverseConstIterator<T>(back());
}

template <typename T>
inline ListReverseIterator<T> List<T>::rend() noexcept
{
    return ListReverseIterator<T>(nullptr);
}

template <typename T>
inline ListReverseConstIterator<T> List<T>::rend() const noexcept
{
    return ListReverseConstIterator<T>(nullptr);
}

template <typename T>
inline List<T>& List<T>::push_front(T& item) noexcept
{
    if (_front != nullptr)
        _front->prev = &item;
    item.next = _front;
    item.prev = nullptr;
    _front = &item;
    if (_back == nullptr)
        _back = _front;
    ++_size;
    return *this;
}

template <typename T>
inline List<T>& List<T>::push_back(T& item) noexcept
{
    if (_back != nullptr)
        _back->next = &item;
    item.next = nullptr;
    item.prev = _back;
    _back = &item;
    if (_front == nullptr)
        _front = _back;
    ++_size;
    return *this;
}

template <typename T>
inline List<T>& List<T>::push_next(T& base, T& item) noexcept
{
    item.next = base.next;
    item.prev = &base;
    if (_back == &base)
        _back = &item;
    if (base.next != nullptr)
        base.next->prev = &item;
    base.next = &item;
    ++_size;
    return *this;
}

template <typename T>
inline List<T>& List<T>::push_prev(T& base, T& item) noexcept
{
    item.next = &base;
    item.prev = base.prev;
    if (_front == &base)
        _front = &item;
    if (base.prev != nullptr)
        base.prev->next = &item;
    base.prev = &item;
    ++_size;
    return *this;
}

template <typename T>
inline T* List<T>::pop_front() noexcept
{
    if (_front == nullptr)
        return nullptr;

    T* result = _front;
    _front = result->next;
    result->next = nullptr;
    result->prev = nullptr;
    if (_front == nullptr)
        _back = nullptr;
    else
        _front->prev = nullptr;
    --_size;
    return result;
}

template <typename T>
inline T* List<T>::pop_back() noexcept
{
    if (_back == nullptr)
        return nullptr;

    T* result = _back;
    _back = result->prev;
    result->next = nullptr;
    result->prev = nullptr;
    if (_back == nullptr)
        _front = nullptr;
    else
        _back->next = nullptr;
    --_size;
    return result;
}

template <typename T>
inline T* List<T>::pop_current(T& base) noexcept
{
    T* result = &base;
    if (result->next != nullptr)
        result->next->prev = result->prev;
    else
        _back = result->prev;
    if (result->prev != nullptr)
        result->prev->next = result->next;
    else
        _front = result->next;
    result->next = nullptr;
    result->prev = nullptr;
    --_size;
    return result;
}

template <typename T>
inline T* List<T>::pop_next(T& base) noexcept
{
    if (base.next == nullptr)
        return nullptr;

    T* result = base.next;
    if (result->next != nullptr)
        result->next->prev = &base;
    else
        _back = &base;
    base.next = result->next;
    result->next = nullptr;
    result->prev = nullptr;
    --_size;
    return result;
}

template <typename T>
inline T* List<T>::pop_prev(T& base) noexcept
{
    if (base.prev == nullptr)
        return nullptr;

    T* result = base.prev;
    if (result->prev != nullptr)
        result->prev->next = &base;
    else
        _front = &base;
    base.prev = result->prev;
    result->next = nullptr;
    result->prev = nullptr;
    --_size;
    return result;
}

template <typename T>
inline void List<T>::reverse() noexcept
{
    T* current = _front;
    T* prev = nullptr;
    T* next = nullptr;

    _back = current;
    while (current != nullptr)
    {
        next = current->next;
        current->next = prev;
        current->prev = next;
        prev = current;
        current = next;
    }
    _front = prev;
}

template <typename T>
inline void List<T>::swap(List& list) noexcept
{
    using std::swap;
    swap(_size, list._size);
    swap(_front, list._front);
    swap(_back, list._back);
}

template <typename T>
inline void swap(List<T>& list1, List<T>& list2) noexcept
{
    list1.swap(list2);
}

template <typename T>
ListIterator<T>& ListIterator<T>::operator++() noexcept
{
    if (_current != nullptr)
        _current = _current->next;
    return *this;
}

template <typename T>
inline ListIterator<T> ListIterator<T>::operator++(int) noexcept
{
    ListIterator<T> result(*this);
    operator++();
    return result;
}

template <typename T>
T& ListIterator<T>::operator*() noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename T>
T* ListIterator<T>::operator->() noexcept
{
    return _current;
}

template <typename T>
void ListIterator<T>::swap(ListIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename T>
void swap(ListIterator<T>& it1, ListIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

template <typename T>
ListConstIterator<T>& ListConstIterator<T>::operator++() noexcept
{
    if (_current != nullptr)
        _current = _current->next;
    return *this;
}

template <typename T>
inline ListConstIterator<T> ListConstIterator<T>::operator++(int) noexcept
{
    ListConstIterator<T> result(*this);
    operator++();
    return result;
}

template <typename T>
const T& ListConstIterator<T>::operator*() const noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename T>
const T* ListConstIterator<T>::operator->() const noexcept
{
    return _current;
}

template <typename T>
void ListConstIterator<T>::swap(ListConstIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename T>
void swap(ListConstIterator<T>& it1, ListConstIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

template <typename T>
ListReverseIterator<T>& ListReverseIterator<T>::operator++() noexcept
{
    if (_current != nullptr)
        _current = _current->prev;
    return *this;
}

template <typename T>
inline ListReverseIterator<T> ListReverseIterator<T>::operator++(int) noexcept
{
    ListReverseIterator<T> result(*this);
    operator++();
    return result;
}

template <typename T>
T& ListReverseIterator<T>::operator*() noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename T>
T* ListReverseIterator<T>::operator->() noexcept
{
    return _current;
}

template <typename T>
void ListReverseIterator<T>::swap(ListReverseIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename T>
void swap(ListReverseIterator<T>& it1, ListReverseIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

template <typename T>
ListReverseConstIterator<T>& ListReverseConstIterator<T>::operator++() noexcept
{
    if (_current != nullptr)
        _current = _current->prev;
    return *this;
}

template <typename T>
inline ListReverseConstIterator<T> ListReverseConstIterator<T>::operator++(int) noexcept
{
    ListReverseConstIterator<T> result(*this);
    operator++();
    return result;
}

template <typename T>
const T& ListReverseConstIterator<T>::operator*() const noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename T>
const T* ListReverseConstIterator<T>::operator->() const noexcept
{
    return _current;
}

template <typename T>
void ListReverseConstIterator<T>::swap(ListReverseConstIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename T>
void swap(ListReverseConstIterator<T>& it1, ListReverseConstIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

} // namespace CppCommon
