/*!
    \file queue.h
    \brief Intrusive queue container definition
    \author Ivan Shynkarenka
    \date 20.06.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_CONTAINERS_QUEUE_H
#define CPPCOMMON_CONTAINERS_QUEUE_H

#include <cassert>
#include <cstddef>
#include <iterator>

namespace CppCommon {

template <typename T>
class QueueIterator;
template <typename T>
class QueueConstIterator;

//! Intrusive queue container
/*!
    Queue represents  container  with  FIFO  (first  in  -  first  out)  stock
    algorithm. For example, you insert item A into queue. Then you insert item
    B into queue. There is an item A at the begin of the queue and there is an
    item B at the end of the queue. After removing item A, there  will  be  an
    item B at the begin of the queue.

    \code
     Front                          Insert here --->--Back
       |                                               |
    +-----+         +-----+         +-----+         +-----+
    |     |    Next |     |    Next |     |    Next |     |    Next
    |  1  |-------->|  2  |-------->|  3  |-------->|  4  |--------> NULL
    |     |         |     |         |     |         |     |
    +-----+         +-----+         +-----+         +-----+
       |
       +--->--- Remove from here
    \endcode

    Not thread-safe.

    <b>Overview</b>\n
    \image html Queue.png "Queue"
    In providing services  in  computer  science,  transport,  and  operations
    research a queue is a buffer where various entities such as data, objects,
    persons, or events are stored and waiting to be processed. The  most  well
    known operation of  the  queue  is  the  First-In-First-Out  (FIFO)  queue
    process - In a FIFO queue, the first element in  the  queue  will  be  the
    first one out; this is equivalent to  the  requirement  that  whenever  an
    element is added, all elements that were added before have to  be  removed
    before the new element can be invoked.

    There are two basic  operations  associated  with  a  queue:  enqueue  and
    dequeue. Enqueue means adding a new item to the rear of  the  queue  while
    dequeue refers to removing the front item from queue  and  returns  it  in
    item.

    Theoretically, one characteristic of a queue is that it does  not  have  a
    specific capacity. Regardless of how many elements are already  contained,
    a new element can always be added. It can also be empty,  at  which  point
    removing an element will be impossible until a new element has been  added
    again.

    A practical implementation of a queue of course does  have  some  capacity
    limit, that depends on the concrete situation it is used in.  For  a  data
    structure the executing computer will eventually run out of  memory,  thus
    limiting the queue size. Queue overflow results  from  trying  to  add  an
    element onto a full queue and  queue  underflow  happens  when  trying  to
    remove an element from an empty queue.

    <b>Scheduling and buffering queues</b>
    A queue is natural data structure for  a  system  to  serve  the  incoming
    requests. Most of the process scheduling or disk scheduling algorithms  in
    operating systems use queues. Computer hardware  like  a  processor  or  a
    network card also maintain buffers in the  form  of  queues  for  incoming
    resource requests. A stack like data structure causes  starvation  of  the
    first requests, and is not applicable in such cases. A mailbox or port  to
    save messages to communicate between two users or processes in a system is
    essentially a queue like structure.

    <b>Search space exploration</b>
    Like stacks, queues can be used to remember the search space that needs to
    be explored at one point of time in traversing algorithms.  Breadth  first
    search of a graph uses a queue to remember the nodes yet to be visited.

    <b>References</b>\n
    \li Donald Knuth. The Art of Computer Programming, Volume  1:  Fundamental
        Algorithms, Third Edition. Addison-Wesley, 1997.  ISBN  0-201-89683-4.
        Section 2.2.1: Stacks, Queues, and Deques, pp. 238-243.
    \li Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, and Clifford
        Stein. Introduction to  Algorithms,  Second  Edition.  MIT  Press  and
        McGraw-Hill,  2001.  ISBN  0-262-03293-7.  Section  10.1:  Stacks  and
        queues, pp.200-204.

    <b>Taken from:</b>\n
    Stack from Wikipedia, the free encyclopedia
    http://en.wikipedia.org/wiki/Queue
*/
template <typename T>
class Queue
{
public:
    // Standard container type definitions
    typedef T value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef QueueIterator<T> iterator;
    typedef QueueConstIterator<T> const_iterator;

    //! Queue node
    struct Node
    {
        T* next;    //!< Pointer to the next queue node

        Node() : next(nullptr) {}
    };

    Queue() noexcept : _size(0), _front(nullptr), _back(nullptr) {}
    template <class InputIterator>
    Queue(InputIterator first, InputIterator last) noexcept;
    Queue(const Queue&) noexcept = default;
    Queue(Queue&&) noexcept = default;
    ~Queue() noexcept = default;

    Queue& operator=(const Queue&) noexcept = default;
    Queue& operator=(Queue&&) noexcept = default;

    //! Check if the queue is not empty
    explicit operator bool() const noexcept { return !empty(); }

    //! Is the queue empty?
    bool empty() const noexcept { return _front == nullptr; }

    //! Get the queue size
    size_t size() const noexcept { return _size; }

    //! Get the front queue item
    T* front() noexcept { return _front; }
    const T* front() const noexcept { return _front; }
    //! Get the back queue item
    T* back() noexcept { return _back; }
    const T* back() const noexcept { return _back; }

    //! Get the begin queue iterator
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    //! Get the end queue iterator
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    //! Push a new item into the back of the queue
    /*!
        \param item - Pushed item
    */
    void push(T& item) noexcept;

    //! Pop the item from the front of the queue
    /*!
        \return The front item popped from the queue
    */
    T* pop() noexcept;

    //! Reverse the queue
    void reverse() noexcept;

    //! Clear the queue
    void clear() noexcept;

    //! Swap two instances
    void swap(Queue& queue) noexcept;
    template <typename U>
    friend void swap(Queue<U>& queue1, Queue<U>& queue2) noexcept;

private:
    size_t _size;   // Queue size
    T* _front;      // Queue front node
    T* _back;       // Queue back node
};

//! Intrusive queue iterator
/*!
    Not thread-safe.
*/
template <typename T>
class QueueIterator
{
    friend QueueConstIterator<T>;

public:
    // Standard iterator type definitions
    typedef T value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef std::forward_iterator_tag iterator_category;

    QueueIterator() noexcept : _node(nullptr) {}
    explicit QueueIterator(T* node) noexcept : _node(node) {}
    QueueIterator(const QueueIterator& it) noexcept = default;
    QueueIterator(QueueIterator&& it) noexcept = default;
    ~QueueIterator() noexcept = default;

    QueueIterator& operator=(const QueueIterator& it) noexcept = default;
    QueueIterator& operator=(QueueIterator&& it) noexcept = default;

    friend bool operator==(const QueueIterator& it1, const QueueIterator& it2) noexcept
    { return it1._node == it2._node; }
    friend bool operator!=(const QueueIterator& it1, const QueueIterator& it2) noexcept
    { return it1._node != it2._node; }

    QueueIterator& operator++() noexcept;
    QueueIterator operator++(int) noexcept;

    reference operator*() noexcept;
    pointer operator->() noexcept;

    //! Check if the iterator is valid
    explicit operator bool() const noexcept { return _node != nullptr; }

    //! Swap two instances
    void swap(QueueIterator& it) noexcept;
    template <typename U>
    friend void swap(QueueIterator<U>& it1, QueueIterator<U>& it2) noexcept;

private:
    T* _node;
};

//! Intrusive queue constant iterator
/*!
    Not thread-safe.
*/
template <typename T>
class QueueConstIterator
{
public:
    // Standard iterator type definitions
    typedef T value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef std::forward_iterator_tag iterator_category;

    QueueConstIterator() noexcept : _node(nullptr) {}
    explicit QueueConstIterator(const T* node) noexcept : _node(node) {}
    QueueConstIterator(const QueueIterator<T>& it) noexcept : _node(it._node) {}
    QueueConstIterator(const QueueConstIterator& it) noexcept = default;
    QueueConstIterator(QueueConstIterator&& it) noexcept = default;
    ~QueueConstIterator() noexcept = default;

    QueueConstIterator& operator=(const QueueIterator<T>& it) noexcept
    { _node = it._node; return *this; }
    QueueConstIterator& operator=(const QueueConstIterator& it) noexcept = default;
    QueueConstIterator& operator=(QueueConstIterator&& it) noexcept = default;

    friend bool operator==(const QueueConstIterator& it1, const QueueConstIterator& it2) noexcept
    { return it1._node == it2._node; }
    friend bool operator!=(const QueueConstIterator& it1, const QueueConstIterator& it2) noexcept
    { return it1._node != it2._node; }

    QueueConstIterator& operator++() noexcept;
    QueueConstIterator operator++(int) noexcept;

    const_reference operator*() const noexcept;
    const_pointer operator->() const noexcept;

    //! Check if the iterator is valid
    explicit operator bool() const noexcept { return _node != nullptr; }

    //! Swap two instances
    void swap(QueueConstIterator& it) noexcept;
    template <typename U>
    friend void swap(QueueConstIterator<U>& it1, QueueConstIterator<U>& it2) noexcept;

private:
    const T* _node;
};

/*! \example containers_queue.cpp Intrusive queue container example */

} // namespace CppCommon

#include "queue.inl"

#endif // CPPCOMMON_CONTAINERS_QUEUE_H
