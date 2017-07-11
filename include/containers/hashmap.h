/*!
    \file hashmap.h
    \brief Hash map container definition
    \author Ivan Shynkarenka
    \date 11.07.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_CONTAINERS_HASHMAP_H
#define CPPCOMMON_CONTAINERS_HASHMAP_H

#include <cassert>
#include <iterator>
#include <vector>

namespace CppCommon {

template <typename TKey, typename TValue, typename THash = std::hash<Key>, typename TEqual = std::equal_to<void>>
class HashMapIterator;
template <typename TKey, typename TValue, typename THash = std::hash<Key>, typename TEqual = std::equal_to<void>>
class HashMapConstIterator;

//! Hash map container
/*!
    Hash map is an efficient  structure  for  associative  keys/value  storing
    and accessing. It uses  hash function  to  convert  string  key  into  the
    integer and use the index to quick access value data.

    Open  address  hash map resolves collisions of the  same  hash  values  by
    inserting new item into the next free place (probing with step 1).
*/
template <typename TKey, typename TValue, typename THash = std::hash<Key>, typename TEqual = std::equal_to<void>>
class HashMap
{
public:
    //! Initialize the hash map with a given capacity and empty
    /*!
        \param item - Pushed item
        \return The current stack collection
    */
    HashMap() noexcept : _size(0) {}
    template <class InputIterator>
    Stack(InputIterator first, InputIterator last);
    Stack(const Stack&) noexcept = delete;
    Stack(Stack&&) noexcept = default;
    ~Stack() noexcept = default;

    Stack& operator=(const Stack&) noexcept = delete;
    Stack& operator=(Stack&&) noexcept = default;

    //! Check if the stack is not empty
    explicit operator bool() const noexcept { return !empty(); }

    //! Is the stack empty?
    bool empty() const noexcept { return _top == nullptr; }

    //! Get the stack size
    size_t size() const noexcept { return _size; }

    //! Get the top stack item
    T* top() noexcept { return _top; }
    const T* top() const noexcept { return _top; }

    //! Get the begin stack iterator
    StackIterator<T> begin() noexcept;
    StackConstIterator<T> begin() const noexcept;
    //! Get the end stack iterator
    StackIterator<T> end() noexcept;
    StackConstIterator<T> end() const noexcept;

    //! Push a new item into the top of the stack
    /*!
        \param item - Pushed item
        \return The current stack collection
    */
    Stack& push(T& item) noexcept;

    //! Pop the item from the top of the stack
    /*!
        \return The top item popped from the stack
    */
    T* pop() noexcept;

    //! Reverse the stack
    void reverse() noexcept;

    //! Swap two instances
    void swap(Stack& stack) noexcept;
    template <typename U>
    friend void swap(Stack<U>& stack1, Stack<U>& stack2) noexcept;

private:
    size_t _size;   // Stack size
    T* _top;        // Stack top node
};

//! Intrusive stack iterator
/*!
    Not thread-safe.
*/
template <typename T>
class StackIterator
{
public:
    // Standard iterator type definitions
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef std::forward_iterator_tag iterator_category;

    StackIterator() noexcept : _current(nullptr) {}
    explicit StackIterator(T* current) noexcept : _current(current) {}
    StackIterator(const StackIterator& it) noexcept = default;
    StackIterator(StackIterator&& it) noexcept = default;
    ~StackIterator() noexcept = default;

    StackIterator& operator=(const StackIterator& it) noexcept = default;
    StackIterator& operator=(StackIterator&& it) noexcept = default;

    friend bool operator==(const StackIterator& it1, const StackIterator& it2) noexcept
    { return it1._current == it2._current; }
    friend bool operator!=(const StackIterator& it1, const StackIterator& it2) noexcept
    { return it1._current != it2._current; }

    StackIterator& operator++() noexcept;
    StackIterator operator++(int) noexcept;

    T& operator*() noexcept;
    T* operator->() noexcept;

    //! Check if the iterator is valid
    explicit operator bool() const noexcept { return _current != nullptr; }

    //! Swap two instances
    void swap(StackIterator& it) noexcept;
    template <typename U>
    friend void swap(StackIterator<U>& it1, StackIterator<U>& it2) noexcept;

private:
    T* _current;
};

//! Intrusive stack constant iterator
/*!
    Not thread-safe.
*/
template <typename T>
class StackConstIterator
{
public:
    // Standard constant iterator type definitions
    typedef const T value_type;
    typedef const T& reference;
    typedef const T* pointer;
    typedef std::forward_iterator_tag iterator_category;

    StackConstIterator() noexcept : _current(nullptr) {}
    explicit StackConstIterator(const T* current) noexcept : _current(current) {}
    StackConstIterator(const StackIterator<T>& it) noexcept : _current(it._current) {}
    StackConstIterator(const StackConstIterator& it) noexcept = default;
    StackConstIterator(StackConstIterator&& it) noexcept = default;
    ~StackConstIterator() noexcept = default;

    StackConstIterator& operator=(const StackIterator<T>& it) noexcept
    { _current = it._current; return *this; }
    StackConstIterator& operator=(const StackConstIterator& it) noexcept = default;
    StackConstIterator& operator=(StackConstIterator&& it) noexcept = default;

    friend bool operator==(const StackConstIterator& it1, const StackConstIterator& it2) noexcept
    { return it1._current == it2._current; }
    friend bool operator!=(const StackConstIterator& it1, const StackConstIterator& it2) noexcept
    { return it1._current != it2._current; }

    StackConstIterator& operator++() noexcept;
    StackConstIterator operator++(int) noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    //! Check if the iterator is valid
    explicit operator bool() const noexcept { return _current != nullptr; }

    //! Swap two instances
    void swap(StackConstIterator& it) noexcept;
    template <typename U>
    friend void swap(StackConstIterator<U>& it1, StackConstIterator<U>& it2) noexcept;

private:
    const T* _current;
};

/*! \example intrusive_stack.cpp Intrusive stack container example */

} // namespace CppCommon

#include "stack.inl"

#endif // CPPCOMMON_CONTAINERS_STACK_H
