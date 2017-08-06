/*!
    \file bintree.inl
    \brief Intrusive non balanced binary tree container inline implementation
    \author Ivan Shynkarenka
    \date 22.06.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T, typename TCompare>
template <class InputIterator>
inline BinTree<T, TCompare>::BinTree(InputIterator first, InputIterator last, const TCompare& compare) noexcept
    : _compare(compare)
{
    for (auto it = first; it != last; ++it)
        insert(*it);
}

template <typename T, typename TCompare>
inline T* BinTree<T, TCompare>::lowest() noexcept
{
    return (T*)InternalLowest();
}

template <typename T, typename TCompare>
inline const T* BinTree<T, TCompare>::lowest() const noexcept
{
    return InternalLowest();
}

template <typename T, typename TCompare>
inline const T* BinTree<T, TCompare>::InternalLowest() const noexcept
{
    const T* result = _root;
    if (result != nullptr)
        while (result->left != nullptr)
            result = result->left;
    return result;
}

template <typename T, typename TCompare>
inline T* BinTree<T, TCompare>::highest() noexcept
{
    return (T*)InternalHighest();
}

template <typename T, typename TCompare>
inline const T* BinTree<T, TCompare>::highest() const noexcept
{
    return InternalHighest();
}

template <typename T, typename TCompare>
inline const T* BinTree<T, TCompare>::InternalHighest() const noexcept
{
    const T* result = _root;
    if (result != nullptr)
        while (result->right != nullptr)
            result = result->right;
    return result;
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::iterator BinTree<T, TCompare>::begin() noexcept
{
    return iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::const_iterator BinTree<T, TCompare>::begin() const noexcept
{
    return const_iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::const_iterator BinTree<T, TCompare>::cbegin() const noexcept
{
    return const_iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::iterator BinTree<T, TCompare>::end() noexcept
{
    return iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::const_iterator BinTree<T, TCompare>::end() const noexcept
{
    return const_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::const_iterator BinTree<T, TCompare>::cend() const noexcept
{
    return const_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::reverse_iterator BinTree<T, TCompare>::rbegin() noexcept
{
    return reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::const_reverse_iterator BinTree<T, TCompare>::rbegin() const noexcept
{
    return const_reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::const_reverse_iterator BinTree<T, TCompare>::crbegin() const noexcept
{
    return const_reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::reverse_iterator BinTree<T, TCompare>::rend() noexcept
{
    return reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::const_reverse_iterator BinTree<T, TCompare>::rend() const noexcept
{
    return const_reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::const_reverse_iterator BinTree<T, TCompare>::crend() const noexcept
{
    return const_reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::iterator BinTree<T, TCompare>::find(const T& item) noexcept
{
    return iterator(this, (T*)InternalFind(item));
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::const_iterator BinTree<T, TCompare>::find(const T& item) const noexcept
{
    return const_iterator(this, InternalFind(item));
}

template <typename T, typename TCompare>
inline const T* BinTree<T, TCompare>::InternalFind(const T& item) const noexcept
{
    // Perform the binary tree search from the root node
    const T* current = _root;

    while (current != nullptr)
    {
        // Move to the left subtree
        if (compare(item, *current))
        {
            current = current->left;
            continue;
        }

        // Move to the right subtree
        if (compare(*current, item))
        {
            current = current->right;
            continue;
        }

        // Found result node
        return current;
    }

    // Nothing was found...
    return nullptr;
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::iterator BinTree<T, TCompare>::lower_bound(const T& item) noexcept
{
    return iterator(this, (T*)InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::const_iterator BinTree<T, TCompare>::lower_bound(const T& item) const noexcept
{
    return const_iterator(this, InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline const T* BinTree<T, TCompare>::InternalLowerBound(const T& item) const noexcept
{
    // Perform the binary tree search from the root node
    const T* current = _root;
    const T* previous = nullptr;

    while (current != nullptr)
    {
        // Move to the left subtree
        if (compare(item, *current))
        {
            previous = current;
            current = current->left;
            continue;
        }

        // Move to the right subtree
        if (compare(*current, item))
        {
            current = current->right;
            continue;
        }

        // Found result node
        return current;
    }

    // Return the previous lower bound node if any met
    return previous;
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::iterator BinTree<T, TCompare>::upper_bound(const T& item) noexcept
{
    return iterator(this, (T*)InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::const_iterator BinTree<T, TCompare>::upper_bound(const T& item) const noexcept
{
    return const_iterator(this, InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline const T* BinTree<T, TCompare>::InternalUpperBound(const T& item) const noexcept
{
    // Perform the binary tree search from the root node
    const T* current = _root;
    const T* previous = nullptr;

    while (current != nullptr)
    {
        // Move to the left subtree
        if (compare(item, *current))
        {
            previous = current;
            current = current->left;
            continue;
        }

        // Move to the right subtree
        current = current->right;
    }

    // Return the previous upper bound node if any met
    return previous;
}

template <typename T, typename TCompare>
inline std::pair<typename BinTree<T, TCompare>::iterator, bool> BinTree<T, TCompare>::insert(T& item) noexcept
{
    return insert(const_iterator(this, _root), item);
}

template <typename T, typename TCompare>
inline std::pair<typename BinTree<T, TCompare>::iterator, bool> BinTree<T, TCompare>::insert(const const_iterator& position, T& item) noexcept
{
    // Perform the binary tree insert from the given node
    T* current = (T*)position.operator->();

    while (current != nullptr)
    {
        // Move to the left subtree
        if (compare(item, *current))
        {
            if (current->left != nullptr)
            {
                current = current->left;
                continue;
            }
            else
            {
                // Link a new item to the left leaf
                current->left = &item;
                break;
            }
        }

        // Move to the right subtree
        if (compare(*current, item))
        {
            if (current->right != nullptr)
            {
                current = current->right;
                continue;
            }
            else
            {
                // Link a new item to the right leaf
                current->right = &item;
                break;
            }
        }

        // Found duplicate node
        return std::make_pair(iterator(this, current), false);
    }

    item.parent = current;
    item.left = nullptr;
    item.right = nullptr;
    if (_root == nullptr)
        _root = &item;
    ++_size;

    return std::make_pair(iterator(this, &item), true);
}

template <typename T, typename TCompare>
inline T* BinTree<T, TCompare>::erase(const T& item) noexcept
{
    return erase(find(item)).operator->();
}

template <typename T, typename TCompare>
inline typename BinTree<T, TCompare>::iterator BinTree<T, TCompare>::erase(const iterator& it) noexcept
{
    T* result = ((iterator&)it).operator->();
    if (result == nullptr)
        return end();

    T* parent = result->parent;
    T* left = result->left;
    T* right = result->right;

    // Binary tree node without left child
    if (left == nullptr)
    {
        // Link the parent node with a right child
        if (parent != nullptr)
        {
            if (parent->left == result)
                parent->left = right;
            else
                parent->right = right;
        }
        else
            _root = right;
        if (right != nullptr)
            right->parent = parent;
    }
    // Binary tree node without right child
    else if (right == nullptr)
    {
        // Link the parent node with a left child
        if (parent != nullptr)
        {
            if (parent->left == result)
                parent->left = left;
            else
                parent->right = left;
        }
        else
            _root = left;
        if (left != nullptr)
            left->parent = parent;
    }
    // Binary tree node with both left and right childs
    else
    {
        // Link the parent node with a left child
        if (parent != nullptr)
        {
            if (parent->left == result)
                parent->left = left;
            else
                parent->right = left;
        }
        else
            _root = left;
        left->parent = parent;

        // Find a new base node
        T* temp = left;
        while (temp->right != nullptr)
            temp = temp->right;
        temp->right = right;
        right->parent = temp;
    }

    result->parent = nullptr;
    result->left = nullptr;
    result->right = nullptr;
    --_size;
    return iterator(this, result);
}

template <typename T, typename TCompare>
inline void BinTree<T, TCompare>::clear() noexcept
{
    _size = 0;
    _root = nullptr;
}

template <typename T, typename TCompare>
inline void BinTree<T, TCompare>::swap(BinTree& bintree) noexcept
{
    using std::swap;
    swap(_compare, bintree._compare);
    swap(_size, bintree._size);
    swap(_root, bintree._root);
}

template <typename T, typename TCompare>
inline void swap(BinTree<T, TCompare>& bintree1, BinTree<T, TCompare>& bintree2) noexcept
{
    bintree1.swap(bintree2);
}

template <class TContainer, typename T>
BinTreeIterator<TContainer, T>& BinTreeIterator<TContainer, T>::operator++() noexcept
{
    if (_node != nullptr)
    {
        if (_node->right != nullptr)
        {
            _node = _node->right;
            while (_node->left != nullptr)
                _node = _node->left;
        }
        else
        {
            while ((_node->parent != nullptr) && compare(*_node->parent, *_node))
                _node = _node->parent;
            _node = _node->parent;
        }
    }
    return *this;
}

template <class TContainer, typename T>
inline BinTreeIterator<TContainer, T> BinTreeIterator<TContainer, T>::operator++(int) noexcept
{
    BinTreeIterator<TContainer, T> result(*this);
    operator++();
    return result;
}

template <class TContainer, typename T>
typename BinTreeIterator<TContainer, T>::reference BinTreeIterator<TContainer, T>::operator*() noexcept
{
    assert((_node != nullptr) && "Iterator must be valid!");

    return *_node;
}

template <class TContainer, typename T>
typename BinTreeIterator<TContainer, T>::pointer BinTreeIterator<TContainer, T>::operator->() noexcept
{
    return _node;
}

template <class TContainer, typename T>
void BinTreeIterator<TContainer, T>::swap(BinTreeIterator& it) noexcept
{
    using std::swap;
    swap(_container, it._container);
    swap(_node, it._node);
}

template <class TContainer, typename T>
void swap(BinTreeIterator<TContainer, T>& it1, BinTreeIterator<TContainer, T>& it2) noexcept
{
    it1.swap(it2);
}

template <class TContainer, typename T>
BinTreeConstIterator<TContainer, T>& BinTreeConstIterator<TContainer, T>::operator++() noexcept
{
    if (_node != nullptr)
    {
        if (_node->right != nullptr)
        {
            _node = _node->right;
            while (_node->left != nullptr)
                _node = _node->left;
        }
        else
        {
            while ((_node->parent != nullptr) && compare(*_node->parent, *_node))
                _node = _node->parent;
            _node = _node->parent;
        }
    }
    return *this;
}

template <class TContainer, typename T>
inline BinTreeConstIterator<TContainer, T> BinTreeConstIterator<TContainer, T>::operator++(int) noexcept
{
    BinTreeConstIterator<TContainer, T> result(*this);
    operator++();
    return result;
}

template <class TContainer, typename T>
typename BinTreeConstIterator<TContainer, T>::const_reference BinTreeConstIterator<TContainer, T>::operator*() const noexcept
{
    assert((_node != nullptr) && "Iterator must be valid!");

    return *_node;
}

template <class TContainer, typename T>
typename BinTreeConstIterator<TContainer, T>::const_pointer BinTreeConstIterator<TContainer, T>::operator->() const noexcept
{
    return _node;
}

template <class TContainer, typename T>
void BinTreeConstIterator<TContainer, T>::swap(BinTreeConstIterator& it) noexcept
{
    using std::swap;
    swap(_container, it._container);
    swap(_node, it._node);
}

template <class TContainer, typename T>
void swap(BinTreeConstIterator<TContainer, T>& it1, BinTreeConstIterator<TContainer, T>& it2) noexcept
{
    it1.swap(it2);
}

template <class TContainer, typename T>
BinTreeReverseIterator<TContainer, T>& BinTreeReverseIterator<TContainer, T>::operator++() noexcept
{
    if (_node != nullptr)
    {
        if (_node->left != nullptr)
        {
            _node = _node->left;
            while (_node->right != nullptr)
                _node = _node->right;
        }
        else
        {
            while ((_node->parent != nullptr) && compare(*_node, *_node->parent))
                _node = _node->parent;
            _node = _node->parent;
        }
    }
    return *this;
}

template <class TContainer, typename T>
inline BinTreeReverseIterator<TContainer, T> BinTreeReverseIterator<TContainer, T>::operator++(int) noexcept
{
    BinTreeReverseIterator<TContainer, T> result(*this);
    operator++();
    return result;
}

template <class TContainer, typename T>
typename BinTreeReverseIterator<TContainer, T>::reference BinTreeReverseIterator<TContainer, T>::operator*() noexcept
{
    assert((_node != nullptr) && "Iterator must be valid!");

    return *_node;
}

template <class TContainer, typename T>
typename BinTreeReverseIterator<TContainer, T>::pointer BinTreeReverseIterator<TContainer, T>::operator->() noexcept
{
    return _node;
}

template <class TContainer, typename T>
void BinTreeReverseIterator<TContainer, T>::swap(BinTreeReverseIterator& it) noexcept
{
    using std::swap;
    swap(_container, it._container);
    swap(_node, it._node);
}

template <class TContainer, typename T>
void swap(BinTreeReverseIterator<TContainer, T>& it1, BinTreeReverseIterator<TContainer, T>& it2) noexcept
{
    it1.swap(it2);
}

template <class TContainer, typename T>
BinTreeConstReverseIterator<TContainer, T>& BinTreeConstReverseIterator<TContainer, T>::operator++() noexcept
{
    if (_node != nullptr)
    {
        if (_node->left != nullptr)
        {
            _node = _node->left;
            while (_node->right != nullptr)
                _node = _node->right;
        }
        else
        {
            while ((_node->parent != nullptr) && compare(*_node, *_node->parent))
                _node = _node->parent;
            _node = _node->parent;
        }
    }
    return *this;
}

template <class TContainer, typename T>
inline BinTreeConstReverseIterator<TContainer, T> BinTreeConstReverseIterator<TContainer, T>::operator++(int) noexcept
{
    BinTreeConstReverseIterator<TContainer, T> result(*this);
    operator++();
    return result;
}

template <class TContainer, typename T>
typename BinTreeConstReverseIterator<TContainer, T>::const_reference BinTreeConstReverseIterator<TContainer, T>::operator*() const noexcept
{
    assert((_node != nullptr) && "Iterator must be valid!");

    return *_node;
}

template <class TContainer, typename T>
typename BinTreeConstReverseIterator<TContainer, T>::const_pointer BinTreeConstReverseIterator<TContainer, T>::operator->() const noexcept
{
    return _node;
}

template <class TContainer, typename T>
void BinTreeConstReverseIterator<TContainer, T>::swap(BinTreeConstReverseIterator& it) noexcept
{
    using std::swap;
    swap(_container, it._container);
    swap(_node, it._node);
}

template <class TContainer, typename T>
void swap(BinTreeConstReverseIterator<TContainer, T>& it1, BinTreeConstReverseIterator<TContainer, T>& it2) noexcept
{
    it1.swap(it2);
}

} // namespace CppCommon
