/*!
    \file wf_bounded_queue.h
    \brief Wait-free bounded queue class definition
    \author Ivan Shynkarenka
    \date 19.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_WF_BOUNDED_QUEUE_H
#define CPPCOMMON_WF_BOUNDED_QUEUE_H

#include <atomic>
#include <cassert>

namespace CppCommon {

//! Wait-free bounded queue
/*!
    Multiple producers / multiple consumers wait-free bounded queue use only atomic operations to provide thread-safe
    enqueue and dequeue operations. Bounded queue size is limited to the capacity provided in the constructor.

    C++ implementation of Dmitry Vyukov's non-intrusive lock free unbound MPSC queue
    http://www.1024cores.net/home/lock-free-algorithms/queues/non-intrusive-mpsc-node-based-queue
*/
template<typename T>
class WFBoundedQueue
{
public:
    //! Default class constructor
    /*!
        \param capacity - bounded queue capacity (must be a power of two)
    */
    WFBoundedQueue(int64_t capacity);
    WFBoundedQueue(const WFBoundedQueue&) = delete;
    WFBoundedQueue(WFBoundedQueue&&) = delete;
    ~WFBoundedQueue() { delete[] _buffer; }

    WFBoundedQueue& operator=(const WFBoundedQueue&) = delete;
    WFBoundedQueue& operator=(WFBoundedQueue&&) = delete;

    //! Get bounded queue capacity
    int64_t capacity() const { return _capacity; }
    //! Get bounded queue size
    int64_t size() const;

    //! Enqueue an item into the bounded queue (multiple producers threads method)
    /*!
        The item will be copied into the bounded queue.

        \param item - item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the bounded queue is full
    */
    bool Enqueue(const T& item);

    //! Dequeue an item from the bounded queue (multiple consumers threads method)
    /*!
        The item will be copied from the bounded queue.

        \param item - item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the bounded queue is empty
    */
    bool Dequeue(T& item);

private:
    struct Node
    {
        T value;
        std::atomic<int64_t> sequence;
    };

    typedef char cache_line_pad[64];

    cache_line_pad _pad0;
    const int64_t _capacity;
    const int64_t _mask;
    Node* const _buffer;

    cache_line_pad _pad1;
    std::atomic<int64_t> _head;

    cache_line_pad _pad2;
    std::atomic<int64_t> _tail;
};

} // namespace CppCommon

#include "wf_bounded_queue.inl"

#endif //CPPCOMMON_WF_BOUNDED_QUEUE_H
