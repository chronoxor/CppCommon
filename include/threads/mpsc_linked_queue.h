/*!
    \file mpsc_linked_queue.h
    \brief Multiple producers / single consumer wait-free linked queue definition
    \author Ivan Shynkarenka
    \date 18.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_MPSC_LINKED_QUEUE_H
#define CPPCOMMON_THREADS_MPSC_LINKED_QUEUE_H

#include <atomic>
#include <cstring>
#include <utility>

namespace CppCommon {

//! Multiple producers / single consumer wait-free linked queue
/*!
    Multiple producers / single consumer wait-free linked queue use only atomic operations to provide thread-safe
    enqueue and dequeue operations. Linked queue is a dynamically grows queue which allocates memory for each
    new node.

    FIFO order is guaranteed!

    Thread-safe.

    C++ implementation of Dmitry Vyukov's non-intrusive lock free unbound MPSC queue
    http://www.1024cores.net/home/lock-free-algorithms/queues/non-intrusive-mpsc-node-based-queue
*/
template<typename T>
class MPSCLinkedQueue
{
public:
    MPSCLinkedQueue();
    MPSCLinkedQueue(const MPSCLinkedQueue&) = delete;
    MPSCLinkedQueue(MPSCLinkedQueue&&) = delete;
    ~MPSCLinkedQueue();

    MPSCLinkedQueue& operator=(const MPSCLinkedQueue&) = delete;
    MPSCLinkedQueue& operator=(MPSCLinkedQueue&&) = delete;

    //! Enqueue an item into the linked queue (multiple producers threads method)
    /*!
        The item will be copied into the linked queue.

        Will not block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if there is no enough memory for the queue node
    */
    bool Enqueue(const T& item);
    //! Enqueue an item into the linked queue (multiple producers threads method)
    /*!
        The item will be moved into the linked queue.

        Will not block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if there is no enough memory for the queue node
    */
    bool Enqueue(T&& item);

    //! Dequeue an item from the linked queue (single consumer thread method)
    /*!
        The item will be moved from the linked queue.

        Will not block.

        \param item - Item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the linked queue is empty
    */
    bool Dequeue(T& item);

private:
    struct Node
    {
        std::atomic<Node*> next;
        T value;
    };

    typedef char cache_line_pad[128];

    cache_line_pad _pad0;
    std::atomic<Node*> _head;
    cache_line_pad _pad1;
    std::atomic<Node*> _tail;
    cache_line_pad _pad2;
};

/*! \example threads_mpsc_linked_queue.cpp Multiple producers / single consumer wait-free linked queue example */

} // namespace CppCommon

#include "mpsc_linked_queue.inl"

#endif // CPPCOMMON_THREADS_MPSC_LINKED_QUEUE_H
