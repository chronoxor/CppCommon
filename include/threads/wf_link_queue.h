/*!
    \file wf_link_queue.h
    \brief Wait-free link queue class definition
    \author Ivan Shynkarenka
    \date 18.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_WF_LINK_QUEUE_H
#define CPPCOMMON_WF_LINK_QUEUE_H

#include <atomic>

namespace CppCommon {

//! Wait-free link queue
/*!
    Multiple producer / single consumer wait-free link queue use only atomic operations to provide thread safe enqueue
    and dequeue operations. Link queue is a dynamically grows queue which allocates memory for each new node.
*/
template<typename T>
class WFLinkQueue
{
public:
    //! Wait-free link queue node
    struct Node
    {
        //! Node value
        T value;
        //! Next node
        std::atomic<Node*> next;
    };

public:
    WFLinkQueue();
    WFLinkQueue(const WFLinkQueue&) = delete;
    WFLinkQueue(WFLinkQueue&&) = delete;
    ~WFLinkQueue();

    WFLinkQueue& operator=(const WFLinkQueue&) = delete;
    WFLinkQueue& operator=(WFLinkQueue&&) = delete;

    //! Enqueue an item into the link queue (producer thread method)
    /*!
        The item will be copied into the link queue.

        \param item - item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if there is no enough memory for the queue node
    */
    bool Enqueue(const T& item);

    //! Dequeue an item from the link queue (consumer thread method)
    /*!
        The item will be copied from the link queue.

        \param item - item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the link queue is empty
    */
    bool Dequeue(T& item);

private:
    typedef char cache_line_pad[64];

    cache_line_pad _pad0;
    std::atomic<Node*> _head;

    cache_line_pad _pad1;
    std::atomic<Node*> _tail;
};

} // namespace CppCommon

#include "wf_link_queue.inl"

#endif //CPPCOMMON_WF_LINK_QUEUE_H
