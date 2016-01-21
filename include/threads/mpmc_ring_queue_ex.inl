/*!
    \file mpmc_ring_queue_ex.inl
    \brief Multiple producers / multiple consumers wait-free ring queue class implementation
    \author Ivan Shynkarenka
    \date 21.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline MPMCRingQueueEx<T>::MPMCRingQueueEx(int64_t capacity) : _capacity(capacity - 1), _mask(capacity - 1), _buffer(new T[capacity]), _head(0), _middle(0), _tail(0)
{
    assert((capacity > 1) && "Ring queue capacity must be greater than one!");
    assert(((capacity & (capacity - 1)) == 0) && "Ring queue capacity must be a power of two!");
}

template<typename T>
inline int64_t MPMCRingQueueEx<T>::size() const
{
    const int64_t head = _head.load(std::memory_order_relaxed);
    const int64_t tail = _tail.load(std::memory_order_relaxed);

    return (head >= tail) ? (head - tail) : (_capacity + head - tail);
}

template<typename T>
inline bool MPMCRingQueueEx<T>::Enqueue(const T& item)
{
    int64_t head, tail;

    do
    {
        head = _head.load(std::memory_order_relaxed);
        tail = _tail.load(std::memory_order_acquire);

        // Check if the ring queue is full
        if (((head - tail + 1) & _mask) == 0)
            return false;

        // Try to increase the head cursor
    } while (!_head.compare_exchange_weak(head, head + 1, std::memory_order_release));

    // Store the item value
    _buffer[head & _mask] = item;

    // Increase the middle cursor
    while (!_middle.compare_exchange_weak(head, head + 1, std::memory_order_release));

    return true;
}

template<typename T>
inline bool MPMCRingQueueEx<T>::Dequeue(T& item)
{
    for (;;)
    {
        int64_t tail = _tail.load(std::memory_order_relaxed);
        int64_t middle = _middle.load(std::memory_order_acquire);

        // Check if the ring queue is empty
        if (((middle - tail) & _mask) == 0)
            return false;

        // Get the item value
        item = _buffer[tail & _mask];

        // Increase the tail cursor
        if (_tail.compare_exchange_weak(tail, tail + 1, std::memory_order_release))
            return true;
    }

    // Never taken
    return false;
}

} // namespace CppCommon
