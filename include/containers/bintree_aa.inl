/*!
    \file bintree_aa.inl
    \brief Intrusive balanced A.Andersson binary tree container inline implementation
    \author Ivan Shynkarenka
    \date 23.06.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T, typename TCompare>
template <class InputIterator>
inline BinTreeAA<T, TCompare>::BinTreeAA(InputIterator first, InputIterator last, const TCompare& compare) noexcept
    : _compare(compare)
{
    for (auto it = first; it != last; ++it)
        insert(*it);
}

template <typename T, typename TCompare>
inline T* BinTreeAA<T, TCompare>::lowest() noexcept
{
    return (T*)InternalLowest();
}

template <typename T, typename TCompare>
inline const T* BinTreeAA<T, TCompare>::lowest() const noexcept
{
    return InternalLowest();
}

template <typename T, typename TCompare>
inline const T* BinTreeAA<T, TCompare>::InternalLowest() const noexcept
{
    const T* result = _root;
    if (result != nullptr)
        while (result->left != nullptr)
            result = result->left;
    return result;
}

template <typename T, typename TCompare>
inline T* BinTreeAA<T, TCompare>::highest() noexcept
{
    return (T*)InternalHighest();
}

template <typename T, typename TCompare>
inline const T* BinTreeAA<T, TCompare>::highest() const noexcept
{
    return InternalHighest();
}

template <typename T, typename TCompare>
inline const T* BinTreeAA<T, TCompare>::InternalHighest() const noexcept
{
    const T* result = _root;
    if (result != nullptr)
        while (result->right != nullptr)
            result = result->right;
    return result;
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::iterator BinTreeAA<T, TCompare>::begin() noexcept
{
    return iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::const_iterator BinTreeAA<T, TCompare>::begin() const noexcept
{
    return const_iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::const_iterator BinTreeAA<T, TCompare>::cbegin() const noexcept
{
    return const_iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::iterator BinTreeAA<T, TCompare>::end() noexcept
{
    return iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::const_iterator BinTreeAA<T, TCompare>::end() const noexcept
{
    return const_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::const_iterator BinTreeAA<T, TCompare>::cend() const noexcept
{
    return const_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::reverse_iterator BinTreeAA<T, TCompare>::rbegin() noexcept
{
    return reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::const_reverse_iterator BinTreeAA<T, TCompare>::rbegin() const noexcept
{
    return const_reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::const_reverse_iterator BinTreeAA<T, TCompare>::crbegin() const noexcept
{
    return const_reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::reverse_iterator BinTreeAA<T, TCompare>::rend() noexcept
{
    return reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::const_reverse_iterator BinTreeAA<T, TCompare>::rend() const noexcept
{
    return const_reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::const_reverse_iterator BinTreeAA<T, TCompare>::crend() const noexcept
{
    return const_reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::iterator BinTreeAA<T, TCompare>::find(const T& item) noexcept
{
    return iterator(this, (T*)InternalFind(item));
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::const_iterator BinTreeAA<T, TCompare>::find(const T& item) const noexcept
{
    return const_iterator(this, InternalFind(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeAA<T, TCompare>::InternalFind(const T& item) const noexcept
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
inline typename BinTreeAA<T, TCompare>::iterator BinTreeAA<T, TCompare>::lower_bound(const T& item) noexcept
{
    return iterator(this, (T*)InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::const_iterator BinTreeAA<T, TCompare>::lower_bound(const T& item) const noexcept
{
    return const_iterator(this, InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeAA<T, TCompare>::InternalLowerBound(const T& item) const noexcept
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
inline typename BinTreeAA<T, TCompare>::iterator BinTreeAA<T, TCompare>::upper_bound(const T& item) noexcept
{
    return iterator(this, (T*)InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::const_iterator BinTreeAA<T, TCompare>::upper_bound(const T& item) const noexcept
{
    return const_iterator(this, InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeAA<T, TCompare>::InternalUpperBound(const T& item) const noexcept
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
inline std::pair<typename BinTreeAA<T, TCompare>::iterator, bool> BinTreeAA<T, TCompare>::insert(T& item) noexcept
{
    return insert(const_iterator(this, _root), item);
}

template <typename T, typename TCompare>
inline std::pair<typename BinTreeAA<T, TCompare>::iterator, bool> BinTreeAA<T, TCompare>::insert(const const_iterator& position, T& item) noexcept
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

    // Balance the binary tree
    T* node = &item;
    node->level = 1;
    while (node->parent != nullptr)
    {
        node = node->parent;
        if (node->level != ((node->left != nullptr) ? node->left->level + 1 : 1))
        {
            Skew(node);
            if ((node->right == nullptr) || (node->level != node->right->level))
                node = node->parent;
        }

        if (!Split(node->parent))
            break;
    }

    return std::make_pair(iterator(this, &item), true);
}

template <typename T, typename TCompare>
inline T* BinTreeAA<T, TCompare>::erase(const T& item) noexcept
{
    return erase(find(item)).operator->();
}

template <typename T, typename TCompare>
inline typename BinTreeAA<T, TCompare>::iterator BinTreeAA<T, TCompare>::erase(const iterator& it) noexcept
{
    T* result = ((iterator&)it).operator->();
    if (result == nullptr)
        return end();

    T* node = result;
    T* temp;

    if (result->left != nullptr)
    {
        node = result->left;
        while (node->right != nullptr)
            node = node->right;
    }
    else if (result->right != nullptr)
        node = result->right;

    temp = ((node->parent == result) ? node : node->parent);
    if (node->parent != nullptr)
    {
        if (node->parent->left == node)
            node->parent->left = nullptr;
        else
            node->parent->right = nullptr;
    }
    else
        _root = nullptr;

    if (result != node)
    {
        if (result->parent != nullptr)
        {
            if (result->parent->left == result)
                result->parent->left = node;
            else
                result->parent->right = node;
        }
        else
            _root = node;

        node->parent = result->parent;
        if (result->left != nullptr)
            result->left->parent = node;
        node->left = result->left;
        if (result->right != nullptr)
            result->right->parent = node;
        node->right = result->right;

        // Copy levels
        node->level = result->level;
    }

    while (temp != nullptr)
    {
        if (temp->level > ((temp->left != nullptr) ? temp->left->level + 1 : 1))
        {
            temp->level = temp->level - 1;
            if (Split(temp))
            {
                if (Split(temp))
                    Skew(temp->parent->parent);
                break;
            }
        }
        else if (temp->level <= ((temp->right != nullptr) ? temp->right->level + 1 : 1))
            break;
        else
        {
            Skew(temp);
            if (temp->level > temp->parent->level)
            {
                Skew(temp);
                Split(temp->parent->parent);
                break;
            }
            temp = temp->parent;
        }
        temp = temp->parent;
    }

    result->parent = nullptr;
    result->left = nullptr;
    result->right = nullptr;
    --_size;
    return iterator(this, result);
}

template <typename T, typename TCompare>
inline void BinTreeAA<T, TCompare>::Skew(T* node)
{
    if (node == nullptr)
        return;

    T* current = node->left;
    if (node->parent != nullptr)
    {
        if (node->parent->left == node)
            node->parent->left = current;
        else
            node->parent->right = current;
    }
    else
        _root = current;
    current->parent = node->parent;
    node->parent = current;

    node->left = current->right;
    if (node->left != nullptr)
        node->left->parent = node;
    current->right = node;

    if (node->left != nullptr)
        node->level = node->left->level + 1;
    else
        node->level = 1;
}

template <typename T, typename TCompare>
inline bool BinTreeAA<T, TCompare>::Split(T* node)
{
    if (node == nullptr)
        return false;

    T* current = node->right;
    if ((current != nullptr) && (current->right != nullptr) && (current->right->level == node->level))
    {
        if (node->parent != nullptr)
        {
        if (node->parent->left == node)
            node->parent->left = current;
          else
            node->parent->right = current;
        }
        else
            _root = current;
        current->parent = node->parent;
        node->parent = current;

        node->right = current->left;
        if (node->right != nullptr)
            node->right->parent = node;
        current->left = node;
        current->level = node->level + 1;
        return true;
    }

    return false;
}

template <typename T, typename TCompare>
inline void BinTreeAA<T, TCompare>::clear() noexcept
{
    _size = 0;
    _root = nullptr;
}

template <typename T, typename TCompare>
inline void BinTreeAA<T, TCompare>::swap(BinTreeAA& bintree) noexcept
{
    using std::swap;
    swap(_compare, bintree._compare);
    swap(_size, bintree._size);
    swap(_root, bintree._root);
}

template <typename T, typename TCompare>
inline void swap(BinTreeAA<T, TCompare>& bintree1, BinTreeAA<T, TCompare>& bintree2) noexcept
{
    bintree1.swap(bintree2);
}

} // namespace CppCommon
