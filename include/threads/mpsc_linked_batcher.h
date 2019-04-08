/*!
    \file mpsc_linked_batcher.h
    \brief Multiple producers / single consumer wait-free linked batcher definition
    \author Ivan Shynkarenka
    \date 19.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_MPSC_LINKED_BATCHER_H
#define CPPCOMMON_THREADS_MPSC_LINKED_BATCHER_H

#include <atomic>
#include <cassert>
#include <functional>
#include <utility>

namespace CppCommon {

//! Multiple producers / single consumer wait-free linked batcher
/*!
    Multiple producers / single consumer wait-free linked batcher use only atomic operations to provide thread-safe
    enqueue and batch dequeue operations. Linked batcher is a dynamically grows queue which allocates memory for each
    new node. It allows a consumer thread to process all items in queue in a batch mode.

    FIFO order is guaranteed!

    Thread-safe.

    Based on Boost Wait-free multi-producer queue
    http://www.boost.org/doc/libs/1_60_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.mp_queue
*/
template<typename T>
class MPSCLinkedBatcher
{
public:
    MPSCLinkedBatcher();
    MPSCLinkedBatcher(const MPSCLinkedBatcher&) = delete;
    MPSCLinkedBatcher(MPSCLinkedBatcher&&) = delete;
    ~MPSCLinkedBatcher();

    MPSCLinkedBatcher& operator=(const MPSCLinkedBatcher&) = delete;
    MPSCLinkedBatcher& operator=(MPSCLinkedBatcher&&) = delete;

    //! Enqueue an item into the linked batcher (multiple producers threads method)
    /*!
        The item will be copied into the linked batcher.

        Will not block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if there is no enough memory for the batcher node
    */
    bool Enqueue(const T& item);
    //! Enqueue an item into the linked batcher (multiple producers threads method)
    /*!
        The item will be moved into the linked batcher.

        Will not block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if there is no enough memory for the batcher node
    */
    bool Enqueue(T&& item);

    //! Dequeue all items from the linked queue (single consumer thread method)
    /*!
        All items in the batcher will be processed by the given handler.

        Will not block.

        \param handler - Batch handler (default is empty handler)
        \return 'true' if all items were successfully handled, 'false' if the linked batcher is empty
    */
    bool Dequeue(const std::function<void(const T&)>& handler = [](const int&){});

private:
    struct Node
    {
        Node* next;
        T value;
    };

    std::atomic<Node*> _head;
};

} // namespace CppCommon

#include "mpsc_linked_batcher.inl"

#endif // CPPCOMMON_THREADS_MPSC_LINKED_BATCHER_H
