/*!
    \file flatmap.h
    \brief Flat map container definition
    \author Ivan Shynkarenka
    \date 27.07.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_CONTAINERS_FLATMAP_H
#define CPPCOMMON_CONTAINERS_FLATMAP_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <vector>

namespace CppCommon {

//! Flat map container
/*!
    Flat map is an efficient  structure  for  associative  keys/value  storing  and
    accessing with keeping order. All key/value items  are  stored  in  the  sorted
    array container with using binary search algorithm to  find  the  item  by  the
    given key.

    Not thread-safe.
*/
template <typename TKey, typename TValue, typename TCompare = std::less<TKey>, typename TAllocator = std::allocator<std::pair<TKey, TValue>>>
class FlatMap
{
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
    typedef typename std::vector<value_type, TAllocator>::iterator iterator;
    typedef typename std::vector<value_type, TAllocator>::const_iterator const_iterator;
    typedef typename std::vector<value_type, TAllocator>::reverse_iterator reverse_iterator;
    typedef typename std::vector<value_type, TAllocator>::const_reverse_iterator const_reverse_iterator;

    //! Initialize the flat map with a given capacity
    /*!
        \param capacity - Flat map capacity (default is 128)
        \param compare - Key comparator (default is TCompare())
        \param allocator - Allocator (default is TAllocator())
    */
    explicit FlatMap(size_t capacity = 128, const TCompare& compare = TCompare(), const TAllocator& allocator = TAllocator());
    template <class InputIterator>
    FlatMap(InputIterator first, InputIterator last, bool unused, size_t capacity = 128, const TCompare& compare = TCompare(), const TAllocator& allocator = TAllocator());
    FlatMap(const FlatMap& flatmap);
    FlatMap(const FlatMap& flatmap, size_t capacity);
    FlatMap(FlatMap&&) noexcept = default;
    ~FlatMap() = default;

    FlatMap& operator=(const FlatMap& flatmap);
    FlatMap& operator=(FlatMap&&) noexcept = default;

    //! Check if the flat map is not empty
    explicit operator bool() const noexcept { return !empty(); }

    //! Access to the item with the given key or insert a new one
    mapped_type& operator[](const TKey& key) { return emplace_internal(key).first->second; }

    //! Is the flat map empty?
    bool empty() const noexcept { return _container.empty(); }

    //! Get the flat map capacity
    size_t capacity() const noexcept { return _container.capacity(); }
    //! Get the flat map size
    size_t size() const noexcept { return _container.size(); }
    //! Get the flat map maximum size
    size_t max_size() const noexcept { return _container.max_size(); }

    //! Compare two items: if the first key is less than the second one?
    bool compare(const TKey& key1, const TKey& key2) const noexcept { return _compare(key1, key2); }
    bool compare(const TKey& key1, const value_type& key2) const noexcept { return _compare(key1, key2.first); }
    bool compare(const value_type& key1, const TKey& key2) const noexcept { return _compare(key1.first, key2); }
    bool compare(const value_type& key1, const value_type& key2) const noexcept { return _compare(key1.first, key2.first); }

    //! Get the begin flat map iterator
    iterator begin() noexcept { return _container.begin(); }
    const_iterator begin() const noexcept { return _container.begin(); }
    const_iterator cbegin() const noexcept { return _container.begin(); }
    //! Get the end flat map iterator
    iterator end() noexcept { return _container.end(); }
    const_iterator end() const noexcept { return _container.end(); }
    const_iterator cend() const noexcept { return _container.end(); }

    //! Get the reverse begin flat map iterator
    reverse_iterator rbegin() noexcept { return _container.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return _container.rbegin(); }
    const_reverse_iterator crbegin() const noexcept { return _container.rbegin(); }
    //! Get the reverse end flat map iterator
    reverse_iterator rend() noexcept { return _container.rend(); }
    const_reverse_iterator rend() const noexcept { return _container.rend(); }
    const_reverse_iterator crend() const noexcept { return _container.rend(); }

    //! Find the iterator which points to the first item with the given key in the flat map or return end iterator
    iterator find(const TKey& key) noexcept;
    const_iterator find(const TKey& key) const noexcept;

    //! Find the iterator which points to the first item with the given key that not less than the given key in the flat map or return end iterator
    iterator lower_bound(const TKey& key) noexcept;
    const_iterator lower_bound(const TKey& key) const noexcept;
    //! Find the iterator which points to the first item with the given key that greater than the given key in the flat map or return end iterator
    iterator upper_bound(const TKey& key) noexcept;
    const_iterator upper_bound(const TKey& key) const noexcept;

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

    //! Insert a new item into the flat map
    /*!
        \param item - Item to insert as a key/value pair
        \return Pair with the iterator to the given key and success flag
    */
    std::pair<iterator, bool> insert(const value_type& item);
    //! Insert a new item into the flat map
    /*!
        \param item - Item to insert as a key/value pair
        \return Pair with the iterator to the given key and success flag
    */
    std::pair<iterator, bool> insert(value_type&& item);
    //! Insert a new item into the flat map with a position hint
    /*!
        \param position - Iterator position to the inserted item
        \param item - Item to insert as a key/value pair
        \return Iterator to the given key
    */
    iterator insert(const const_iterator& position, const value_type& item);
    //! Insert a new item into the flat map with a position hint
    /*!
        \param position - Iterator position to the inserted item
        \param item - Item to insert as a key/value pair
        \return Iterator to the given key
    */
    iterator insert(const const_iterator& position, value_type&& item);
    //! Insert all items into the flat map from the given iterators range
    /*!
        \param first - The first iterator of the inserted range
        \param last - The last iterator of the inserted range
    */
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last);

    //! Emplace a new item into the flat map
    /*!
        \param args - Arguments to emplace
        \return Pair with the iterator to the given key and success flag
    */
    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args);
    //! Emplace a new item into the flat map with a position hint
    /*!
        \param position - Iterator position to the emplaced item
        \param args - Arguments to emplace
        \return Iterator to the given key
    */
    template <typename... Args>
    iterator emplace_hint(const const_iterator& position, Args&&... args);

    //! Erase the item with the given key from the flat map
    /*!
        \param key - Key of the item to erase
        \return Number of erased elements (0 or 1 for the hash map)
    */
    size_t erase(const TKey& key);
    //! Erase the item by its iterator from the flat map
    /*!
        \param position - Iterator position to the erased item
        \return Iterator pointing to the position immediately following the last of the elements erased
    */
    iterator erase(const const_iterator& position);
    //! Erase all items between two given iterators from the flat map
    /*!
        \param first - The first iterator of the erased range
        \param last - The last iterator of the erased range
        \return Iterator pointing to the position immediately following the last of the elements erased
    */
    iterator erase(const const_iterator& first, const const_iterator& last);

    //! Reserve the flat map capacity to fit the given count of items
    /*!
        \param count - Count of items to fit
    */
    void reserve(size_t count) { _container.reserve(count); }
    //! Requests the flat map to reduce its capacity to fit its size
    /*!
        This may cause a reallocation, but has no effect on the vector size and cannot alter its elements.
    */
    void shrink_to_fit() { _container.shrink_to_fit(); }

    //! Clear the flat map
    void clear() noexcept { _container.clear(); }

    //! Swap two instances
    void swap(FlatMap& flatmap) noexcept;
    template <typename UKey, typename UValue, typename UCompare, typename UAllocator>
    friend void swap(FlatMap<UKey, UValue, UCompare, UAllocator>& flatmap1, FlatMap<UKey, UValue, UCompare, UAllocator>& flatmap2) noexcept;

private:
    TCompare _compare;                              // Flat map key comparator
    std::vector<value_type, TAllocator> _container; // Flat map container

    template <typename... Args>
    std::pair<iterator, bool> emplace_internal(const TKey& key, Args&&... args);
    template <typename... Args>
    iterator emplace_hint_internal(const const_iterator& position, const TKey& key, Args&&... args);
};

/*! \example containers_flatmap.cpp Flat map container example */

} // namespace CppCommon

#include "flatmap.inl"

#endif // CPPCOMMON_CONTAINERS_FLATMAP_H
