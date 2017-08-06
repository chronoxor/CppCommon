/*!
    \file bintree_rb.inl
    \brief Intrusive balanced Red-Black binary tree container inline implementation
    \author Ivan Shynkarenka
    \date 23.06.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T, typename TCompare>
template <class InputIterator>
inline BinTreeRB<T, TCompare>::BinTreeRB(InputIterator first, InputIterator last, const TCompare& compare) noexcept
    : _compare(compare)
{
    for (auto it = first; it != last; ++it)
        insert(*it);
}

template <typename T, typename TCompare>
inline T* BinTreeRB<T, TCompare>::lowest() noexcept
{
    return (T*)InternalLowest();
}

template <typename T, typename TCompare>
inline const T* BinTreeRB<T, TCompare>::lowest() const noexcept
{
    return InternalLowest();
}

template <typename T, typename TCompare>
inline const T* BinTreeRB<T, TCompare>::InternalLowest() const noexcept
{
    const T* result = _root;
    if (result != nullptr)
        while (result->left != nullptr)
            result = result->left;
    return result;
}

template <typename T, typename TCompare>
inline T* BinTreeRB<T, TCompare>::highest() noexcept
{
    return (T*)InternalHighest();
}

template <typename T, typename TCompare>
inline const T* BinTreeRB<T, TCompare>::highest() const noexcept
{
    return InternalHighest();
}

template <typename T, typename TCompare>
inline const T* BinTreeRB<T, TCompare>::InternalHighest() const noexcept
{
    const T* result = _root;
    if (result != nullptr)
        while (result->right != nullptr)
            result = result->right;
    return result;
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::iterator BinTreeRB<T, TCompare>::begin() noexcept
{
    return iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::const_iterator BinTreeRB<T, TCompare>::begin() const noexcept
{
    return const_iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::const_iterator BinTreeRB<T, TCompare>::cbegin() const noexcept
{
    return const_iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::iterator BinTreeRB<T, TCompare>::end() noexcept
{
    return iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::const_iterator BinTreeRB<T, TCompare>::end() const noexcept
{
    return const_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::const_iterator BinTreeRB<T, TCompare>::cend() const noexcept
{
    return const_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::reverse_iterator BinTreeRB<T, TCompare>::rbegin() noexcept
{
    return reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::const_reverse_iterator BinTreeRB<T, TCompare>::rbegin() const noexcept
{
    return const_reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::const_reverse_iterator BinTreeRB<T, TCompare>::crbegin() const noexcept
{
    return const_reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::reverse_iterator BinTreeRB<T, TCompare>::rend() noexcept
{
    return reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::const_reverse_iterator BinTreeRB<T, TCompare>::rend() const noexcept
{
    return const_reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::const_reverse_iterator BinTreeRB<T, TCompare>::crend() const noexcept
{
    return const_reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::iterator BinTreeRB<T, TCompare>::find(const T& item) noexcept
{
    return iterator(this, (T*)InternalFind(item));
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::const_iterator BinTreeRB<T, TCompare>::find(const T& item) const noexcept
{
    return const_iterator(this, InternalFind(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeRB<T, TCompare>::InternalFind(const T& item) const noexcept
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
inline typename BinTreeRB<T, TCompare>::iterator BinTreeRB<T, TCompare>::lower_bound(const T& item) noexcept
{
    return iterator(this, (T*)InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::const_iterator BinTreeRB<T, TCompare>::lower_bound(const T& item) const noexcept
{
    return const_iterator(this, InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeRB<T, TCompare>::InternalLowerBound(const T& item) const noexcept
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
inline typename BinTreeRB<T, TCompare>::iterator BinTreeRB<T, TCompare>::upper_bound(const T& item) noexcept
{
    return iterator(this, (T*)InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::const_iterator BinTreeRB<T, TCompare>::upper_bound(const T& item) const noexcept
{
    return const_iterator(this, InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeRB<T, TCompare>::InternalUpperBound(const T& item) const noexcept
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
inline std::pair<typename BinTreeRB<T, TCompare>::iterator, bool> BinTreeRB<T, TCompare>::insert(T& item) noexcept
{
    return insert(const_iterator(this, _root), item);
}

template <typename T, typename TCompare>
inline std::pair<typename BinTreeRB<T, TCompare>::iterator, bool> BinTreeRB<T, TCompare>::insert(const const_iterator& position, T& item) noexcept
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
    // Set red color for new red-black balanced binary tree node
    node->rb = true;
    // Check red-black properties
    while ((node->parent != nullptr) && node->parent->rb)
    {
        // We have a violation...
        if (node->parent == node->parent->parent->left)
        {
            T* uncle = node->parent->parent->right;
            if ((uncle != nullptr) && uncle->rb)
            {
                // Uncle is red
                node->parent->rb = false;
                uncle->rb = false;
                node->parent->parent->rb = true;
                node = node->parent->parent;
            }
            else
            {
                // Uncle is back
                if (node == node->parent->right)
                {
                    // Make node a left child
                    node = node->parent;
                    RotateLeft(node);
                }

                // Recolor and rotate
                node->parent->rb = false;
                node->parent->parent->rb = true;
                RotateRight(node->parent->parent);
            }
        }
        else
        {
            // Mirror image of above code...
            T* uncle = node->parent->parent->left;
            if ((uncle != nullptr) && uncle->rb)
            {
                // Uncle is red
                node->parent->rb = false;
                uncle->rb = false;
                node->parent->parent->rb = true;
                node = node->parent->parent;
            }
            else
            {
                // Uncle is black
                if (node == node->parent->left)
                {
                    node = node->parent;
                    RotateRight(node);
                }

                // Recolor and rotate
                node->parent->rb = false;
                node->parent->parent->rb = true;
                RotateLeft(node->parent->parent);
            }
        }
    }
    _root->rb = false;

    return std::make_pair(iterator(this, &item), true);
}

template <typename T, typename TCompare>
inline T* BinTreeRB<T, TCompare>::erase(const T& item) noexcept
{
    return erase(find(item)).operator->();
}

template <typename T, typename TCompare>
inline typename BinTreeRB<T, TCompare>::iterator BinTreeRB<T, TCompare>::erase(const iterator& it) noexcept
{
    T* result = ((iterator&)it).operator->();
    if (result == nullptr)
        return end();

    T* x;
    T* y;
    T* node = result;

    if ((node->left == nullptr) || (node->right == nullptr))
    {
        // y has a nullptr node as a child
        y = node;
    }
    else
    {
        // Find tree successor with a nullptr node as a child
        y = node->right;
        while (y->left != nullptr)
            y = y->left;
    }

    // Swap two nodes
    if (y != node)
    {
        if (node->parent == nullptr)
            _root = y;
        Swap(node, y);
    }

    // x is y's only child
    if (y->left != nullptr)
        x = y->left;
    else
        x = y->right;

    // Remove y from the parent chain
    if (x != nullptr)
        x->parent = y->parent;
    if (y->parent != nullptr)
    {
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
    }
    else
        _root = x;

    // Unlink given node
    if (!y->rb)
        Unlink(x, y->parent);

    result->parent = nullptr;
    result->left = nullptr;
    result->right = nullptr;
    --_size;
    return iterator(this, result);
}

template <typename T, typename TCompare>
inline void BinTreeRB<T, TCompare>::RotateLeft(T* node)
{
    T* current = node->right;

    // Establish node->right link
    node->right = current->left;
    if (current->left != nullptr)
        current->left->parent = node;

    // Establish current->parent link
    current->parent = node->parent;
    if (node->parent != nullptr)
    {
        if (node == node->parent->left)
            node->parent->left = current;
        else
            node->parent->right = current;
    }
    else
        _root = current;

    // Link node and current
    current->left = node;
    node->parent = current;
}

template <typename T, typename TCompare>
inline void BinTreeRB<T, TCompare>::RotateRight(T* node)
{
    T* current = node->left;

    // Establish node->right link
    node->left = current->right;
    if (current->right != nullptr)
        current->right->parent = node;

    // Establish current->parent link
    current->parent = node->parent;
    if (node->parent != nullptr)
    {
        if (node == node->parent->right)
            node->parent->right = current;
        else
            node->parent->left = current;
    }
    else
        _root = current;

    // Link node and current
    current->right = node;
    node->parent = current;
}

template <typename T, typename TCompare>
inline void BinTreeRB<T, TCompare>::Unlink(T* node, T* parent)
{
    T* w;

    while ((parent != nullptr) && ((node == nullptr) || !node->rb))
    {
        if (node == parent->left)
        {
            w = parent->right;
            if ((w != nullptr) && w->rb)
            {
                w->rb = false;
                parent->rb = true;
                RotateLeft(parent);
                w = parent->right;
            }
            if (w == nullptr)
                break;
            if (((w->left == nullptr) || !w->left->rb) && ((w->right == nullptr) || !w->right->rb))
            {
                w->rb = true;
                node = parent;
                parent = parent->parent;
            }
            else
            {
                if ((w->right == nullptr) || !w->right->rb)
                {
                    w->left->rb = false;
                    w->rb = true;
                    RotateRight(w);
                    w = parent->right;
                }

                // Copy red-black color information
                if (parent->rb)
                    w->rb = true;
                else
                    w->rb = false;
                parent->rb = false;
                w->right->rb = false;
                RotateLeft(parent);
                node = _root;
                parent = nullptr;
            }
        }
        else
        {
            w = parent->left;
            if ((w != nullptr) && w->rb)
            {
                w->rb = false;
                parent->rb = true;
                RotateRight(parent);
                w = parent->left;
            }
            if (w == nullptr)
                break;
            if (((w->left == nullptr) || !w->left->rb) && ((w->right == nullptr) || !w->right->rb))
            {
                w->rb = true;
                node = parent;
                parent = parent->parent;
            }
            else
            {
                if ((w->left == nullptr) || !w->left->rb)
                {
                    w->right->rb = false;
                    w->rb = true;
                    RotateLeft(w);
                    w = parent->left;
                }

                // Copy red-black color information
                if (parent->rb)
                    w->rb = true;
                else
                    w->rb = false;
                parent->rb = false;
                w->left->rb = false;
                RotateRight(parent);
                node = _root;
                parent = nullptr;
            }
        }
    }

    if (node != nullptr)
        node->rb = false;
}

template <typename T, typename TCompare>
inline void BinTreeRB<T, TCompare>::Swap(T*& node1, T*& node2)
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
inline void BinTreeRB<T, TCompare>::clear() noexcept
{
    _size = 0;
    _root = nullptr;
}

template <typename T, typename TCompare>
inline void BinTreeRB<T, TCompare>::swap(BinTreeRB& bintree) noexcept
{
    using std::swap;
    swap(_compare, bintree._compare);
    swap(_size, bintree._size);
    swap(_root, bintree._root);
}

template <typename T, typename TCompare>
inline void swap(BinTreeRB<T, TCompare>& bintree1, BinTreeRB<T, TCompare>& bintree2) noexcept
{
    bintree1.swap(bintree2);
}

} // namespace CppCommon
