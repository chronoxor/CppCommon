/*!
    \file queue.inl
    \brief Intrusive queue container inline implementation
    \author Ivan Shynkarenka
    \date 20.06.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T>
template <class InputIterator>
inline Queue<T>::Queue(InputIterator first, InputIterator last)
{
    for (InputIterator it = first; it != last; ++it)
        push(*it);
}

template <typename T>
inline QueueIterator<T> Queue<T>::begin() noexcept
{
    return QueueIterator<T>(front());
}

template <typename T>
inline QueueConstIterator<T> Queue<T>::begin() const noexcept
{
    return QueueConstIterator<T>(front());
}

template <typename T>
inline QueueIterator<T> Queue<T>::end() noexcept
{
    return QueueIterator<T>(nullptr);
}

template <typename T>
inline QueueConstIterator<T> Queue<T>::end() const noexcept
{
    return QueueConstIterator<T>(nullptr);
}

template <typename T>
inline Queue<T>& Queue<T>::push(T& item) noexcept
{
    if (_back != nullptr)
        _back->next = &item;
    item.next = nullptr;
    _back = &item;
    if (_front == nullptr)
        _front = _back;
    ++_size;
    return *this;
}

template <typename T>
inline T* Queue<T>::pop() noexcept
{
    if (_front == nullptr)
        return nullptr;

    T* result = _front;
    _front = result->next;
    result->next = nullptr;
    if (_front == nullptr)
        _back = nullptr;
    --_size;
    return result;
}

template <typename T>
inline void Queue<T>::reverse() noexcept
{
    T* current = _front;
    T* prev = nullptr;
    T* next = nullptr;

    _back = current;
    while (current != nullptr)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    _front = prev;
}

template <typename T>
inline void Queue<T>::swap(Queue& queue) noexcept
{
    using std::swap;
    swap(_size, queue._size);
    swap(_front, queue._front);
    swap(_back, queue._back);
}

template <typename T>
inline void swap(Queue<T>& queue1, Queue<T>& queue2) noexcept
{
    queue1.swap(queue2);
}

template <typename T>
QueueIterator<T>& QueueIterator<T>::operator++() noexcept
{
    if (_current != nullptr)
        _current = _current->next;
    return *this;
}

template <typename T>
inline QueueIterator<T> QueueIterator<T>::operator++(int) noexcept
{
    QueueIterator<T> result(*this);
    operator++();
    return result;
}

template <typename T>
T& QueueIterator<T>::operator*() noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename T>
T* QueueIterator<T>::operator->() noexcept
{
    return _current;
}

template <typename T>
void QueueIterator<T>::swap(QueueIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename T>
void swap(QueueIterator<T>& it1, QueueIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

template <typename T>
QueueConstIterator<T>& QueueConstIterator<T>::operator++() noexcept
{
    if (_current != nullptr)
        _current = _current->next;
    return *this;
}

template <typename T>
inline QueueConstIterator<T> QueueConstIterator<T>::operator++(int) noexcept
{
    QueueConstIterator<T> result(*this);
    operator++();
    return result;
}

template <typename T>
const T& QueueConstIterator<T>::operator*() const noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename T>
const T* QueueConstIterator<T>::operator->() const noexcept
{
    return _current;
}

template <typename T>
void QueueConstIterator<T>::swap(QueueConstIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename T>
void swap(QueueConstIterator<T>& it1, QueueConstIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

} // namespace CppCommon
