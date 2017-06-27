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
inline BinTreeRB<T, TCompare>::BinTreeRB(InputIterator first, InputIterator last, const TCompare& compare)
    : _compare(compare)
{
    for (InputIterator it = first; it != last; ++it)
        Push(*it);
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
inline BinTreeIterator<T> BinTreeRB<T, TCompare>::begin() noexcept
{
    return BinTreeIterator<T>(lowest());
}

template <typename T, typename TCompare>
inline BinTreeConstIterator<T> BinTreeRB<T, TCompare>::begin() const noexcept
{
    return BinTreeConstIterator<T>(lowest());
}

template <typename T, typename TCompare>
inline BinTreeIterator<T> BinTreeRB<T, TCompare>::end() noexcept
{
    return BinTreeIterator<T>(nullptr);
}

template <typename T, typename TCompare>
inline BinTreeConstIterator<T> BinTreeRB<T, TCompare>::end() const noexcept
{
    return BinTreeConstIterator<T>(nullptr);
}

template <typename T, typename TCompare>
inline BinTreeReverseIterator<T> BinTreeRB<T, TCompare>::rbegin() noexcept
{
    return BinTreeReverseIterator<T>(highest());
}

template <typename T, typename TCompare>
inline BinTreeReverseConstIterator<T> BinTreeRB<T, TCompare>::rbegin() const noexcept
{
    return BinTreeReverseConstIterator<T>(highest());
}

template <typename T, typename TCompare>
inline BinTreeReverseIterator<T> BinTreeRB<T, TCompare>::rend() noexcept
{
    return BinTreeReverseIterator<T>(nullptr);
}

template <typename T, typename TCompare>
inline BinTreeReverseConstIterator<T> BinTreeRB<T, TCompare>::rend() const noexcept
{
    return BinTreeReverseConstIterator<T>(nullptr);
}

template <typename T, typename TCompare>
inline BinTreeIterator<T> BinTreeRB<T, TCompare>::find(const T& item) noexcept
{
    return BinTreeIterator<T>((T*)InternalFind(item));
}

template <typename T, typename TCompare>
inline BinTreeConstIterator<T> BinTreeRB<T, TCompare>::find(const T& item) const noexcept
{
    return BinTreeConstIterator<T>(InternalFind(item));
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
inline BinTreeIterator<T> BinTreeRB<T, TCompare>::lower_bound(const T& item) noexcept
{
    return BinTreeIterator<T>((T*)InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline BinTreeConstIterator<T> BinTreeRB<T, TCompare>::lower_bound(const T& item) const noexcept
{
    return BinTreeConstIterator<T>(InternalLowerBound(item));
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
inline BinTreeIterator<T> BinTreeRB<T, TCompare>::upper_bound(const T& item) noexcept
{
    return BinTreeIterator<T>((T*)InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline BinTreeConstIterator<T> BinTreeRB<T, TCompare>::upper_bound(const T& item) const noexcept
{
    return BinTreeConstIterator<T>(InternalUpperBound(item));
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
inline BinTreeRB<T, TCompare>& BinTreeRB<T, TCompare>::push(T& item) noexcept
{
    // Perform the binary tree insert from the root node
    T* current = _root;

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
        assert("Duplicate node can not be placed into the binary tree!");
        return *this;
    }

    item.parent = current;
    item.left = nullptr;
    item.right = nullptr;
    if (_root == nullptr)
        _root = &item;
    ++_size;
    return *this;
}

template <typename T, typename TCompare>
inline T* BinTreeRB<T, TCompare>::pop(const T& item) noexcept
{
    T* result = (T*)InternalFind(item);
    if (result == nullptr)
        return nullptr;

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
    return result;
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
