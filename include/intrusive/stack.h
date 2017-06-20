/*!
    \file stack.h
    \brief Intrusive stack container definition
    \author Ivan Shynkarenka
    \date 20.06.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_INTRUSIVE_STACK_H
#define CPPCOMMON_INTRUSIVE_STACK_H

#include <cassert>
#include <iterator>

namespace CppCommon {

template <typename T>
class StackIterator;
template <typename T>
class StackConstIterator;

//! Intrusive stack container
/*!
    Stack  represents  container  with  LIFO  (last  in  -  first  out)  stock
    algorithm. For example, you insert item A into stack. Then you insert item
    B into stack. There is an item B on the top of the stack.  After  removing
    item B, there will be an item A on the top of the stack.

    \code
      Top-<--- Insert here
       |
    +-----+         +-----+         +-----+         +-----+
    |     |    Next |     |    Next |     |    Next |     |    Next
    |  1  |-------->|  2  |-------->|  3  |-------->|  4  |--------> NULL
    |     |         |     |         |     |         |     |
    +-----+         +-----+         +-----+         +-----+
       |
       +-->--- Remove from here
    \endcode

    Not thread-safe.

    <b>Overview.</b>\n
    \image html Stack.png "Stack."
    In computer science, a stack is a temporary abstract data  type  and  data
    structure based on the principle of Last In First Out (LIFO).  Stacks  are
    used extensively at every level of a modern computer system. For  example,
    a modern PC uses stacks at the architecture level, which are used  to  run
    an operating system. The operating system also uses stacks, which are used
    to run a Java Virtual Machine, which  is  stack  oriented,  and  the  Java
    language itself has a class called "Stack",  which  can  be  used  by  the
    programmer. The stack is ubiquitous.

    A stack-based computer system is one  that  stores  temporary  information
    primarily in stacks, rather than hardware CPU registers (a  register-based
    computer system).

    <b>Abstract data type.</b>\n
    As an abstract data type, the stack is a  container  (data  structure)  of
    nodes and has two basic operations: push and pop. Push adds a  given  node
    to the top of the stack leaving previous  nodes  below.  Pop  removes  and
    returns the current top node of the stack. A frequently used  metaphor  is
    the idea of a stack of plates in a spring loaded cafeteria stack. In  such
    a stack, only the top plate is visible and accessible  to  the  user,  all
    other plates remain hidden. As new plates are  added  (pushed),  each  new
    plate becomes the top of the stack, and hides each plate below. As  plates
    are removed (popped) from the stack, they can be used,  and  second  plate
    becomes the top of the stack. Two important principles are illustrated  by
    this metaphor, the Last In First Out principle is one. The second is  that
    the contents of the stack are hidden. Only the top plate is visible, so to
    see what is on the third plate, the first and second plates will  have  to
    be removed.

    <b>Other Operations.</b>\n
    In modern computer languages, the stack is usually implemented  with  more
    operations than just "push" and "pop". The length of a stack can often  be
    returned as a parameter. Another helper  operation  peek  can  return  the
    current top node of the stack without removing it from the stack.

    <b>Implementation.</b>\n
    A typical storage requirement for a stack  of  n  elements  is  O(n).  The
    typical time requirement of O(1) operations is also easy to  satisfy  with
    an dynamic array or (singly) linked list implementation.

    <b>History.</b>\n
    The stack method of expression evaluation  was  first  proposed  by  early
    German computer scientist F.L.  Bauer,  who  received  the  IEEE  Computer
    Society Pioneer Award in 1988 for his work on Computer Stacks.

    <b>References.</b>\n
    \li Donald Knuth. The Art of Computer Programming, Volume  1:  Fundamental
        Algorithms, Third Edition. Addison-Wesley, 1997.  ISBN  0-201-89683-4.
        Section 2.2.1: Stacks, Queues, and Deques, pp. 238-243.
    \li Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, and Clifford
        Stein. Introduction to  Algorithms,  Second  Edition.  MIT  Press  and
        McGraw-Hill,  2001.  ISBN  0-262-03293-7.  Section  10.1:  Stacks  and
        queues, pp.200-204.

    <b>Taken from:</b>\n
    Stack (data structure) from Wikipedia, the free encyclopedia
    http://en.wikipedia.org/wiki/Stack_%28data_structure%29
*/
template <typename T>
class Stack
{
public:
    //! Stack node
    struct Node
    {
        T* next;    //!< Pointer to the next stack node
    };

    Stack() noexcept : _top(nullptr) {}
    Stack(const Stack&) noexcept = delete;
    Stack(Stack&&) noexcept = default;
    ~Stack() noexcept = default;

    Stack& operator=(const Stack&) noexcept = delete;
    Stack& operator=(Stack&&) noexcept = default;

    //! Check if the stack is not empty
    explicit operator bool() const noexcept { return !empty(); }

    //! Is the stack empty?
    bool empty() const noexcept { return _top == nullptr; }

    //! Get the stack size
    size_t size() const noexcept;

    //! Get the stack top item
    T* top() noexcept { return _top; }
    const T* top() const noexcept { return _top; }

    //! Get the stack begin iterator
    StackIterator<T> begin() noexcept;
    StackConstIterator<T> begin() const noexcept;
    //! Get the stack end iterator
    StackIterator<T> end() noexcept;
    StackConstIterator<T> end() const noexcept;

    //! Push a new item into the top of the stack
    /*!
        \param item - Stack item
        \return The current stack collection
    */
    Stack& Push(T& item) noexcept;
    //! Pop the top item from the top of the stack
    /*!
        \return The top item popped from the stack
    */
    T* Pop() noexcept;

    //! Reverse the stack
    void Reverse() noexcept;

    //! Swap two instances
    void swap(Stack& stack) noexcept;
    template <typename U>
    friend void swap(Stack<U>& stack1, Stack<U>& stack2) noexcept;

private:
    T* _top;    //!< The top node
};

//! Intrusive stack iterator
/*!
    Not thread-safe.
*/
template <typename T>
class StackIterator
{
    friend class Stack<T>;

public:
    // Standard iterator type definitions
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef std::forward_iterator_tag iterator_category;

    StackIterator() noexcept : _current(nullptr) {}
    StackIterator(const StackIterator& it) noexcept = default;
    StackIterator(StackIterator&& it) noexcept = default;
    ~StackIterator() noexcept = default;

    StackIterator& operator=(const StackIterator& it) noexcept = default;
    StackIterator& operator=(StackIterator&& it) noexcept = default;

    friend bool operator==(const StackIterator& it1, const StackIterator& it2) noexcept
    { return it1._current == it2._current; }
    friend bool operator!=(const StackIterator& it1, const StackIterator& it2) noexcept
    { return it1._current != it2._current; }

    StackIterator& operator++() noexcept;
    StackIterator operator++(int) noexcept;

    T& operator*() noexcept;
    T* operator->() noexcept;

    //! Check if the iterator is valid
    explicit operator bool() const noexcept { return _current == nullptr; }

    //! Swap two instances
    void swap(StackIterator& it) noexcept;
    template <typename U>
    friend void swap(StackIterator<U>& it1, StackIterator<U>& it2) noexcept;

private:
    T* _current;

    StackIterator(T* current) noexcept : _current(current) {}
};

//! Intrusive stack constant iterator
/*!
    Not thread-safe.
*/
template <typename T>
class StackConstIterator
{
    friend class Stack<T>;

public:
    // Standard constant iterator type definitions
    typedef const T value_type;
    typedef const T& reference;
    typedef const T* pointer;
    typedef std::forward_iterator_tag iterator_category;

    StackConstIterator() noexcept : _current(nullptr) {}
    StackConstIterator(const StackConstIterator& it) noexcept = default;
    StackConstIterator(StackConstIterator&& it) noexcept = default;
    ~StackConstIterator() noexcept = default;

    StackConstIterator& operator=(const StackConstIterator& it) noexcept = default;
    StackConstIterator& operator=(StackConstIterator&& it) noexcept = default;

    friend bool operator==(const StackConstIterator& it1, const StackConstIterator& it2) noexcept
    { return it1._current == it2._current; }
    friend bool operator!=(const StackConstIterator& it1, const StackConstIterator& it2) noexcept
    { return it1._current != it2._current; }

    StackConstIterator& operator++() noexcept;
    StackConstIterator operator++(int) noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    //! Check if the iterator is valid
    explicit operator bool() const noexcept { return _current == nullptr; }

    //! Swap two instances
    void swap(StackConstIterator& it) noexcept;
    template <typename U>
    friend void swap(StackConstIterator<U>& it1, StackConstIterator<U>& it2) noexcept;

private:
    const T* _current;

    StackConstIterator(const T* current) noexcept : _current(current) {}
};

/*! \example intrusive_stack.cpp Intrusive stack container example */

} // namespace CppCommon

#include "stack.inl"

#endif // CPPCOMMON_INTRUSIVE_STACK_H
