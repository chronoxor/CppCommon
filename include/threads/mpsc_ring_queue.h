/*!
    \file mpsc_ring_queue.h
    \brief Multiple producers / single consumer wait-free ring queue class definition
    \author Ivan Shynkarenka
    \date 21.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_MPSC_RING_QUEUE_H
#define CPPCOMMON_MPSC_RING_QUEUE_H

#include <atomic>
#include <cassert>
#include <thread>
#include <vector>

#include "spsc_ring_queue.h"

namespace CppCommon {

//! Multiple producers / single consumer wait-free ring queue
/*!
    Multiple producers / single consumer wait-free ring queue use only atomic operations to provide thread-safe
    enqueue and dequeue operations. This data structure consist of several SPSC ring buffers which count is
    provided as a hardware concurrency in the constructor. All of them are randomly accessed with a RDTS
    distribution index. Consumer thread sequentially copy all the items from producer's ring buffers to the
    single consumer's ring buffer. All the items available in sesequential or batch mode. Ring queue size is
    limited to the capacity provided in the constructor.
*/
template<typename T>
class MPSCRingQueue
{
public:
    //! Default class constructor
    /*!
        \param capacity - ring queue capacity (must be a power of two)
        \param concurrency - hardware concurrency (default is std::thread::hardware_concurrency)
    */
    explicit MPSCRingQueue(int64_t capacity, int64_t concurrency = std::thread::hardware_concurrency);
    MPSCRingQueue(const MPSCRingQueue&) = delete;
    MPSCRingQueue(MPSCRingQueue&&) = default;
    ~MPSCRingQueue();

    MPSCRingQueue& operator=(const MPSCRingQueue&) = delete;
    MPSCRingQueue& operator=(MPSCRingQueue&&) = default;

    //! Get ring queue capacity
    int64_t capacity() const { return _capacity; }
    //! Get ring queue concurrency
    int64_t concurrency() const { return _concurrency; }
    //! Get the current producer ring queue size
    int64_t producer_size() const { return _producers[GetProducerId() % _concurrency].size(); }
    //! Get consumer ring queue size
    int64_t consumer_size() const { return _consumer.size(); }

    //! Enqueue an item into the ring queue (multiple producers threads method)
    /*!
        The item will be copied into the ring queue.

        \param item - item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the ring queue is full
    */
    bool Enqueue(const T& item);

    //! Dequeue an item from the ring queue (single consumer threads method)
    /*!
        The item will be copied from the ring queue.

        \param item - item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the ring queue is empty
    */
    bool Dequeue(T& item);

    //! Dequeue all items from the linked queue (single consumer thread method)
    /*!
        All items in the batcher will be processed by the given handler.

        \param hanlder - batch handler
        \return 'true' if all items were successfully handled, 'false' if the linked batcher is empty
    */
    bool Dequeue(const std::function<void(const T&)>& handler);

private:
    int64_t _capacity;
    int64_t _concurrency;
    std::vector<SPSCRingQueue<T>> _producers;
    SPSCRingQueue<T> _consumer;

    //! Get the current producer Id
    int64_t GetProducerId() const;
};

} // namespace CppCommon

#include "mpsc_ring_queue.inl"

#endif //CPPCOMMON_MPSC_RING_QUEUE_H
