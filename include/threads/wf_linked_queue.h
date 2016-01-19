/*!
    \file wf_linked_queue.h
    \brief Wait-free linked queue class definition
    \author Ivan Shynkarenka
    \date 18.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_WF_LINKED_QUEUE_H
#define CPPCOMMON_WF_LINKED_QUEUE_H

#include <atomic>

namespace CppCommon {

//! Wait-free linked queue
/*!
    Multiple producers / single consumer wait-free linked queue use only atomic operations to provide thread-safe
    enqueue and dequeue operations. Linked queue is a dynamically grows queue which allocates memory for each
    new node.

    C++ implementation of Dmitry Vyukov's non-intrusive lock free unbound MPSC queue
    http://www.1024cores.net/home/lock-free-algorithms/queues/non-intrusive-mpsc-node-based-queue
*/
template<typename T>
class WFLinkedQueue
{
public:
    WFLinkedQueue();
    WFLinkedQueue(const WFLinkedQueue&) = delete;
    WFLinkedQueue(WFLinkedQueue&&) = delete;
    ~WFLinkedQueue();

    WFLinkedQueue& operator=(const WFLinkedQueue&) = delete;
    WFLinkedQueue& operator=(WFLinkedQueue&&) = delete;

    //! Enqueue an item into the linked queue (multiple producers threads method)
    /*!
        The item will be copied into the linked queue.

        \param item - item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if there is no enough memory for the queue node
    */
    bool Enqueue(const T& item);

    //! Dequeue an item from the linked queue (single consumer thread method)
    /*!
        The item will be copied from the linked queue.

        \param item - item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the linked queue is empty
    */
    bool Dequeue(T& item);

private:
    struct Node
    {
        T value;
        std::atomic<Node*> next;
    };

    typedef char cache_line_pad[64];

    cache_line_pad _pad0;
    std::atomic<Node*> _head;

    cache_line_pad _pad1;
    std::atomic<Node*> _tail;
};

} // namespace CppCommon

#include "wf_linked_queue.inl"

#endif //CPPCOMMON_WF_LINKED_QUEUE_H
