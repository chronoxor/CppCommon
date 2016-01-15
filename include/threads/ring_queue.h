/*!
    \file ring_queue.h
    \brief Wait-free ring queue class definition
    \author Ivan Shynkarenka
    \date 15.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_RING_QUEUE_H
#define CPPCOMMON_RING_QUEUE_H

#include <atomic>

namespace CppCommon {

//! Wait-free ring queue
/*!
    Single producer / single consumer wait-free ring queue use only atomic operations to provide thread safe enqueue
    and dequeue operations. Ring queue is bounded to the fixed size provided through the template parameter.

    A combination of the algorithms described by the circular buffers documentation found in the Linux kernel, and the
    bounded MPMC queue by Dmitry Vyukov. Implemented in pure C++11. Should work across most CPU architectures.
    http://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue
*/
template<typename T, size_t N>
class RingQueue
{
public:
    RingQueue();
    RingQueue(const RingQueue&) = delete;
    RingQueue(RingQueue&&) = delete;
    ~RingQueue() { delete[] _buffer; }

    RingQueue& operator=(const RingQueue&) = delete;
    RingQueue& operator=(RingQueue&&) = delete;

    //! Get ring queue size
    size_t size() const { return _size; }

    //! Enqueue item into the ring queue (producer thread method)
    /*!
        \param item - item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the ring queue is full
    */
    bool Push(const T& item);
    //! Dequeue item from the ring queue (consumer thread method)
    /*!
        \param item - item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the ring queue is empty
    */
    bool Pop(T& item);

private:
    const size_t _size;
    const size_t _mask;
    T* const _buffer;
    std::atomic<size_t> _head;
    std::atomic<size_t> _tail;
};

} // namespace CppCommon

#include "ring_queue.inl"

#endif //CPPCOMMON_RING_QUEUE_H
