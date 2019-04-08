/*!
    \file bintree_splay.h
    \brief Intrusive balanced Splay binary tree container definition
    \author Ivan Shynkarenka
    \date 23.06.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_CONTAINERS_BINTREE_SPLAY_H
#define CPPCOMMON_CONTAINERS_BINTREE_SPLAY_H

#include "bintree.h"

namespace CppCommon {

//! Intrusive balanced Splay binary tree container
/*!
    Not thread-safe.

    <b>Overview</b>\n
    A splay tree is a self-balancing binary search tree  with  the  additional
    unusual property that recently  accessed  elements  are  quick  to  access
    again. It performs basic operations such as insertion, look-up and removal
    in O(log(n)) amortized time. For many non-uniform sequences of operations,
    splay trees perform better than other search trees, even when the specific
    pattern of the sequence is unknown. The splay tree was invented by  Daniel
    Sleator and Robert Tarjan.

    All normal operations on a binary search tree are combined with one  basic
    operation, called splaying.  Splaying  the  tree  for  a  certain  element
    rearranges the tree so that the element is placed at the root of the tree.
    One way to do this is to first perform a standard binary tree  search  for
    the element in question, and then use tree rotations in a specific fashion
    to bring the element to the top. Alternatively, a bottom-up algorithm  can
    combine the search and the tree reorganization.

    <b>Advantages and disadvantages</b>\n
    Good performance for a splay tree depends on the fact  that  it  is  self-
    balancing, and indeed self optimising, in that frequently  accessed  nodes
    will move nearer to the root where they can be accessed more quickly. This
    is an advantage for nearly all practical applications, and is particularly
    useful for implementing caches; however it is important to note  that  for
    uniform access, a splay tree's performance will be considerably  (although
    not asymptotically) worse than a somewhat balanced  simple  binary  search
    tree.

    Splay trees also have the  advantage  of  being  considerably  simpler  to
    implement than other self-balancing binary  search  trees,  such  as  red-
    black trees or AVL trees, while their average-case performance is just  as
    efficient. Also, splay trees do not need to store  any  bookkeeping  data,
    thus minimizing memory requirements. However, these other data  structures
    provide worst-case time guarantees, and can be more efficient in  practice
    for uniform access.

    One worst case issue with the  basic  splay  tree  algorithm  is  that  of
    sequentially accessing all the elements of the tree  in  the  sort  order.
    This leaves the tree completely unbalanced (this takes n accesses  -  each
    an O(1) operation). Reaccessing the first item triggers an operation  that
    takes O(n) operations to rebalance the tree  before  returning  the  first
    item. This is a significant delay for that final operation,  although  the
    amortised performance over the entire sequence is actually O(1).  However,
    recent research shows that randomly rebalancing the tree  can  avoid  this
    unbalancing effect  and  give  similar  performance  to  the  other  self-
    balancing algorithms.

    It is possible to create a persistent version of splay trees which  allows
    access to both the  previous  and  new  versions  after  an  update.  This
    requires amortized O(log n) space per update.

    Contrarily to other types of self balancing trees, splay trees  work  well
    with  nodes  containing  identical  keys.  Even   with   identical   keys,
    performance remains amortized O(log n). All tree operations  preserve  the
    order of the identical nodes within the tree, which is a property  similar
    to stable sorting algorithms. A  carefully  designed  find  operation  can
    return the left most or right most node of a given key.

    <b>The splay operation</b>\n
    When a node x is accessed, a splay operation is performed on x to move  it
    to the root. To perform a splay operation we carry out a sequence of splay
    steps, each of which moves x closer to the  root.  As  long  as  x  has  a
    grandparent, each particular step depends on two factors:
    \li Whether x is the left or right child of its parent node, p,
    \li Whether p is the left or right child of its parent, g (the grandparent
        of x).

    Thus, there are four cases when x has a grandparent. They  fall  into  two
    types of splay steps.

    \image html BinTreeSplay-Zigzag.png "Zig-zag step"
    <b>Zig-zag Step:</b> One zig-zag case is when x is the right  child  of  p
    and p is the left child of g (shown above). p is the new left child of  x,
    g is the new right child of x, and the subtrees A, B, C, and D  of  x,  p,
    and g are rearranged as necessary. The other zig-zag case  is  the  mirror
    image of this, i.e. when x is the left child of p and p is the right child
    of g. Note that a zig-zag step is equivalent to doing a  rotation  on  the
    edge between x and p, then doing a rotation on the edge between x and g.

    \image html BinTreeSplay-Zigzig.png "Zig-zig step"
    <b>Zig-zig Step:</b> One zig-zig case is when x is the left child of p and
    p is the left child of g (shown above). p is the new right child of  x,  g
    is the new right child of p, and the subtrees A, B, C, and D of x, p,  and
    g are rearranged as necessary. The other zig-zig case is the mirror  image
    of this, i.e. when x is the right child of p and p is the right  child  of
    g. Note that zig-zig steps are the only  thing  that  differentiate  splay
    trees from the rotate to root method indroduced by Allen and  Munro  prior
    to the introduction of splay trees.

    \image html BinTreeSplay-Zig.png "Zig step"
    <b>Zig Step:</b> There is also a third kind of splay  step  that  is  done
    when x has a parent p but no grandparent. This is called a zig step and is
    simply a rotation on the edge between x and p. Zig  steps  exist  to  deal
    with the parity issue and will be done only as the last step  in  a  splay
    operation and only when x has odd depth at the beginning of the operation.

    By performing a splay operation  on  the  node  of  interest  after  every
    access, we keep recently accessed nodes near the root and  keep  the  tree
    roughly balanced, so that we achieve the desired amortized time bounds.

    <b>Performance theorems</b>\n
    There are  several  theorems  and  conjectures  regarding  the  worst-case
    runtime for performing a  sequence  S  of  m  accesses  in  a  splay  tree
    containing n elements.

    \li <b>Balance Theorem:</b> The cost of performing the sequence S is
    \f$O\left(m + (\log n + 1) + n\log(n) \right)\f$. In  other  words,  splay
    trees perform as well as static balanced binary search trees on  sequences
    of at least n accesses.

    \li <b>Static Optimality Theorem:</b>  Let  qi  be  the  number  of  times
    element  i  is  accessed   in   S.   The   cost   of   performing   S   is
    \f$O\left(m+\sum_{i=1}^n q_i\log\frac{m}{q_i}\right)\f$. In  other  words,
    splay trees perform as well as  optimum  static  binary  search  trees  on
    sequences of at least n accesses.

    \li <b>Static Finger Theorem:</b> Let ij be the element  accessed  in  the
    jth access of S and let f be any fixed element (the finger). The  cost  of
    performing   S   is   \f$O\left(m+n\log   n+\sum_{j=1}^m   \log(|i_j-f|+1)
    \right)\f$.

    \li <b>Working Set  Theorem:</b>  Let  t(j)  be  the  number  of  distinct
    elements accessed between access j and the previous time  element  ij  was
    accessed. The cost of performing  S  is  \f$O\left(m+n\log  n+\sum_{j=1}^m
    \log(t(j)+1) \right)\f$.

    \li  <b>Dynamic  Finger  Theorem:</b>  The  cost  of   performing   S   is
    \f$O\left(m+n\log n+\sum_{j=1}^m \log(|i_{j+1}-i_j|+1) \right)\f$.

    \li <b>Scanning Theorem:</b> Also known as the Sequential Access  Theorem.
    Accessing the n elements of a splay tree in  symmetric  order  takes  O(n)
    time, regardles of the initial structure of the splay tree.  The  tightest
    upper bound proven so far is 4.5n.

    <b>Dynamic optimality conjecture</b>\n
    In addition to the proven performance guarantees for splay trees there  is
    an unproven conjecture of great interest from  the  original  Sleator  and
    Tarjan  paper.  This  conjecture  is  known  as  the  dynamic   optimality
    conjecture and it basically claims that splay trees perform as well as any
    other binary search tree algorithm up to a constant factor.

    \li <b>Dynamic Optimality Conjecture:</b> Let A be any binary search  tree
    algorithm that accesses an element x by traversing the path from the  root
    to x at a cost of d(x) +  1,  and  that  between  accesses  can  make  any
    rotations in the tree at a cost of 1 per rotation. Let A(S)  be  the  cost
    for A to perform the sequence S of accesses. Then the  cost  for  a  splay
    tree to perform the same accesses is O(n + A(S)).

    There is a special case of the dynamic optimality conjecture known as  the
    traversal conjecture that is also unproven.

    \li <b>Traversal Conjecture:</b>   Let  T1  and  T2  be  two  splay  trees
    containing the same elements. Let S be the sequence obtained  by  visiting
    the elements in T2 in preorder (i.e. depth first search order). The  total
    cost of performing the sequence S of acesses on T1 is O(n).

    <b>Usage</b>\n
    Splay tree might be used in different caches and provides near O(1) lookup
    to the most frequent access items.

    <b>References</b>\n
    \li Sleator and R.E. Tarjan. Self-Adjusting Binary Search  Trees.  Journal
        of the ACM 32:3, pages 652-686, 1985.
    \li R. Cole, B. Mishra, J. Schmidt,  A.  Siegel.  On  the  Dynamic  Finger
        Conjecture  for  Splay  Trees.  Part  I:  Splay  Sorting  log  n-Block
        Sequences. SIAM Journal on Computing 30, pages 1-43, 2000.
    \li R. Cole. On the Dynamic Finger Conjecture for Splay  Trees.  Part  II:
        The Proof. SIAM Journal on Computing 30, pages 44-85, 2000.
    \li R.E. Tarjan. Sequential access  in  splay  trees  takes  linear  time.
        Combinatorica 5, pages 367-378, 1985.
    \li Amr Elmasry. On the sequential access theorem and deque conjecture for
        splay trees. Theor. Comput. Sci. 314(3), pages 459-466, 2004.

    <b>Taken from:</b>\n
    Splay tree from Wikipedia, the free encyclopedia
    http://en.wikipedia.org/wiki/Splay_tree
*/
template <typename T, typename TCompare = std::less<T>>
class BinTreeSplay
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
    typedef BinTreeIterator<BinTreeSplay<T, TCompare>, T> iterator;
    typedef BinTreeConstIterator<BinTreeSplay<T, TCompare>, T> const_iterator;
    typedef BinTreeReverseIterator<BinTreeSplay<T, TCompare>, T> reverse_iterator;
    typedef BinTreeConstReverseIterator<BinTreeSplay<T, TCompare>, T> const_reverse_iterator;

    //! Splay binary tree node
    struct Node
    {
        T* parent;          //!< Pointer to the parent binary tree node
        T* left;            //!< Pointer to the left child binary tree node
        T* right;           //!< Pointer to the right child binary tree node

        Node() : parent(nullptr), left(nullptr), right(nullptr) {}
    };

    explicit BinTreeSplay(const TCompare& compare = TCompare()) noexcept
        : _compare(compare),
          _size(0),
          _root(nullptr)
    {}
    template <class InputIterator>
    BinTreeSplay(InputIterator first, InputIterator last, const TCompare& compare = TCompare()) noexcept;
    BinTreeSplay(const BinTreeSplay&) noexcept = default;
    BinTreeSplay(BinTreeSplay&&) noexcept = default;
    ~BinTreeSplay() noexcept = default;

    BinTreeSplay& operator=(const BinTreeSplay&) noexcept = default;
    BinTreeSplay& operator=(BinTreeSplay&&) noexcept = default;

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
    void swap(BinTreeSplay& bintree) noexcept;
    template <typename U, typename UCompare>
    friend void swap(BinTreeSplay<U, UCompare>& bintree1, BinTreeSplay<U, UCompare>& bintree2) noexcept;

private:
    TCompare _compare;  // Binary tree compare
    size_t _size;       // Binary tree size
    T* _root;           // Binary tree root node

    const T* InternalLowest() const noexcept;
    const T* InternalHighest() const noexcept;
    const T* InternalFind(const T& item) const noexcept;
    const T* InternalLowerBound(const T& item) const noexcept;
    const T* InternalUpperBound(const T& item) const noexcept;

    void Splay(T* x) const;
    void Zig(T* x) const;
    void ZigZig(T* x) const;
    void ZigZag(T* x) const;
};

} // namespace CppCommon

#include "bintree_splay.inl"

#endif // CPPCOMMON_CONTAINERS_BINTREE_SPLAY_H
