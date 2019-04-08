/*!
    \file mpsc_ring_queue.h
    \brief Multiple producers / single consumer wait-free ring queue definition
    \author Ivan Shynkarenka
    \date 21.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_MPSC_RING_QUEUE_H
#define CPPCOMMON_THREADS_MPSC_RING_QUEUE_H

#include "threads/spin_lock.h"
#include "threads/spsc_ring_queue.h"
#include "time/timestamp.h"

#include <cassert>
#include <cstdio>
#include <functional>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

namespace CppCommon {

//! Multiple producers / single consumer wait-free ring queue
/*!
    Multiple producers / single consumer wait-free ring queue use only atomic operations to provide thread-safe
    enqueue and dequeue operations. This data structure consist of several SPSC ring queues which count is
    provided as a hardware concurrency in the constructor. All of them are randomly accessed with a RDTS
    distribution index. All the items available in sesequential or batch mode. All ring queue sizes are
    limited to the capacity provided in the constructor.

    FIFO order is not guaranteed!

    Thread-safe.
*/
template<typename T>
class MPSCRingQueue
{
public:
    //! Default class constructor
    /*!
        \param capacity - Ring queue capacity (must be a power of two)
        \param concurrency - Hardware concurrency (default is std::thread::hardware_concurrency)
    */
    explicit MPSCRingQueue(size_t capacity, size_t concurrency = std::thread::hardware_concurrency());
    MPSCRingQueue(const MPSCRingQueue&) = delete;
    MPSCRingQueue(MPSCRingQueue&&) = delete;
    ~MPSCRingQueue() = default;

    MPSCRingQueue& operator=(const MPSCRingQueue&) = delete;
    MPSCRingQueue& operator=(MPSCRingQueue&&) = delete;

    //! Check if the queue is not empty
    explicit operator bool() const noexcept { return !empty(); }

    //! Is ring queue empty?
    bool empty() const noexcept { return (size() == 0); }
    //! Get ring queue capacity
    size_t capacity() const noexcept { return _capacity; }
    //! Get ring queue concurrency
    size_t concurrency() const noexcept { return _concurrency; }
    //! Get ring queue size
    size_t size() const noexcept;

    //! Enqueue an item into the ring queue (multiple producers threads method)
    /*!
        The item will be copied into the ring queue.

        Will not block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the ring queue is full
    */
    bool Enqueue(const T& item);
    //! Enqueue an item into the ring queue (multiple producers threads method)
    /*!
        The item will be moved into the ring queue.

        Will not block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the ring queue is full
    */
    bool Enqueue(T&& item);

    //! Dequeue an item from the ring queue (single consumer threads method)
    /*!
        The item will be moved from the ring queue.

        Will not block.

        \param item - Item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the ring queue is empty
    */
    bool Dequeue(T& item);

    //! Dequeue all items from the linked queue (single consumer thread method)
    /*!
        All items in the batcher will be processed by the given handler.

        Will not block.

        \param handler - Batch handler (default is empty handler)
        \return 'true' if all items were successfully handled, 'false' if the linked batcher is empty
    */
    bool Dequeue(const std::function<void(const T&)>& handler = [](const int&){});

private:
    struct Producer
    {
        SpinLock lock;
        SPSCRingQueue<T> queue;

        Producer(size_t capacity) : queue(capacity) {}
    };

    size_t _capacity;
    size_t _concurrency;
    std::vector<std::shared_ptr<Producer>> _producers;
    size_t _consumer;
};

/*! \example threads_mpsc_ring_queue.cpp Multiple producers / single consumer wait-free ring queue example */

} // namespace CppCommon

#include "mpsc_ring_queue.inl"

#endif // CPPCOMMON_THREADS_MPSC_RING_QUEUE_H
