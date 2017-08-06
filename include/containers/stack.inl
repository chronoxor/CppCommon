/*!
    \file stack.inl
    \brief Intrusive stack container inline implementation
    \author Ivan Shynkarenka
    \date 20.06.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T>
template <class InputIterator>
inline Stack<T>::Stack(InputIterator first, InputIterator last) noexcept
{
    for (auto it = first; it != last; ++it)
        push(*it);
}

template <typename T>
inline typename Stack<T>::iterator Stack<T>::begin() noexcept
{
    return iterator(top());
}

template <typename T>
inline typename Stack<T>::const_iterator Stack<T>::begin() const noexcept
{
    return const_iterator(top());
}

template <typename T>
inline typename Stack<T>::const_iterator Stack<T>::cbegin() const noexcept
{
    return const_iterator(top());
}

template <typename T>
inline typename Stack<T>::iterator Stack<T>::end() noexcept
{
    return iterator(nullptr);
}

template <typename T>
inline typename Stack<T>::const_iterator Stack<T>::end() const noexcept
{
    return const_iterator(nullptr);
}

template <typename T>
inline typename Stack<T>::const_iterator Stack<T>::cend() const noexcept
{
    return const_iterator(nullptr);
}

template <typename T>
inline void Stack<T>::push(T& item) noexcept
{
    item.next = _top;
    _top = &item;
    ++_size;
}

template <typename T>
inline T* Stack<T>::pop() noexcept
{
    if (_top == nullptr)
        return nullptr;

    T* result = _top;
    _top = result->next;
    result->next = nullptr;
    --_size;
    return result;
}

template <typename T>
inline void Stack<T>::reverse() noexcept
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

template <typename T>
inline void Stack<T>::clear() noexcept
{
    _size = 0;
    _top = nullptr;
}

template <typename T>
inline void Stack<T>::swap(Stack& stack) noexcept
{
    using std::swap;
    swap(_size, stack._size);
    swap(_top, stack._top);
}

template <typename T>
inline void swap(Stack<T>& stack1, Stack<T>& stack2) noexcept
{
    stack1.swap(stack2);
}

template <typename T>
StackIterator<T>& StackIterator<T>::operator++() noexcept
{
    if (_node != nullptr)
        _node = _node->next;
    return *this;
}

template <typename T>
inline StackIterator<T> StackIterator<T>::operator++(int) noexcept
{
    StackIterator<T> result(*this);
    operator++();
    return result;
}

template <typename T>
typename StackIterator<T>::reference StackIterator<T>::operator*() noexcept
{
    assert((_node != nullptr) && "Iterator must be valid!");

    return *_node;
}

template <typename T>
typename StackIterator<T>::pointer StackIterator<T>::operator->() noexcept
{
    return _node;
}

template <typename T>
void StackIterator<T>::swap(StackIterator& it) noexcept
{
    using std::swap;
    swap(_node, it._node);
}

template <typename T>
void swap(StackIterator<T>& it1, StackIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

template <typename T>
StackConstIterator<T>& StackConstIterator<T>::operator++() noexcept
{
    if (_node != nullptr)
        _node = _node->next;
    return *this;
}

template <typename T>
inline StackConstIterator<T> StackConstIterator<T>::operator++(int) noexcept
{
    StackConstIterator<T> result(*this);
    operator++();
    return result;
}

template <typename T>
typename StackConstIterator<T>::const_reference StackConstIterator<T>::operator*() const noexcept
{
    assert((_node != nullptr) && "Iterator must be valid!");

    return *_node;
}

template <typename T>
typename StackConstIterator<T>::const_pointer StackConstIterator<T>::operator->() const noexcept
{
    return _node;
}

template <typename T>
void StackConstIterator<T>::swap(StackConstIterator& it) noexcept
{
    using std::swap;
    swap(_node, it._node);
}

template <typename T>
void swap(StackConstIterator<T>& it1, StackConstIterator<T>& it2) noexcept
{
    it1.swap(it2);
}

} // namespace CppCommon
