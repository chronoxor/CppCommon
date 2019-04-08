/*!
    \file bintree_rb.h
    \brief Intrusive balanced Red-Black binary tree container definition
    \author Ivan Shynkarenka
    \date 23.06.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_CONTAINERS_BINTREE_RB_H
#define CPPCOMMON_CONTAINERS_BINTREE_RB_H

#include "bintree.h"

namespace CppCommon {

//! Intrusive balanced Red-Black binary tree container
/*!
    Not thread-safe.

    <b>Overview</b>\n
    A red-black tree is a type of self-balancing binary search  tree,  a  data
    structure  used  in  computer  science,  typically   used   to   implement
    associative arrays. The original structure was invented in 1972 by  Rudolf
    Bayer who called them "symmetric binary B-trees", but acquired its  modern
    name in a paper in 1978 by Leo J.  Guibas  and  Robert  Sedgewick.  It  is
    complex, but has good worst-case running time for its  operations  and  is
    efficient in practice: it can search, insert, and delete in O(log n) time,
    where n is the number of elements in the tree.

    <b>Background and terminology</b>\n
    A red-black tree is a special type of binary tree, which  is  a  structure
    used in computer science to organize pieces of comparable  data,  such  as
    numbers. Each piece of data is stored in a node. One of the  nodes  always
    functions as our starting place, and is not the child of any node; we call
    this the root node or root. It has up to two "children",  other  nodes  to
    which it connects. Each of these children can have up to two  children  of
    its own, and so on. The root node thus has a path  connecting  it  to  any
    other node in the tree.

    If a node has no children, we call it a leaf node, since intuitively it is
    at the periphery of the tree. A subtree is the portion of  the  tree  that
    can be reached from a certain  node,  considered  as  a  tree  itself.  In
    red-black trees, the leaves are assumed to be null, that is, they  do  not
    contain any data.

    As red-black  trees  are  also  binary  search  trees,  they  satisfy  the
    constraint that every node contains a value less  than  or  equal  to  all
    nodes in its right subtree, and greater than or equal to all nodes in  its
    left subtree. This makes it quick to search the tree for  a  given  value,
    and allows efficient in-order traversal of elements.

    <b>Uses and advantages.</b>\n
    Red-black trees, along with AVL trees, offer the best possible  worst-case
    guarantees for insertion time, deletion time, and search  time.  Not  only
    does this make  them  valuable  in  time-sensitive  applications  such  as
    real-time applications, but it makes  them  valuable  building  blocks  in
    other data structures which provide worst-case  guarantees;  for  example,
    many data structures used  in  computational  geometry  can  be  based  on
    red-black trees.

    Red-black trees are also particularly valuable in functional  programming,
    where they are one of the most common persistent data structures, used  to
    construct associative arrays and sets which can retain  previous  versions
    after mutations. The persistent version of red-black trees requires  O(log
    n) space for each insertion or deletion, in addition to time.

    Red-black trees are an isometry of 2-3-4 trees. In other words, for  every
    2-3-4 tree, there exists at least one red-black tree with data elements in
    the same order. The insertion and deletion operations on 2-3-4  trees  are
    also equivalent to color-flipping and rotations in red-black  trees.  This
    makes 2-3-4 trees an important tool for  understanding  the  logic  behind
    red-black trees,  and  this  is  why  many  introductory  algorithm  texts
    introduce 2-3-4 trees just before red-black trees, even though 2-3-4 trees
    are not often used in practice.

    <b>Properties</b>\n
    \image html BinTreeRB.png "Red-Black binary tree"
    A red-black tree is a binary search tree  where  each  node  has  a  color
    attribute, the value of which is either red or black. In addition  to  the
    ordinary  requirements  imposed  on  binary  search  trees,  we  make  the
    following additional requirements of any valid red-black tree:

    \li 1. A node is either red or black.
    \li 2. The root is black.
    \li 3. All leaves are black. (This includes the NIL children)
    \li 4. Both children of every red node are black.  (i.e.  Every  red  node
           must have a black parent.)
    \li 5. All paths from any given node to its leaf nodes  contain  the  same
           number of black nodes.

    These constraints enforce a critical property of red-black trees: that the
    longest possible path from the root to a leaf is no  more  than  twice  as
    long as the shortest possible path. The result is that the tree is roughly
    balanced. Since operations such as inserting, deleting, and finding values
    requires worst-case time proportional to the  height  of  the  tree,  this
    theoretical upper bound  on  the  height  allows  red-black  trees  to  be
    efficient in the worst-case, unlike ordinary binary search trees.

    To see why these properties guarantee this, it suffices to  note  that  no
    path can have two red nodes in a row, due  to  property  4.  The  shortest
    possible  path  has  all  black  nodes,  and  the  longest  possible  path
    alternates between red and black nodes. Since all maximal paths  have  the
    same number of black nodes, by property 5, this shows that no path is more
    than twice as long as any other path.

    In many presentations of tree data structures, it is possible for  a  node
    to have only one child, and leaf nodes contain data.  It  is  possible  to
    present red-black trees in this paradigm, but it changes  several  of  the
    properties and complicates  the  algorithms.  For  this  reason,  in  this
    article we use "nil leaves" or "null leaves", which contain  no  data  and
    merely serve to indicate where the tree ends, as shown above. These  nodes
    are often omitted  in  drawings,  resulting  in  a  tree  which  seems  to
    contradict the above principles, but which in fact does not. A consequence
    of this is that all internal (non-leaf) nodes have two children,  although
    one or more of those children may be a null leaf.

    Some explain a red-black tree as a binary search tree whose edges, instead
    of nodes, are colored in  red  or  black,  but  this  does  not  make  any
    difference. The color of a node in  our  terminology  corresponds  to  the
    color of the edge connecting the node to its parent, except that the  root
    node  is  always  black  in  our  terminology  (property  2)  whereas  the
    corresponding edge does not exist.

    <b>Operations</b>\n
    Read-only operations on a red-black  tree  require  no  modification  from
    those used for binary search trees, because  every  red-black  tree  is  a
    specialization of a simple binary  search  tree.  However,  the  immediate
    result of an  insertion  or  removal  may  violate  the  properties  of  a
    red-black tree. Restoring the red-black properties requires a small number
    (O(log n) or amortized O(1)) of color changes (which  are  very  quick  in
    practice) and no more than  three  tree  rotations  (two  for  insertion).
    Although insert and delete operations are complicated, their times  remain
    O(log n).

    <b>Proof of asymptotic bounds</b>\n

    A red black  tree  which  contains  n  internal  nodes  has  a  height  of
    O(log(n)).

    Definitions:

    \li h(v) = height of subtree rooted at node v
    \li bh(v) = the number of black nodes (not counting v if it is black) from
        v to any leaf in the subtree (called the black-height).

    <b>Lemma:</b> A subtree rooted at node v has at least 2bh(v) ? 1  internal
    nodes.

    Proof of Lemma (by induction height):

    Basis: h(v) = 0

    If v has a height of zero then it must be nil, therefore bh(v) = 0. So:

    \f$2^{bh(v)} - 1 = 20 - 1 = 1 - 1 = 0\f$

    Inductive Hypothesis: v such that  h(v)  =  k,  has  \f$2^{bh(v)}  -  1\f$
    internal nodes implies that v' such that h(v') = k+1 has  \f$2^{bh(v')}  -
    1\f$ internal nodes.

    Since v' has h(v') > 0 it is an internal node. As such it has two children
    which have a black-height of  either  bh(v')  or  bh(v')-1  (depending  on
    whether v' is red or black). By the inductive hypothesis each child has at
    least \f$2^{bh(v)} - 1 - 1\f$ internal nodes, so v' has:

    \f[2^{bh(v)} - 1 - 1 + 2^{bh(v')} - 1 - 1 + 1 = 2^{bh(v')} - 1\f]

    internal nodes.

    Using this lemma  we  can  now  show  that  the  height  of  the  tree  is
    logarithmic. Since at least half of the nodes on any path from the root to
    a leaf are black (property 4 of a red black tree), the  black-  height  of
    the root is at least \f$h(root) \over 2\f$. By the lemma we get:

    \f[n \geq 2^{{h(root) \over 2}} - 1 \leftrightarrow  \;  \log{(n+1)}  \geq
    {h(root) \over 2} \leftrightarrow \; h(root) \leq 2\log{(n+1)}\f]

    Therefore the height of the root is O(log(n)).

    <b>Usage</b>\n
    Reb-Black trees have as a first advantage that their performance is easier
    to  predict, making  them a good  data structure  for libraries. Reb-Black
    tree  win AVL trees  in cases when there are lots of inserts / deletes and
    comparisons  are  chap.  Reb-Black tree  will be faster because on average
    Reb-Black tree use less rotation.

    <b>References</b>\n
    \li Mathworld: Red-Black Tree
    \li San Diego State University: CS 660: Red-Black  tree  notes,  by  Roger
        Whitney
    \li Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, and Clifford
        Stein. Introduction to  Algorithms,  Second  Edition.  MIT  Press  and
        McGraw-Hill, 2001. ISBN 0-262-03293-7 . Chapter 13:  Red-Black  Trees,
        pp.273-301.

    <b>Taken from:</b>\n
    Red-black tree from Wikipedia, the free encyclopedia
    http://en.wikipedia.org/wiki/Red-black_tree
*/
template <typename T, typename TCompare = std::less<T>>
class BinTreeRB
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
    typedef BinTreeIterator<BinTreeRB<T, TCompare>, T> iterator;
    typedef BinTreeConstIterator<BinTreeRB<T, TCompare>, T> const_iterator;
    typedef BinTreeReverseIterator<BinTreeRB<T, TCompare>, T> reverse_iterator;
    typedef BinTreeConstReverseIterator<BinTreeRB<T, TCompare>, T> const_reverse_iterator;

    //! Red-Black binary tree node
    struct Node
    {
        T* parent;  //!< Pointer to the parent binary tree node
        T* left;    //!< Pointer to the left child binary tree node
        T* right;   //!< Pointer to the right child binary tree node
        bool rb;    //!< Red-Black flag

        Node() : parent(nullptr), left(nullptr), right(nullptr), rb(false) {}
    };

    explicit BinTreeRB(const TCompare& compare = TCompare()) noexcept
        : _compare(compare),
          _size(0),
          _root(nullptr)
    {}
    template <class InputIterator>
    BinTreeRB(InputIterator first, InputIterator last, const TCompare& compare = TCompare()) noexcept;
    BinTreeRB(const BinTreeRB&) noexcept = default;
    BinTreeRB(BinTreeRB&&) noexcept = default;
    ~BinTreeRB() noexcept = default;

    BinTreeRB& operator=(const BinTreeRB&) noexcept = default;
    BinTreeRB& operator=(BinTreeRB&&) noexcept = default;

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
    void swap(BinTreeRB& bintree) noexcept;
    template <typename U, typename UCompare>
    friend void swap(BinTreeRB<U, UCompare>& bintree1, BinTreeRB<U, UCompare>& bintree2) noexcept;

private:
    TCompare _compare;  // Binary tree compare
    size_t _size;       // Binary tree size
    T* _root;           // Binary tree root node

    const T* InternalLowest() const noexcept;
    const T* InternalHighest() const noexcept;
    const T* InternalFind(const T& item) const noexcept;
    const T* InternalLowerBound(const T& item) const noexcept;
    const T* InternalUpperBound(const T& item) const noexcept;

    void RotateLeft(T* node);
    void RotateRight(T* node);
    void Unlink(T* node, T* parent);
    static void Swap(T*& node1, T*& node2);
};

} // namespace CppCommon

#include "bintree_rb.inl"

#endif // CPPCOMMON_CONTAINERS_BINTREE_RB_H
