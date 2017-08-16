/*!
    \file bintree_avl.inl
    \brief Intrusive balanced AVL binary tree container inline implementation
    \author Ivan Shynkarenka
    \date 23.06.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T, typename TCompare>
template <class InputIterator>
inline BinTreeAVL<T, TCompare>::BinTreeAVL(InputIterator first, InputIterator last, const TCompare& compare) noexcept
    : _compare(compare)
{
    for (auto it = first; it != last; ++it)
        insert(*it);
}

template <typename T, typename TCompare>
inline T* BinTreeAVL<T, TCompare>::lowest() noexcept
{
    return (T*)InternalLowest();
}

template <typename T, typename TCompare>
inline const T* BinTreeAVL<T, TCompare>::lowest() const noexcept
{
    return InternalLowest();
}

template <typename T, typename TCompare>
inline const T* BinTreeAVL<T, TCompare>::InternalLowest() const noexcept
{
    const T* result = _root;
    if (result != nullptr)
        while (result->left != nullptr)
            result = result->left;
    return result;
}

template <typename T, typename TCompare>
inline T* BinTreeAVL<T, TCompare>::highest() noexcept
{
    return (T*)InternalHighest();
}

template <typename T, typename TCompare>
inline const T* BinTreeAVL<T, TCompare>::highest() const noexcept
{
    return InternalHighest();
}

template <typename T, typename TCompare>
inline const T* BinTreeAVL<T, TCompare>::InternalHighest() const noexcept
{
    const T* result = _root;
    if (result != nullptr)
        while (result->right != nullptr)
            result = result->right;
    return result;
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::iterator BinTreeAVL<T, TCompare>::begin() noexcept
{
    return iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::const_iterator BinTreeAVL<T, TCompare>::begin() const noexcept
{
    return const_iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::const_iterator BinTreeAVL<T, TCompare>::cbegin() const noexcept
{
    return const_iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::iterator BinTreeAVL<T, TCompare>::end() noexcept
{
    return iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::const_iterator BinTreeAVL<T, TCompare>::end() const noexcept
{
    return const_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::const_iterator BinTreeAVL<T, TCompare>::cend() const noexcept
{
    return const_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::reverse_iterator BinTreeAVL<T, TCompare>::rbegin() noexcept
{
    return reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::const_reverse_iterator BinTreeAVL<T, TCompare>::rbegin() const noexcept
{
    return const_reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::const_reverse_iterator BinTreeAVL<T, TCompare>::crbegin() const noexcept
{
    return const_reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::reverse_iterator BinTreeAVL<T, TCompare>::rend() noexcept
{
    return reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::const_reverse_iterator BinTreeAVL<T, TCompare>::rend() const noexcept
{
    return const_reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::const_reverse_iterator BinTreeAVL<T, TCompare>::crend() const noexcept
{
    return const_reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::iterator BinTreeAVL<T, TCompare>::find(const T& item) noexcept
{
    return iterator(this, (T*)InternalFind(item));
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::const_iterator BinTreeAVL<T, TCompare>::find(const T& item) const noexcept
{
    return const_iterator(this, InternalFind(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeAVL<T, TCompare>::InternalFind(const T& item) const noexcept
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
inline typename BinTreeAVL<T, TCompare>::iterator BinTreeAVL<T, TCompare>::lower_bound(const T& item) noexcept
{
    return iterator(this, (T*)InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::const_iterator BinTreeAVL<T, TCompare>::lower_bound(const T& item) const noexcept
{
    return const_iterator(this, InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeAVL<T, TCompare>::InternalLowerBound(const T& item) const noexcept
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
inline typename BinTreeAVL<T, TCompare>::iterator BinTreeAVL<T, TCompare>::upper_bound(const T& item) noexcept
{
    return iterator(this, (T*)InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::const_iterator BinTreeAVL<T, TCompare>::upper_bound(const T& item) const noexcept
{
    return const_iterator(this, InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeAVL<T, TCompare>::InternalUpperBound(const T& item) const noexcept
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
inline std::pair<typename BinTreeAVL<T, TCompare>::iterator, bool> BinTreeAVL<T, TCompare>::insert(T& item) noexcept
{
    return insert(const_iterator(this, _root), item);
}

template <typename T, typename TCompare>
inline std::pair<typename BinTreeAVL<T, TCompare>::iterator, bool> BinTreeAVL<T, TCompare>::insert(const const_iterator& position, T& item) noexcept
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
    node->balance = 0;
    while (node->parent != nullptr)
    {
        // Rule 1
        if (((node->parent != nullptr) && (node->parent->left == node)) && (node->parent->balance == 0))
        {
            node->parent->balance = -1;
            node = node->parent;
            continue;
        }
        if (((node->parent != nullptr) && (node->parent->right == node)) && (node->parent->balance == 0))
        {
            node->parent->balance = 1;
            node = node->parent;
            continue;
        }

        // Rule 2
        if (((node->parent != nullptr) && (node->parent->left == node)) && (node->parent->balance == 1))
        {
            node->parent->balance = 0;
            break;
        }
        if (((node->parent != nullptr) && (node->parent->right == node)) && (node->parent->balance == -1))
        {
            node->parent->balance = 0;
            break;
        }

        // Rule 3
        if (((node->parent != nullptr) && (node->parent->left == node)) && (node->parent->balance == -1))
        {
            if (node->balance == 1)
                RotateLeftLeft(node->parent);
            else
                RotateRight(node->parent);
            break;
        }
        if (((node->parent != nullptr) && (node->parent->right == node)) && (node->parent->balance == 1))
        {
            if (node->balance == -1)
                RotateRightRight(node->parent);
            else
                RotateLeft(node->parent);
            break;
        }
    }

    // Correct AVL balanced binary tree root
    while (_root->parent != nullptr)
        _root = _root->parent;

    return std::make_pair(iterator(this, &item), true);
}

template <typename T, typename TCompare>
inline T* BinTreeAVL<T, TCompare>::erase(const T& item) noexcept
{
    return erase(find(item)).operator->();
}

template <typename T, typename TCompare>
inline typename BinTreeAVL<T, TCompare>::iterator BinTreeAVL<T, TCompare>::erase(const iterator& it) noexcept
{
    T* result = ((iterator&)it).operator->();
    if (result == nullptr)
        return end();

    T* node = result;
    T* start = nullptr;

    // If we want to delete the root item, we have to do some extra operation the preserve some pointers...
    if (node->parent == nullptr)
    {
        // The root is the only one node in the tree
        if ((node->left == nullptr) && (node->right == nullptr))
        {
            _root = nullptr;
            node = nullptr;
        }
    }

    // Removed node has no children
    if ((node != nullptr) && (node->left == nullptr) && (node->right == nullptr))
    {
        if (node->parent->left == node)
            node->parent->left=nullptr;
        else
            node->parent->right=nullptr;
        start = node->parent;
        node = nullptr;
    }

    // Removed node has only right son
    if ((node != nullptr) && (node->left == nullptr) && (node->right != nullptr))
    {
        T* a = node->right;
        if (node->parent == nullptr)
            _root = a;
        Swap(node, a);
        node->right = nullptr;
        start = node;
    }

    // Removed node has only left son
    if ((node != nullptr) && (node->left != nullptr) && (node->right == nullptr))
    {
        T* a = node->left;
        if (node->parent == nullptr)
            _root = a;
        Swap(node, a);
        node->left = nullptr;
        start = node;
    }

    // Removed node has both sons.
    if ((node != nullptr) && (node->left != nullptr) && (node->right != nullptr))
    {
        T* a = node->left;
        while (a->right != nullptr)
            a = a->right;
        T* b = a->left;

        if (node->parent == nullptr)
            _root = a;
        Swap(node, a);

        if (b != nullptr)
        {
            if (a->parent == nullptr)
                _root = b;
            Swap(a, b);
            a->left = nullptr;
            start = a;
        }
        else
        {
            if (a->parent->left == a)
                a->parent->left = nullptr;
            else
                a->parent->right = nullptr;
            start = a->parent;
        }
    }

    // Unlink the removed node
    if (start != nullptr)
        Unlink(start);

    // Correct AVL balanced binary tree root
    if (_root != nullptr)
    {
        while (_root->parent != nullptr)
            _root = _root->parent;
    }

    result->parent = nullptr;
    result->left = nullptr;
    result->right = nullptr;
    --_size;
    return iterator(this, result);
}

template <typename T, typename TCompare>
inline void BinTreeAVL<T, TCompare>::RotateLeft(T* node)
{
    if (node->right == nullptr)
        return;

    T* current = node->right;

    if (node->parent != nullptr)
    {
        if ((node->parent != nullptr) && (node->parent->left == node))
            node->parent->left = current;
        else
            node->parent->right = current;
        current->parent = node->parent;
    }
    else
        current->parent = nullptr;
    node->right = current->left;
    current->left = node;
    node->parent = current;
    if (node->right != nullptr)
        node->right->parent = node;

    if (current->balance == 0)
    {
        node->balance = 1;
        current->balance = -1;
    }
    else
    {
        node->balance = 0;
        current->balance = 0;
    }
}

template <typename T, typename TCompare>
inline void BinTreeAVL<T, TCompare>::RotateRight(T* node)
{
    if (node->left == nullptr)
        return;

    T* current = node->left;

    if (node->parent != nullptr)
    {
        if ((node->parent != nullptr) && (node->parent->left == node))
            node->parent->left = current;
        else
            node->parent->right = current;
        current->parent = node->parent;
    }
    else
        current->parent = nullptr;
    node->left = current->right;
    current->right = node;
    node->parent = current;
    if (node->left != nullptr)
        node->left->parent = node;

    if (current->balance == 0)
    {
        node->balance = -1;
        current->balance = 1;
    }
    else
    {
        node->balance = 0;
        current->balance = 0;
    }
}

template <typename T, typename TCompare>
inline void BinTreeAVL<T, TCompare>::RotateLeftLeft(T* node)
{
    if ((node->left == nullptr) || (node->left->right == nullptr))
        return;

    T* current = node->left;
    T* next = node->left->right;

    if (node->parent != nullptr)
    {
        if ((node->parent != nullptr) && (node->parent->left == node))
            node->parent->left = next;
        else
            node->parent->right = next;
    }
    current->right = next->left;
    node->left = next->right;
    next->left = current;
    next->right = node;
    next->parent = node->parent;
    node->parent = next;
    current->parent = next;
    if (current->right != nullptr)
        current->right->parent = current;
    if (node->left != nullptr)
        node->left->parent = node;

    switch (next->balance)
    {
        case -1:
            node->balance = 1;
            current->balance = 0;
            break;
        case 0:
            node->balance = 0;
            current->balance = 0;
            break;
        case 1:
            node->balance = 0;
            current->balance = -1;
            break;
    }
    next->balance = 0;
}

template <typename T, typename TCompare>
inline void BinTreeAVL<T, TCompare>::RotateRightRight(T* node)
{
    if ((node->right == nullptr) || (node->right->left == nullptr))
        return;

    T* current = node->right;
    T* next = node->right->left;

    if (node->parent != nullptr)
    {
        if ((node->parent != nullptr) && (node->parent->left == node))
            node->parent->left = next;
        else
            node->parent->right = next;
    }
    node->right = next->left;
    current->left = next->right;
    next->left = node;
    next->right = current;
    next->parent = node->parent;
    node->parent = next;
    current->parent = next;
    if (node->right != nullptr)
        node->right->parent = node;
    if (current->left != nullptr)
        current->left->parent = current;

    switch (next->balance)
    {
        case -1:
            node->balance = 0;
            current->balance = 1;
            break;
        case 0:
            node->balance = 0;
            current->balance = 0;
            break;
        case 1:
            node->balance = -1;
            current->balance = 0;
            break;
    }
    next->balance = 0;
}

template <typename T, typename TCompare>
inline void BinTreeAVL<T, TCompare>::Unlink(T* node)
{
    // Rule 1
    if ((node->balance == 0) && (node->left == nullptr))
    {
        node->balance = 1;
        return;
    }
    if ((node->balance == 0) && (node->right == nullptr))
    {
        node->balance = -1;
        return;
    }

    // Rule 2
    if ((node->balance == -1) && (node->left == nullptr))
        node->balance = 0;
    if ((node->balance == 1) && (node->right == nullptr))
        node->balance = 0;

    // Rule 3
    if ((node->balance == -1) && (node->right == nullptr))
    {
        if (node->left->balance == 1)
            RotateLeftLeft(node);
        else
            RotateRight(node);
        node = node->parent;
        if (node->balance == 1)
            return;
    }
    if ((node->balance == 1) && (node->left == nullptr))
    {
        if (node->right->balance == -1)
            RotateRightRight(node);
        else
            RotateLeft(node);
        node = node->parent;
        if (node->balance == -1)
            return;
    }

    while (node->parent != nullptr)
    {
        // Rule 1
        if (((node->parent != nullptr) && (node->parent->left == node)) && (node->parent->balance == 0))
        {
            node->parent->balance = 1;
            break;
        }
        if (((node->parent != nullptr) && (node->parent->right == node)) && (node->parent->balance == 0))
        {
            node->parent->balance = -1;
            break;
        }

        // Rule 2
        if (((node->parent != nullptr) && (node->parent->left == node)) && (node->parent->balance == -1))
        {
            node->parent->balance = 0;
            node = node->parent;
            continue;
        }
        if (((node->parent != nullptr) && (node->parent->right == node)) && (node->parent->balance == 1))
        {
            node->parent->balance = 0;
            node = node->parent;
            continue;
        }

        // Rule 3
        if (((node->parent != nullptr) && (node->parent->right == node)) && (node->parent->balance == -1))
        {
            if (node->parent->left->balance == 1)
                RotateLeftLeft(node->parent);
            else
                RotateRight(node->parent);
            node = node->parent->parent;
            if (node->balance == 1)
                return;
            continue;
        }
        if (((node->parent != nullptr) && (node->parent->left == node)) && (node->parent->balance == 1))
        {
            if (node->parent->right->balance == -1)
                RotateRightRight(node->parent);
            else
                RotateLeft(node->parent);
            node = node->parent->parent;
            if (node->balance == -1)
                return;
            continue;
        }

        // Never happens...
        break;
    }
}

template <typename T, typename TCompare>
inline void BinTreeAVL<T, TCompare>::Swap(T*& node1, T*& node2)
{
    T* first_parent = node1->parent;
    T* first_left = node1->left;
    T* first_right = node1->right;
    T* second_parent = node2->parent;
    T* second_left = node2->left;
    T* second_right = node2->right;
    bool first_is_left = ((first_parent != nullptr) && (first_parent->left == node1));
    bool second_is_left = ((second_parent != nullptr) && (second_parent->left == node2));

    // Update first node links
    if (first_parent != nullptr)
    {
        if (first_is_left)
            first_parent->left = node2;
        else
            first_parent->right = node2;
    }
    if (first_left != nullptr)
        first_left->parent = node2;
    if (first_right != nullptr)
        first_right->parent = node2;

    // Update second node links
    if (second_parent != nullptr)
    {
        if (second_is_left)
            second_parent->left = node1;
        else
            second_parent->right = node1;
    }
    if (second_left != nullptr)
        second_left->parent = node1;
    if (second_right != nullptr)
        second_right->parent = node1;

    // Swap node links
    std::swap(node1->parent, node2->parent);
    std::swap(node1->left, node2->left);
    std::swap(node1->right, node2->right);
    std::swap(node1->balance, node2->balance);

    // Swap nodes
    std::swap(node1, node2);
}

template <typename T, typename TCompare>
inline void BinTreeAVL<T, TCompare>::clear() noexcept
{
    _size = 0;
    _root = nullptr;
}

template <typename T, typename TCompare>
inline void BinTreeAVL<T, TCompare>::swap(BinTreeAVL& bintree) noexcept
{
    using std::swap;
    swap(_compare, bintree._compare);
    swap(_size, bintree._size);
    swap(_root, bintree._root);
}

template <typename T, typename TCompare>
inline void swap(BinTreeAVL<T, TCompare>& bintree1, BinTreeAVL<T, TCompare>& bintree2) noexcept
{
    bintree1.swap(bintree2);
}

} // namespace CppCommon
