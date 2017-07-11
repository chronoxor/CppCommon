/*!
    \file bintree_aa.h
    \brief Intrusive balanced A.Andersson binary tree container definition
    \author Ivan Shynkarenka
    \date 23.06.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_CONTAINERS_BINTREE_AA_H
#define CPPCOMMON_CONTAINERS_BINTREE_AA_H

#include <cassert>
#include <iterator>

namespace CppCommon {

//! Intrusive balanced A.Andersson binary tree container
/*!
    <b>Overview</b>\n
    Andersson trees are simple and easy to implement  balanced  binary  search
    trees that are based on the foundations of red black trees.  Consequently,
    Andersson trees have similar performance and structuring properties as red
    black trees without the difficult implementation. Red black trees  are  an
    abstraction of the symmetric binary B-tree, which is a clever  abstraction
    of a B-tree of order 4.  Andersson  trees  are  a  simplification  of  the
    symmetric binary B-tree that use a B-tree of order  3  to  remove  several
    unpleasant cases in the implementation. If the last  two  sentences  meant
    absolutely nothing to you, that's okay. This background isn't necessary to
    understand Andersson trees or implement them well.  Andersson  trees  were
    introduced by Arne Andersson in his  paper  "Balanced  Search  Trees  Made
    Simple". They were further studied by a  few  people,  most  notably  Mark
    Allen Weiss, who discussed them briefly in his books  on  data  structures
    and algorithm analysis.

    Andersson trees are a very simple  alternative  to  the  more  traditional
    balanced binary search trees. The performance properties are very close to
    that of red black trees, and the effort required in implementing  them  is
    minimal for anyone who is comfortable writing basic binary search trees.

    <b>Performance</b>\n
    The performance of an AA tree is equivalent to the performance of  a  red-
    black tree. While an AA tree makes more rotations than a  red-black  tree,
    the simpler algorithms tend to be faster, and all of this balances out  to
    result in similar performance. A red-black tree is more consistent in  its
    performance than an AA tree, but an AA tree tends  to  be  flatter,  which
    results in slightly faster search times.

    <b>Usage</b>\n
    When comparisons are expensive but lookups are more frequent than updates,
    the  AA  tree  might win.  AA  tree  tends  to  be flatter,  which results
    in  slightly faster search times.

    <b>Taken from:</b>\n
    AA tree from Wikipedia, the free encyclopedia
    http://en.wikipedia.org/wiki/AA_tree
*/
template <typename T, typename TCompare = std::less<T>>
class BinTreeAA
{
public:
    //! A.Andersson binary tree node
    struct Node
    {
        T* parent;      //!< Pointer to the parent binary tree node
        T* left;        //!< Pointer to the left child binary tree node
        T* right;       //!< Pointer to the right child binary tree node
        size_t level;   //!< Balance level
    };

    explicit BinTreeAA(const TCompare& compare = TCompare()) noexcept
        : _compare(compare),
          _size(0),
          _root(nullptr)
    {}
    template <class InputIterator>
    BinTreeAA(InputIterator first, InputIterator last, const TCompare& compare = TCompare());
    BinTreeAA(const BinTreeAA&) noexcept = delete;
    BinTreeAA(BinTreeAA&&) noexcept = default;
    ~BinTreeAA() noexcept = default;

    BinTreeAA& operator=(const BinTreeAA&) noexcept = delete;
    BinTreeAA& operator=(BinTreeAA&&) noexcept = default;

    //! Check if the binary tree is not empty
    explicit operator bool() const noexcept { return !empty(); }

    //! Is the binary tree empty?
    bool empty() const noexcept { return _root == nullptr; }

    //! Get the binary tree size
    size_t size() const noexcept { return _size; }

    //! Get the root binary tree item
    T* root() noexcept { return _root; }
    const T* root() const noexcept { return _root; }
    //! Get the lowest binary tree item
    T* lowest() noexcept;
    const T* lowest() const noexcept;
    //! Get the highest binary tree item
    T* highest() noexcept;
    const T* highest() const noexcept;

    //! Compare two items: if the first item is less than the second one?
    bool compare(const T& item1, const T& item2) const noexcept { return _compare(item1, item2); }

    //! Get the begin binary tree iterator
    BinTreeIterator<T> begin() noexcept;
    BinTreeConstIterator<T> begin() const noexcept;
    //! Get the end binary tree iterator
    BinTreeIterator<T> end() noexcept;
    BinTreeConstIterator<T> end() const noexcept;

    //! Get the reverse begin binary tree iterator
    BinTreeReverseIterator<T> rbegin() noexcept;
    BinTreeReverseConstIterator<T> rbegin() const noexcept;
    //! Get the reverse end binary tree iterator
    BinTreeReverseIterator<T> rend() noexcept;
    BinTreeReverseConstIterator<T> rend() const noexcept;

    //! Find the iterator which points to the first equal item in the binary tree or return end iterator
    BinTreeIterator<T> find(const T& item) noexcept;
    BinTreeConstIterator<T> find(const T& item) const noexcept;

    //! Find the iterator which points to the first item not less than the given item in the binary tree or return end iterator
    BinTreeIterator<T> lower_bound(const T& item) noexcept;
    BinTreeConstIterator<T> lower_bound(const T& item) const noexcept;
    //! Find the iterator which points to the first item that greater than the given item in the binary tree or return end iterator
    BinTreeIterator<T> upper_bound(const T& item) noexcept;
    BinTreeConstIterator<T> upper_bound(const T& item) const noexcept;

    //! Insert a new item into the binary tree
    /*!
        \param item - Item to insert
        \return The current binary tree collection
    */
    BinTreeAA& insert(T& item) noexcept;

    //! Erase the given item from the binary tree
    /*!
        \param item - Item to erase
        \return Erased item
    */
    T* erase(const T& item) noexcept;
    //! Erase the given item from the binary tree
    /*!
        \param it - Iterator to the erased item
        \return Erased item iterator
    */
    BinTreeIterator<T> erase(const BinTreeIterator<T>& it) noexcept;

    //! Swap two instances
    void swap(BinTreeAA& bintree) noexcept;
    template <typename U>
    friend void swap(BinTreeAA<U>& bintree1, BinTreeAA<U>& bintree2) noexcept;

private:
    TCompare _compare;  // Binary tree compare
    size_t _size;       // Binary tree size
    T* _root;           // Binary tree root node

    const T* InternalLowest() const noexcept;
    const T* InternalHighest() const noexcept;
    const T* InternalFind(const T& item) const noexcept;
    const T* InternalLowerBound(const T& item) const noexcept;
    const T* InternalUpperBound(const T& item) const noexcept;

    //! Skew the binary tree node
    /*!
        Skew is a right rotation when an insertion or deletion creates a left
        red link.

        \param node - Skew node
    */
    void Skew(T* node);
    //! Split the binary tree node.
    /*!
        Split is a conditional left rotation when an insertion or deletion
        creates two consecutive red links.

        \param node - Split node
        \return 'true' if the split operation completed successfully, 'false' if the split operation failed
    */
    bool Split(T* node);
};

} // namespace CppCommon

#include "bintree_aa.inl"

#endif // CPPCOMMON_CONTAINERS_BINTREE_AA_H
