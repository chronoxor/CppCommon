/*!
    \file hashmap.h
    \brief Hash map container definition
    \author Ivan Shynkarenka
    \date 11.07.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_CONTAINERS_HASHMAP_H
#define CPPCOMMON_CONTAINERS_HASHMAP_H

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <vector>

namespace CppCommon {

template <class TContainer, typename TKey, typename TValue>
class HashMapIterator;
template <class TContainer, typename TKey, typename TValue>
class HashMapConstIterator;
template <class TContainer, typename TKey, typename TValue>
class HashMapReverseIterator;
template <class TContainer, typename TKey, typename TValue>
class HashMapConstReverseIterator;

//! Hash map container
/*!
    Hash map is an efficient  structure  for  associative  keys/value  storing  and
    accessing without keeping order. It uses hash function to  convert  string  key
    into the integer and use the index to quick access value data.

    Open  address  hash map resolves collisions of the  same  hash  values  by
    inserting new item into the next free place (probing with step 1).

    Not thread-safe.
*/
template <typename TKey, typename TValue, typename THash = std::hash<TKey>, typename TEqual = std::equal_to<TKey>, typename TAllocator = std::allocator<std::pair<TKey, TValue>>>
class HashMap
{
    friend class HashMapIterator<HashMap<TKey, TValue, THash, TEqual, TAllocator>, TKey, TValue>;
    friend class HashMapConstIterator<HashMap<TKey, TValue, THash, TEqual, TAllocator>, TKey, TValue>;
    friend class HashMapReverseIterator<HashMap<TKey, TValue, THash, TEqual, TAllocator>, TKey, TValue>;
    friend class HashMapConstReverseIterator<HashMap<TKey, TValue, THash, TEqual, TAllocator>, TKey, TValue>;

public:
    // Standard container type definitions
    typedef TKey key_type;
    typedef TValue mapped_type;
    typedef std::pair<TKey, TValue> value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef HashMapIterator<HashMap<TKey, TValue, THash, TEqual, TAllocator>, TKey, TValue> iterator;
    typedef HashMapConstIterator<HashMap<TKey, TValue, THash, TEqual, TAllocator>, TKey, TValue> const_iterator;
    typedef HashMapReverseIterator<HashMap<TKey, TValue, THash, TEqual, TAllocator>, TKey, TValue> reverse_iterator;
    typedef HashMapConstReverseIterator<HashMap<TKey, TValue, THash, TEqual, TAllocator>, TKey, TValue> const_reverse_iterator;

    //! Initialize the hash map with a given capacity and blank key value
    /*!
        \param capacity - Hash map capacity (default is 128)
        \param blank - Blank key value (default is TKey())
        \param hash - Key hasher (default is THash())
        \param equal - Key comparator (default is THash())
        \param allocator - Allocator (default is TAllocator())
    */
    explicit HashMap(size_t capacity = 128, const TKey& blank = TKey(), const THash& hash = THash(), const TEqual& equal = TEqual(), const TAllocator& allocator = TAllocator());
    template <class InputIterator>
    HashMap(InputIterator first, InputIterator last, bool unused, size_t capacity = 128, const TKey& blank = TKey(), const THash& hash = THash(), const TEqual& equal = TEqual(), const TAllocator& allocator = TAllocator());
    HashMap(const HashMap& hashmap);
    HashMap(const HashMap& hashmap, size_t capacity);
    HashMap(HashMap&&) = default;
    ~HashMap() = default;

    HashMap& operator=(const HashMap& hashmap);
    HashMap& operator=(HashMap&&) = default;

    //! Check if the hash map is not empty
    explicit operator bool() const noexcept { return !empty(); }

    //! Access to the item with the given key or insert a new one
    mapped_type& operator[](const TKey& key) { return emplace_internal(key).first->second; }

    //! Is the hash map empty?
    bool empty() const noexcept { return _size == 0; }

    //! Get the hash map size
    size_t size() const noexcept { return _size; }
    //! Get the hash map maximum size
    size_t max_size() const noexcept { return std::numeric_limits<size_type>::max(); }
    //! Get the hash map bucket count
    size_t bucket_count() const noexcept { return _buckets.size(); }
    //! Get the hash map maximum bucket count
    size_t max_bucket_count() const noexcept { return std::numeric_limits<size_type>::max(); }

    //! Calculate hash of the given key
    size_t key_hash(const TKey& key) const noexcept { return _hash(key); }
    //! Compare two keys: if the first key equals to the second one?
    bool key_equal(const TKey& key1, const TKey& key2) const noexcept { return _equal(key1, key2); }

    //! Get the begin hash map iterator
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    //! Get the end hash map iterator
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    //! Get the reverse begin hash map iterator
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    //! Get the reverse end hash map iterator
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    //! Find the iterator which points to the first item with the given key in the hash map or return end iterator
    iterator find(const TKey& key) noexcept;
    const_iterator find(const TKey& key) const noexcept;

    //! Find the bounds of a range that includes all the elements in the hash map with the given key
    std::pair<iterator, iterator> equal_range(const TKey& key) noexcept;
    std::pair<const_iterator, const_iterator> equal_range(const TKey& key) const noexcept;

    //! Find the count of items with the given key
    size_t count(const TKey& key) const noexcept { return (find(key) == end()) ? 0 : 1; }

    //! Access to the item with the given key or throw std::out_of_range exception
    /*!
        \param key - Key of the item
        \return Item with the given key
    */
    mapped_type& at(const TKey& key) noexcept;
    //! Access to the constant item with the given key or throw std::out_of_range exception
    /*!
        \param key - Key of the item
        \return Constant item with the given key
    */
    const mapped_type& at(const TKey& key) const noexcept;

    //! Insert a new item into the hash map
    /*!
        \param item - Item to insert as a key/value pair
        \return Pair with the iterator to the given key and success flag
    */
    std::pair<iterator, bool> insert(const value_type& item);
    //! Insert a new item into the hash map
    /*!
        \param item - Item to insert as a key/value pair
        \return Pair with the iterator to the given key and success flag
    */
    std::pair<iterator, bool> insert(value_type&& item);

    //! Emplace a new item into the hash map
    /*!
        \param args - Arguments to emplace
        \return Pair with the iterator to the given key and success flag
    */
    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args);

    //! Erase the item with the given key from the hash map
    /*!
        \param key - Key of the item to erase
        \return Number of erased elements (0 or 1 for the hash map)
    */
    size_t erase(const TKey& key);
    //! Erase the item by its iterator from the hash map
    /*!
        \param position - Iterator position to the erased item
    */
    void erase(const const_iterator& position);

    //! Rehash the hash map to the given capacity or more
    /*!
        \param capacity - Hash map capacity
    */
    void rehash(size_t capacity);
    //! Reserve the hash map capacity to fit the given count of items
    /*!
        \param count - Count of items to fit
    */
    void reserve(size_t count);

    //! Clear the hash map
    void clear() noexcept;

    //! Swap two instances
    void swap(HashMap& hashmap) noexcept;
    template <typename UKey, typename UValue, typename UHash, typename UEqual, typename UAllocator>
    friend void swap(HashMap<UKey, UValue, UHash, UEqual, UAllocator>& hashmap1, HashMap<UKey, UValue, UHash, UEqual, UAllocator>& hashmap2) noexcept;

private:
    THash _hash;    // Hash map key hasher
    TEqual _equal;  // Hash map key comparator
    TKey _blank;    // Hash map blank key
    size_t _size;   // Hash map size
    std::vector<value_type, TAllocator> _buckets; // Hash map buckets

    template <typename... Args>
    std::pair<iterator, bool> emplace_internal(const TKey& key, Args&&... args);
    void erase_internal(size_t index);
    size_t key_to_index(const TKey& key) const noexcept;
    size_t next_index(size_t index) const noexcept;
    size_t diff(size_t index1, size_t index2) const noexcept;
};

//! Hash map iterator
/*!
    Not thread-safe.
*/
template <class TContainer, typename TKey, typename TValue>
class HashMapIterator
{
    friend HashMapConstIterator<TContainer, TKey, TValue>;
    friend TContainer;

public:
    // Standard iterator type definitions
    typedef std::pair<TKey, TValue> value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    HashMapIterator() noexcept : _container(nullptr), _index(0) {}
    explicit HashMapIterator(TContainer* container) noexcept;
    explicit HashMapIterator(TContainer* container, size_t index) noexcept : _container(container), _index(index) {}
    HashMapIterator(const HashMapIterator& it) noexcept = default;
    HashMapIterator(HashMapIterator&& it) noexcept = default;
    ~HashMapIterator() noexcept = default;

    HashMapIterator& operator=(const HashMapIterator& it) noexcept = default;
    HashMapIterator& operator=(HashMapIterator&& it) noexcept = default;

    friend bool operator==(const HashMapIterator& it1, const HashMapIterator& it2) noexcept
    { return (it1._container == it2._container) && (it1._index == it2._index); }
    friend bool operator!=(const HashMapIterator& it1, const HashMapIterator& it2) noexcept
    { return (it1._container != it2._container) || (it1._index != it2._index); }

    HashMapIterator& operator++() noexcept;
    HashMapIterator operator++(int) noexcept;

    reference operator*() noexcept;
    pointer operator->() noexcept;

    //! Check if the iterator is valid
    explicit operator bool() const noexcept { return _container != nullptr; }

    //! Swap two instances
    void swap(HashMapIterator& it) noexcept;
    template <class UContainer, typename UKey, typename UValue>
    friend void swap(HashMapIterator<UContainer, UKey, UValue>& it1, HashMapIterator<UContainer, UKey, UValue>& it2) noexcept;

private:
    TContainer* _container;
    size_t _index;
};

//! Hash map constant iterator
/*!
    Not thread-safe.
*/
template <class TContainer, typename TKey, typename TValue>
class HashMapConstIterator
{
    friend TContainer;

public:
    // Standard iterator type definitions
    typedef std::pair<TKey, TValue> value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    HashMapConstIterator() noexcept : _container(nullptr), _index(0) {}
    explicit HashMapConstIterator(const TContainer* container) noexcept;
    explicit HashMapConstIterator(const TContainer* container, size_t index) noexcept : _container(container), _index(index) {}
    HashMapConstIterator(const HashMapIterator<TContainer, TKey, TValue>& it) noexcept : _container(it._container), _index(it._index) {}
    HashMapConstIterator(const HashMapConstIterator& it) noexcept = default;
    HashMapConstIterator(HashMapConstIterator&& it) noexcept = default;
    ~HashMapConstIterator() noexcept = default;

    HashMapConstIterator& operator=(const HashMapIterator<TContainer, TKey, TValue>& it) noexcept
    { _container = it._container; _index = it._index; return *this; }
    HashMapConstIterator& operator=(const HashMapConstIterator& it) noexcept = default;
    HashMapConstIterator& operator=(HashMapConstIterator&& it) noexcept = default;

    friend bool operator==(const HashMapConstIterator& it1, const HashMapConstIterator& it2) noexcept
    { return (it1._container == it2._container) && (it1._index == it2._index); }
    friend bool operator!=(const HashMapConstIterator& it1, const HashMapConstIterator& it2) noexcept
    { return (it1._container != it2._container) || (it1._index != it2._index); }

    HashMapConstIterator& operator++() noexcept;
    HashMapConstIterator operator++(int) noexcept;

    const_reference operator*() const noexcept;
    const_pointer operator->() const noexcept;

    //! Check if the iterator is valid
    explicit operator bool() const noexcept { return _container != nullptr; }

    //! Swap two instances
    void swap(HashMapConstIterator& it) noexcept;
    template <class UContainer, typename UKey, typename UValue>
    friend void swap(HashMapConstIterator<UContainer, UKey, UValue>& it1, HashMapConstIterator<UContainer, UKey, UValue>& it2) noexcept;

private:
    const TContainer* _container;
    size_t _index;
};

//! Hash map reverse iterator
/*!
    Not thread-safe.
*/
template <class TContainer, typename TKey, typename TValue>
class HashMapReverseIterator
{
    friend HashMapConstReverseIterator<TContainer, TKey, TValue>;
    friend TContainer;

public:
    // Standard iterator type definitions
    typedef std::pair<TKey, TValue> value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    HashMapReverseIterator() noexcept : _container(nullptr), _index(0) {}
    explicit HashMapReverseIterator(TContainer* container) noexcept;
    explicit HashMapReverseIterator(TContainer* container, size_t index) noexcept : _container(container), _index(index) {}
    HashMapReverseIterator(const HashMapReverseIterator& it) noexcept = default;
    HashMapReverseIterator(HashMapReverseIterator&& it) noexcept = default;
    ~HashMapReverseIterator() noexcept = default;

    HashMapReverseIterator& operator=(const HashMapReverseIterator& it) noexcept = default;
    HashMapReverseIterator& operator=(HashMapReverseIterator&& it) noexcept = default;

    friend bool operator==(const HashMapReverseIterator& it1, const HashMapReverseIterator& it2) noexcept
    { return (it1._container == it2._container) && (it1._index == it2._index); }
    friend bool operator!=(const HashMapReverseIterator& it1, const HashMapReverseIterator& it2) noexcept
    { return (it1._container != it2._container) || (it1._index != it2._index); }

    HashMapReverseIterator& operator++() noexcept;
    HashMapReverseIterator operator++(int) noexcept;

    reference operator*() noexcept;
    pointer operator->() noexcept;

    //! Check if the iterator is valid
    explicit operator bool() const noexcept { return _container != nullptr; }

    //! Swap two instances
    void swap(HashMapReverseIterator& it) noexcept;
    template <class UContainer, typename UKey, typename UValue>
    friend void swap(HashMapReverseIterator<UContainer, UKey, UValue>& it1, HashMapReverseIterator<UContainer, UKey, UValue>& it2) noexcept;

private:
    TContainer* _container;
    size_t _index;
};

//! Hash map constant reverse iterator
/*!
    Not thread-safe.
*/
template <class TContainer, typename TKey, typename TValue>
class HashMapConstReverseIterator
{
    friend TContainer;

public:
    // Standard iterator type definitions
    typedef std::pair<TKey, TValue> value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    HashMapConstReverseIterator() noexcept : _container(nullptr), _index(0) {}
    explicit HashMapConstReverseIterator(const TContainer* container) noexcept;
    explicit HashMapConstReverseIterator(const TContainer* container, size_t index) noexcept : _container(container), _index(index) {}
    HashMapConstReverseIterator(const HashMapReverseIterator<TContainer, TKey, TValue>& it) noexcept : _container(it._container), _index(it._index) {}
    HashMapConstReverseIterator(const HashMapConstReverseIterator& it) noexcept = default;
    HashMapConstReverseIterator(HashMapConstReverseIterator&& it) noexcept = default;
    ~HashMapConstReverseIterator() noexcept = default;

    HashMapConstReverseIterator& operator=(const HashMapReverseIterator<TContainer, TKey, TValue>& it) noexcept
    { _container = it._container; _index = it._index; return *this; }
    HashMapConstReverseIterator& operator=(const HashMapConstReverseIterator& it) noexcept = default;
    HashMapConstReverseIterator& operator=(HashMapConstReverseIterator&& it) noexcept = default;

    friend bool operator==(const HashMapConstReverseIterator& it1, const HashMapConstReverseIterator& it2) noexcept
    { return (it1._container == it2._container) && (it1._index == it2._index); }
    friend bool operator!=(const HashMapConstReverseIterator& it1, const HashMapConstReverseIterator& it2) noexcept
    { return (it1._container != it2._container) || (it1._index != it2._index); }

    HashMapConstReverseIterator& operator++() noexcept;
    HashMapConstReverseIterator operator++(int) noexcept;

    const_reference operator*() const noexcept;
    const_pointer operator->() const noexcept;

    //! Check if the iterator is valid
    explicit operator bool() const noexcept { return _container != nullptr; }

    //! Swap two instances
    void swap(HashMapConstReverseIterator& it) noexcept;
    template <class UContainer, typename UKey, typename UValue>
    friend void swap(HashMapConstReverseIterator<UContainer, UKey, UValue>& it1, HashMapConstReverseIterator<UContainer, UKey, UValue>& it2) noexcept;

private:
    const TContainer* _container;
    size_t _index;
};

/*! \example containers_hashmap.cpp Hash map container example */

} // namespace CppCommon

#include "hashmap.inl"

#endif // CPPCOMMON_CONTAINERS_HASHMAP_H
