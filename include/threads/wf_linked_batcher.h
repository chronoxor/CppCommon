/*!
    \file wf_linked_batcher.h
    \brief Wait-free linked batcher class definition
    \author Ivan Shynkarenka
    \date 19.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_WF_LINKED_BATCHER_H
#define CPPCOMMON_WF_LINKED_BATCHER_H

#include <atomic>
#include <functional>

namespace CppCommon {

//! Wait-free linked batcher
/*!
    Multiple producers / single consumer wait-free linked batcher use only atomic operations to provide thread-safe
    enqueue and batch dequeue operations. Linked batcher is a dynamically grows queue which allocates memory for each
    new node. It allows a consumer thread to process all items in queue in a batch mode.

    Based on Boost Wait-free multi-producer queue
    http://www.boost.org/doc/libs/1_60_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.mp_queue
*/
template<typename T>
class WFLinkedBatcher
{
public:
    WFLinkedBatcher();
    WFLinkedBatcher(const WFLinkedBatcher&) = delete;
    WFLinkedBatcher(WFLinkedBatcher&&) = delete;
    ~WFLinkedBatcher();

    WFLinkedBatcher& operator=(const WFLinkedBatcher&) = delete;
    WFLinkedBatcher& operator=(WFLinkedBatcher&&) = delete;

    //! Enqueue an item into the linked batcher (multiple producers threads method)
    /*!
        The item will be copied into the linked batcher.

        \param item - item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if there is no enough memory for the batcher node
    */
    bool Enqueue(const T& item);

    //! Dequeue all items from the linked queue (single consumer thread method)
    /*!
        All items in the batcher will be processed by the given handler.

        \param hanlder - batch handler
        \return 'true' if all items were successfully handled, 'false' if the linked batcher is empty
    */
    bool Dequeue(const std::function<void(const T&)>& handler);

private:
    struct Node
    {
        T value;
        Node* next;
    };

    std::atomic<Node*> _head;
};

} // namespace CppCommon

#include "wf_linked_batcher.inl"

#endif //CPPCOMMON_WF_LINKED_BATCHER_H
