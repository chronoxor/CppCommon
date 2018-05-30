/*!
    \file bintree_splay.inl
    \brief Intrusive balanced Splay binary tree container inline implementation
    \author Ivan Shynkarenka
    \date 23.06.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T, typename TCompare>
template <class InputIterator>
inline BinTreeSplay<T, TCompare>::BinTreeSplay(InputIterator first, InputIterator last, const TCompare& compare) noexcept
    : _compare(compare)
{
    for (auto it = first; it != last; ++it)
        insert(*it);
}

template <typename T, typename TCompare>
inline T* BinTreeSplay<T, TCompare>::lowest() noexcept
{
    return (T*)InternalLowest();
}

template <typename T, typename TCompare>
inline const T* BinTreeSplay<T, TCompare>::lowest() const noexcept
{
    return InternalLowest();
}

template <typename T, typename TCompare>
inline const T* BinTreeSplay<T, TCompare>::InternalLowest() const noexcept
{
    const T* result = _root;
    if (result != nullptr)
        while (result->left != nullptr)
            result = result->left;
    return result;
}

template <typename T, typename TCompare>
inline T* BinTreeSplay<T, TCompare>::highest() noexcept
{
    return (T*)InternalHighest();
}

template <typename T, typename TCompare>
inline const T* BinTreeSplay<T, TCompare>::highest() const noexcept
{
    return InternalHighest();
}

template <typename T, typename TCompare>
inline const T* BinTreeSplay<T, TCompare>::InternalHighest() const noexcept
{
    const T* result = _root;
    if (result != nullptr)
        while (result->right != nullptr)
            result = result->right;
    return result;
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::iterator BinTreeSplay<T, TCompare>::begin() noexcept
{
    return iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::const_iterator BinTreeSplay<T, TCompare>::begin() const noexcept
{
    return const_iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::const_iterator BinTreeSplay<T, TCompare>::cbegin() const noexcept
{
    return const_iterator(this, lowest());
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::iterator BinTreeSplay<T, TCompare>::end() noexcept
{
    return iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::const_iterator BinTreeSplay<T, TCompare>::end() const noexcept
{
    return const_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::const_iterator BinTreeSplay<T, TCompare>::cend() const noexcept
{
    return const_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::reverse_iterator BinTreeSplay<T, TCompare>::rbegin() noexcept
{
    return reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::const_reverse_iterator BinTreeSplay<T, TCompare>::rbegin() const noexcept
{
    return const_reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::const_reverse_iterator BinTreeSplay<T, TCompare>::crbegin() const noexcept
{
    return const_reverse_iterator(this, highest());
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::reverse_iterator BinTreeSplay<T, TCompare>::rend() noexcept
{
    return reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::const_reverse_iterator BinTreeSplay<T, TCompare>::rend() const noexcept
{
    return const_reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::const_reverse_iterator BinTreeSplay<T, TCompare>::crend() const noexcept
{
    return const_reverse_iterator(this, nullptr);
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::iterator BinTreeSplay<T, TCompare>::find(const T& item) noexcept
{
    return iterator(this, (T*)InternalFind(item));
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::const_iterator BinTreeSplay<T, TCompare>::find(const T& item) const noexcept
{
    return const_iterator(this, InternalFind(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeSplay<T, TCompare>::InternalFind(const T& item) const noexcept
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
            previous = current;
            current = current->right;
            continue;
        }

        // Balance the binary tree
        if (current != nullptr)
            Splay((T*)current);

        // Found result node
        return current;
    }

    // Balance the binary tree
    if (previous != nullptr)
        Splay((T*)previous);

    // Nothing was found...
    return nullptr;
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::iterator BinTreeSplay<T, TCompare>::lower_bound(const T& item) noexcept
{
    return iterator(this, (T*)InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::const_iterator BinTreeSplay<T, TCompare>::lower_bound(const T& item) const noexcept
{
    return const_iterator(this, InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeSplay<T, TCompare>::InternalLowerBound(const T& item) const noexcept
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

        // Balance the binary tree
        if (current != nullptr)
            Splay((T*)current);

        // Found result node
        return current;
    }

    // Balance the binary tree
    if (previous != nullptr)
        Splay((T*)previous);

    // Return the previous lower bound node if any met
    return previous;
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::iterator BinTreeSplay<T, TCompare>::upper_bound(const T& item) noexcept
{
    return iterator(this, (T*)InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::const_iterator BinTreeSplay<T, TCompare>::upper_bound(const T& item) const noexcept
{
    return const_iterator(this, InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline const T* BinTreeSplay<T, TCompare>::InternalUpperBound(const T& item) const noexcept
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

    // Balance the binary tree
    if (previous != nullptr)
        Splay((T*)previous);

    // Return the previous upper bound node if any met
    return previous;
}

template <typename T, typename TCompare>
inline std::pair<typename BinTreeSplay<T, TCompare>::iterator, bool> BinTreeSplay<T, TCompare>::insert(T& item) noexcept
{
    return insert(find(item), item);
}

template <typename T, typename TCompare>
inline std::pair<typename BinTreeSplay<T, TCompare>::iterator, bool> BinTreeSplay<T, TCompare>::insert(const const_iterator& position, T& item) noexcept
{
    // Perform the binary tree insert from the given node
    T* current = (T*)position.operator->();
    if (current != nullptr)
    {
        // Found duplicate node
        return std::make_pair(iterator(this, current), false);
    }

    item.parent = nullptr;
    item.left = nullptr;
    item.right = nullptr;
    if (_root != nullptr)
    {
        if (compare(item, *_root))
        {
            item.left = _root->left;
            item.right = _root;
            _root->parent = &item;
            if (_root->left != nullptr)
                _root->left->parent = &item;
            _root->left = nullptr;
        }
        else
        {
            item.right = _root->right;
            item.left = _root;
            _root->parent = &item;
            if (_root->right != nullptr)
                _root->right->parent = &item;
            _root->right = nullptr;
        }
    }
    _root = &item;
    ++_size;

    return std::make_pair(iterator(this, &item), true);
}

template <typename T, typename TCompare>
inline T* BinTreeSplay<T, TCompare>::erase(const T& item) noexcept
{
    return erase(find(item)).operator->();
}

template <typename T, typename TCompare>
inline typename BinTreeSplay<T, TCompare>::iterator BinTreeSplay<T, TCompare>::erase(const iterator& it) noexcept
{
    T* result = ((iterator&)it).operator->();
    if (result == nullptr)
        return end();

    if (result->left == nullptr)
    {
        _root = result->right;
        if (_root != nullptr)
            _root->parent = nullptr;
    }
    else if (result->right == nullptr)
    {
        _root = result->left;
        if (_root != nullptr)
            _root->parent = nullptr;
    }
    else
    {
        _root = result->left;
        _root->parent = nullptr;
        T* highest = result->left;
        while (highest->right != nullptr)
            highest = highest->right;
        highest->right = result->right;
        if (highest->right != nullptr)
            highest->right->parent = highest;
    }

    result->parent = nullptr;
    result->left = nullptr;
    result->right = nullptr;
    --_size;
    return iterator(this, result);
}

template <typename T, typename TCompare>
inline void BinTreeSplay<T, TCompare>::Splay(T* x) const
{
    while (x->parent != nullptr)
    {
        T* p = x->parent;
        T* g = p->parent;
        if (g == nullptr)
            Zig(x);
        else if ((g->left == p) && (p->left == x))
            ZigZig(x);
        else if ((g->right == p) && (p->right == x))
            ZigZig(x);
        else
            ZigZag(x);
    }
    (T*&)_root = x;
}

template <typename T, typename TCompare>
inline void BinTreeSplay<T, TCompare>::Zig(T* x) const
{
    T* p = x->parent;
    if (p->left == x)
    {
        [[maybe_unused]] T* a = x->left;
        [[maybe_unused]] T* b = x->right;
        [[maybe_unused]] T* c = p->right;

        x->parent = nullptr;
        x->right = p;

        p->parent = x;
        p->left = b;

        if (b != nullptr)
            b->parent = p;
    }
    else
    {
        [[maybe_unused]] T* a = p->left;
        [[maybe_unused]] T* b = x->left;
        [[maybe_unused]] T* c = x->right;

        x->parent = nullptr;
        x->left = p;

        p->parent = x;
        p->right = b;

        if (b != nullptr)
            b->parent = p;
    }
}

template <typename T, typename TCompare>
inline void BinTreeSplay<T, TCompare>::ZigZig(T* x) const
{
    T* p = x->parent;
    T* g = p->parent;
    if (p->left == x)
    {
        [[maybe_unused]] T* a = x->left;
        [[maybe_unused]] T* b = x->right;
        [[maybe_unused]] T* c = p->right;
        [[maybe_unused]] T* d = g->right;

        x->parent = g->parent;
        x->right = p;

        p->parent = x;
        p->left = b;
        p->right = g;

        g->parent = p;
        g->left = c;

        if (x->parent != nullptr)
        {
            if (x->parent->left == g)
                x->parent->left = x;
            else
                x->parent->right = x;
        }

        if (b != nullptr)
            b->parent = p;

        if (c != nullptr)
            c->parent = g;
    }
    else
    {
        [[maybe_unused]] T* a = g->left;
        [[maybe_unused]] T* b = p->left;
        [[maybe_unused]] T* c = x->left;
        [[maybe_unused]] T* d = x->right;

        x->parent = g->parent;
        x->left = p;

        p->parent = x;
        p->left = g;
        p->right = c;

        g->parent = p;
        g->right = b;

        if (x->parent != nullptr)
        {
            if (x->parent->left == g)
                x->parent->left = x;
            else
                x->parent->right = x;
        }

        if (b != nullptr)
            b->parent = g;

        if (c != nullptr)
            c->parent = p;
    }
}

template <typename T, typename TCompare>
inline void BinTreeSplay<T, TCompare>::ZigZag(T* x) const
{
    T* p = x->parent;
    T* g = p->parent;
    if (p->right == x)
    {
        [[maybe_unused]] T* a = p->left;
        [[maybe_unused]] T* b = x->left;
        [[maybe_unused]] T* c = x->right;
        [[maybe_unused]] T* d = g->right;

        x->parent = g->parent;
        x->left = p;
        x->right = g;

        p->parent = x;
        p->right = b;

        g->parent = x;
        g->left = c;

        if (x->parent != nullptr)
        {
            if (x->parent->left == g)
                x->parent->left = x;
            else
                x->parent->right = x;
        }

        if (b != nullptr)
            b->parent = p;

        if (c != nullptr)
            c->parent = g;
    }
    else
    {
        [[maybe_unused]] T* a = g->left;
        [[maybe_unused]] T* b = x->left;
        [[maybe_unused]] T* c = x->right;
        [[maybe_unused]] T* d = p->right;

        x->parent = g->parent;
        x->left = g;
        x->right = p;

        p->parent = x;
        p->left = c;

        g->parent = x;
        g->right = b;

        if (x->parent != nullptr)
        {
            if (x->parent->left == g)
                x->parent->left = x;
            else
                x->parent->right = x;
        }

        if (b != nullptr)
            b->parent = g;

        if (c != nullptr)
            c->parent = p;
    }
}

template <typename T, typename TCompare>
inline void BinTreeSplay<T, TCompare>::clear() noexcept
{
    _size = 0;
    _root = nullptr;
}

template <typename T, typename TCompare>
inline void BinTreeSplay<T, TCompare>::swap(BinTreeSplay& bintree) noexcept
{
    using std::swap;
    swap(_compare, bintree._compare);
    swap(_size, bintree._size);
    swap(_root, bintree._root);
}

template <typename T, typename TCompare>
inline void swap(BinTreeSplay<T, TCompare>& bintree1, BinTreeSplay<T, TCompare>& bintree2) noexcept
{
    bintree1.swap(bintree2);
}

} // namespace CppCommon
