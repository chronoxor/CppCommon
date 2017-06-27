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
inline BinTree<T, TCompare>::BinTree(InputIterator first, InputIterator last, const TCompare& compare)
    : _compare(compare)
{
    for (InputIterator it = first; it != last; ++it)
        Push(*it);
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
inline BinTreeIterator<T> BinTree<T, TCompare>::begin() noexcept
{
    return BinTreeIterator<T>(lowest());
}

template <typename T, typename TCompare>
inline BinTreeConstIterator<T> BinTree<T, TCompare>::begin() const noexcept
{
    return BinTreeConstIterator<T>(lowest());
}

template <typename T, typename TCompare>
inline BinTreeIterator<T> BinTree<T, TCompare>::end() noexcept
{
    return BinTreeIterator<T>(nullptr);
}

template <typename T, typename TCompare>
inline BinTreeConstIterator<T> BinTree<T, TCompare>::end() const noexcept
{
    return BinTreeConstIterator<T>(nullptr);
}

template <typename T, typename TCompare>
inline BinTreeReverseIterator<T> BinTree<T, TCompare>::rbegin() noexcept
{
    return BinTreeReverseIterator<T>(highest());
}

template <typename T, typename TCompare>
inline BinTreeReverseConstIterator<T> BinTree<T, TCompare>::rbegin() const noexcept
{
    return BinTreeReverseConstIterator<T>(highest());
}

template <typename T, typename TCompare>
inline BinTreeReverseIterator<T> BinTree<T, TCompare>::rend() noexcept
{
    return BinTreeReverseIterator<T>(nullptr);
}

template <typename T, typename TCompare>
inline BinTreeReverseConstIterator<T> BinTree<T, TCompare>::rend() const noexcept
{
    return BinTreeReverseConstIterator<T>(nullptr);
}

template <typename T, typename TCompare>
inline BinTreeIterator<T> BinTree<T, TCompare>::find(const T& item) noexcept
{
    return BinTreeIterator<T>((T*)InternalFind(item));
}

template <typename T, typename TCompare>
inline BinTreeConstIterator<T> BinTree<T, TCompare>::find(const T& item) const noexcept
{
    return BinTreeConstIterator<T>(InternalFind(item));
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
inline BinTreeIterator<T> BinTree<T, TCompare>::lower_bound(const T& item) noexcept
{
    return BinTreeIterator<T>((T*)InternalLowerBound(item));
}

template <typename T, typename TCompare>
inline BinTreeConstIterator<T> BinTree<T, TCompare>::lower_bound(const T& item) const noexcept
{
    return BinTreeConstIterator<T>(InternalLowerBound(item));
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
inline BinTreeIterator<T> BinTree<T, TCompare>::upper_bound(const T& item) noexcept
{
    return BinTreeIterator<T>((T*)InternalUpperBound(item));
}

template <typename T, typename TCompare>
inline BinTreeConstIterator<T> BinTree<T, TCompare>::upper_bound(const T& item) const noexcept
{
    return BinTreeConstIterator<T>(InternalUpperBound(item));
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
inline BinTree<T, TCompare>& BinTree<T, TCompare>::push(T& item) noexcept
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
inline T* BinTree<T, TCompare>::pop(const T& item) noexcept
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

template <typename T, typename TCompare>
BinTreeIterator<T, TCompare>& BinTreeIterator<T, TCompare>::operator++() noexcept
{
    if (_current != nullptr)
    {
        if (_current->right != nullptr)
        {
            _current = _current->right;
            while (_current->left != nullptr)
                _current = _current->left;
        }
        else
        {
            while ((_current->parent != nullptr) && compare(*_current->parent, *_current))
                _current = _current->parent;
            _current = _current->parent;
        }
    }
    return *this;
}

template <typename T, typename TCompare>
inline BinTreeIterator<T, TCompare> BinTreeIterator<T, TCompare>::operator++(int) noexcept
{
    BinTreeIterator<T, TCompare> result(*this);
    operator++();
    return result;
}

template <typename T, typename TCompare>
T& BinTreeIterator<T, TCompare>::operator*() noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename T, typename TCompare>
T* BinTreeIterator<T, TCompare>::operator->() noexcept
{
    return _current;
}

template <typename T, typename TCompare>
void BinTreeIterator<T, TCompare>::swap(BinTreeIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename T, typename TCompare>
void swap(BinTreeIterator<T, TCompare>& it1, BinTreeIterator<T, TCompare>& it2) noexcept
{
    it1.swap(it2);
}

template <typename T, typename TCompare>
BinTreeConstIterator<T, TCompare>& BinTreeConstIterator<T, TCompare>::operator++() noexcept
{
    if (_current != nullptr)
    {
        if (_current->right != nullptr)
        {
            _current = _current->right;
            while (_current->left != nullptr)
                _current = _current->left;
        }
        else
        {
            while ((_current->parent != nullptr) && compare(*_current->parent, *_current))
                _current = _current->parent;
            _current = _current->parent;
        }
    }
    return *this;
}

template <typename T, typename TCompare>
inline BinTreeConstIterator<T, TCompare> BinTreeConstIterator<T, TCompare>::operator++(int) noexcept
{
    BinTreeConstIterator<T, TCompare> result(*this);
    operator++();
    return result;
}

template <typename T, typename TCompare>
const T& BinTreeConstIterator<T, TCompare>::operator*() const noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename T, typename TCompare>
const T* BinTreeConstIterator<T, TCompare>::operator->() const noexcept
{
    return _current;
}

template <typename T, typename TCompare>
void BinTreeConstIterator<T, TCompare>::swap(BinTreeConstIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename T, typename TCompare>
void swap(BinTreeConstIterator<T, TCompare>& it1, BinTreeConstIterator<T, TCompare>& it2) noexcept
{
    it1.swap(it2);
}

template <typename T, typename TCompare>
BinTreeReverseIterator<T, TCompare>& BinTreeReverseIterator<T, TCompare>::operator++() noexcept
{
    if (_current != nullptr)
    {
        if (_current->left != nullptr)
        {
            _current = _current->left;
            while (_current->right != nullptr)
                _current = _current->right;
        }
        else
        {
            while ((_current->parent != nullptr) && compare(*_current, *_current->parent))
                _current = _current->parent;
            _current = _current->parent;
        }
    }
    return *this;
}

template <typename T, typename TCompare>
inline BinTreeReverseIterator<T, TCompare> BinTreeReverseIterator<T, TCompare>::operator++(int) noexcept
{
    BinTreeReverseIterator<T, TCompare> result(*this);
    operator++();
    return result;
}

template <typename T, typename TCompare>
T& BinTreeReverseIterator<T, TCompare>::operator*() noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename T, typename TCompare>
T* BinTreeReverseIterator<T, TCompare>::operator->() noexcept
{
    return _current;
}

template <typename T, typename TCompare>
void BinTreeReverseIterator<T, TCompare>::swap(BinTreeReverseIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename T, typename TCompare>
void swap(BinTreeReverseIterator<T, TCompare>& it1, BinTreeReverseIterator<T, TCompare>& it2) noexcept
{
    it1.swap(it2);
}

template <typename T, typename TCompare>
BinTreeReverseConstIterator<T, TCompare>& BinTreeReverseConstIterator<T, TCompare>::operator++() noexcept
{
    if (_current != nullptr)
    {
        if (_current->left != nullptr)
        {
            _current = _current->left;
            while (_current->right != nullptr)
                _current = _current->right;
        }
        else
        {
            while ((_current->parent != nullptr) && compare(*_current, *_current->parent))
                _current = _current->parent;
            _current = _current->parent;
        }
    }
    return *this;
}

template <typename T, typename TCompare>
inline BinTreeReverseConstIterator<T, TCompare> BinTreeReverseConstIterator<T, TCompare>::operator++(int) noexcept
{
    BinTreeReverseConstIterator<T, TCompare> result(*this);
    operator++();
    return result;
}

template <typename T, typename TCompare>
const T& BinTreeReverseConstIterator<T, TCompare>::operator*() const noexcept
{
    assert((_current != nullptr) && "Iterator must be valid!");

    return *_current;
}

template <typename T, typename TCompare>
const T* BinTreeReverseConstIterator<T, TCompare>::operator->() const noexcept
{
    return _current;
}

template <typename T, typename TCompare>
void BinTreeReverseConstIterator<T, TCompare>::swap(BinTreeReverseConstIterator& it) noexcept
{
    using std::swap;
    swap(_current, it._current);
}

template <typename T, typename TCompare>
void swap(BinTreeReverseConstIterator<T, TCompare>& it1, BinTreeReverseConstIterator<T, TCompare>& it2) noexcept
{
    it1.swap(it2);
}

} // namespace CppCommon
