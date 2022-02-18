/*!
    \file bintree_avl.h
    \brief Intrusive balanced AVL binary tree container definition
    \author Ivan Shynkarenka
    \date 23.06.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_CONTAINERS_BINTREE_AVL_H
#define CPPCOMMON_CONTAINERS_BINTREE_AVL_H

#include "bintree.h"

namespace CppCommon {

//! Intrusive balanced AVL binary tree container
/*!
    Not thread-safe.

    <b>Overview</b>\n
    In computer science, an AVL tree is a self-balancing binary  search  tree,
    and the first such data structure to be  invented.  In  an  AVL  tree  the
    heights of the two child subtrees of any  node  differ  by  at  most  one,
    therefore it  is  also  called  height-balanced.  Lookup,  insertion,  and
    deletion all take O(log n) time in  both  the  average  and  worst  cases.
    Additions and deletions may require the tree to be rebalanced  by  one  or
    more tree rotations.

    The AVL tree is named after its two  inventors,  G.M.  Adelson-Velsky  and
    E.M. Landis, who published it in their 1962 paper "An  algorithm  for  the
    organization of information."

    The balance factor of a node is the height of its right subtree minus  the
    height of its left subtree. A node with balance factor  1,  0,  or  -1  is
    considered balanced. A node with any other balance  factor  is  considered
    unbalanced and requires rebalancing the tree. The balance factor is either
    stored directly at each node or computed from the heights of the subtrees.

    While AVL trees are theoretically  quite  sound,  they  are  not  commonly
    implemented due  to  their  high  implementation  complexity  to  keep  it
    balanced,  making  development  less  effective  when  compared  to  self-
    correcting tree structures,  such  as  splay  trees  or  heaps.  They  do,
    however, perform better than e.g. red-black trees. They are widely used in
    academic settings as an instructional data structure.

    <b>Operations</b>\n
    The basic operations of an AVL tree generally  involve  carrying  out  the
    same algorithms as would be carried out on  an  unbalanced  binary  search
    tree, but preceded or followed by  one  or  more  of  the  so-called  "AVL
    rotations."

    <b>Insertion</b>\n
    Insertion into an AVL tree may be carried out by inserting the given value
    into the tree as if it were an unbalanced binary  search  tree,  and  then
    retracing one's steps toward the root, rotating about any nodes which have
    become unbalanced during the insertion (see tree rotation). Since at  most
    1.5 times lg n nodes are retraced on the journey back  to  the  root,  and
    each AVL rotation takes constant time,  the  insertion  process  in  total
    takes O(log n) time.

    <b>Deletion</b>\n
    Deletion from an AVL tree may be carried out by rotating the  node  to  be
    deleted down into a leaf  node,  and  then  pruning  off  that  leaf  node
    directly. Since at most log n nodes are rotated during the  rotation  into
    the leaf, and each AVL rotation takes constant time, the deletion  process
    in total takes O(log n) time.

    Practically, this is a large overhead and complex to  program.  Therefore,
    it's more common to implement a lazy deletion -- leave the deletion target
    in place, flag it as "deleted", and replace it with an  inserted  node  if
    they would occupy the same spot.

    <b>Lookup</b>\n
    Lookup in an AVL tree is performed exactly  as  in  an  unbalanced  binary
    search tree, and thus takes O(log n) time, since an  AVL  tree  is  always
    kept balanced. No special provisions need to  be  taken,  and  the  tree's
    structure is not modified by lookups. (This is in contrast to  splay  tree
    lookups, which do modify their tree's structure.)

    <b>Usage</b>\n
    AVL trees are  faster than Red-Black  trees when lookups are more frequent
    than inserts/deletes and comparisons are expensive.

    <b>References</b>\n
    \li G. Adelson-Velskii and E.M. Landis, "An algorithm for the organization
        of  information."  Doklady  Akademii  Nauk  SSSR,  146:263-266,   1962
        (Russian). English translation by  Myron  J.  Ricci  in  Soviet  Math.
        Doklady, 3:1259-1263, 1962.
    \li Donald Knuth. The Art of Computer Programming, Volume 3:  Sorting  and
        Searching, Third Edition. Addison-Wesley,  1997.  ISBN  0-201-89685-0.
        Pages 458-475 of section 6.2.3: Balanced Trees. Note that Knuth  calls
        AVL trees simply "balanced trees".

    <b>Taken from:</b>\n
    AVL tree from Wikipedia, the free encyclopedia
    http://en.wikipedia.org/wiki/AVL_tree
*/
template <typename T, typename TCompare = std::less<T>>
class BinTreeAVL
{
public:
    // Standard container type definitions
    typedef T value_type;
    typedef TCompare value_compare;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef BinTreeIterator<BinTreeAVL<T, TCompare>, T> iterator;
    typedef BinTreeConstIterator<BinTreeAVL<T, TCompare>, T> const_iterator;
    typedef BinTreeReverseIterator<BinTreeAVL<T, TCompare>, T> reverse_iterator;
    typedef BinTreeConstReverseIterator<BinTreeAVL<T, TCompare>, T> const_reverse_iterator;

    //! AVL binary tree node
    struct Node
    {
        T* parent;              //!< Pointer to the parent binary tree node
        T* left;                //!< Pointer to the left child binary tree node
        T* right;               //!< Pointer to the right child binary tree node
        signed char balance;    //!< Balance level (-1, 0, 1)

        Node() : parent(nullptr), left(nullptr), right(nullptr), balance(0) {}
    };

    explicit BinTreeAVL(const TCompare& compare = TCompare()) noexcept
        : _compare(compare),
          _size(0),
          _root(nullptr)
    {}
    template <class InputIterator>
    BinTreeAVL(InputIterator first, InputIterator last, const TCompare& compare = TCompare()) noexcept;
    BinTreeAVL(const BinTreeAVL&) noexcept = default;
    BinTreeAVL(BinTreeAVL&&) noexcept = default;
    ~BinTreeAVL() noexcept = default;

    BinTreeAVL& operator=(const BinTreeAVL&) noexcept = default;
    BinTreeAVL& operator=(BinTreeAVL&&) noexcept = default;

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
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    //! Get the end binary tree iterator
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    //! Get the reverse begin binary tree iterator
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    //! Get the reverse end binary tree iterator
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    //! Find the iterator which points to the first equal item in the binary tree or return end iterator
    iterator find(const T& item) noexcept;
    const_iterator find(const T& item) const noexcept;

    //! Find the iterator which points to the first item that not less than the given item in the binary tree or return end iterator
    iterator lower_bound(const T& item) noexcept;
    const_iterator lower_bound(const T& item) const noexcept;
    //! Find the iterator which points to the first item that greater than the given item in the binary tree or return end iterator
    iterator upper_bound(const T& item) noexcept;
    const_iterator upper_bound(const T& item) const noexcept;

    //! Insert a new item into the binary tree
    /*!
        \param item - Item to insert
        \return Pair with the iterator to the inserted item and success flag
    */
    std::pair<iterator, bool> insert(T& item) noexcept;
    //! Insert a new item into the binary tree with a position hint
    /*!
        \param position - Iterator position to the inserted item
        \param item - Item to insert
        \return Pair with the iterator to the inserted item and success flag
    */
    std::pair<iterator, bool> insert(const const_iterator& position, T& item) noexcept;

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
    iterator erase(const iterator& it) noexcept;

    //! Clear the binary tree
    void clear() noexcept;

    //! Swap two instances
    void swap(BinTreeAVL& bintree) noexcept;
    template <typename U, typename UCompare>
    friend void swap(BinTreeAVL<U, UCompare>& bintree1, BinTreeAVL<U, UCompare>& bintree2) noexcept;

private:
    TCompare _compare;  // Binary tree compare
    size_t _size;       // Binary tree size
    T* _root;           // Binary tree root node

    const T* InternalLowest() const noexcept;
    const T* InternalHighest() const noexcept;
    const T* InternalFind(const T& item) const noexcept;
    const T* InternalLowerBound(const T& item) const noexcept;
    const T* InternalUpperBound(const T& item) const noexcept;

    static void RotateLeft(T* node);
    static void RotateRight(T* node);
    static void RotateLeftLeft(T* node);
    static void RotateRightRight(T* node);
    static void Unlink(T* node);
    static void Swap(T*& node1, T*& node2);
};

} // namespace CppCommon

#include "bintree_avl.inl"

#endif // CPPCOMMON_CONTAINERS_BINTREE_AVL_H
