/*!
    \file spsc_ring_queue.inl
    \brief Single producer / single consumer wait-free ring queue class implementation
    \author Ivan Shynkarenka
    \date 15.01.2016
    \copyright MIT License
*/

namespace {
    constexpr bool IsPowerOfTwo(int64_t x) { return x && ((x & (x - 1)) == 0); }
}

namespace CppCommon {

template<typename T>
inline SPSCRingQueue<T>::SPSCRingQueue(int64_t capacity) : _capacity(capacity - 1), _mask(capacity - 1), _buffer(new T[capacity]), _head(0), _tail(0)
{
    assert((capacity > 1) && "Ring queue capacity must be greater than one!");
    assert(IsPowerOfTwo(capacity) && "Ring queue capacity must be a power of two!");
}

template<typename T>
inline int64_t SPSCRingQueue<T>::size() const
{
    const int64_t head = _head.load(std::memory_order_relaxed);
    const int64_t tail = _tail.load(std::memory_order_relaxed);

    return (head >= tail) ? (head - tail) : (_capacity + 1 + head - tail);
}

template<typename T>
inline bool SPSCRingQueue<T>::Enqueue(const T& item)
{
    const int64_t head = _head.load(std::memory_order_relaxed);
    const int64_t tail = _tail.load(std::memory_order_acquire);

    // Check if the ring queue is full
    if (((head - tail + 1) & _mask) == 0)
        return false;

    // Store the item value
    _buffer[head & _mask] = item;

    // Increase the head cursor
    _head.store(head + 1, std::memory_order_release);

    return true;
}

template<typename T>
inline bool SPSCRingQueue<T>::Dequeue(T& item)
{
    const int64_t tail = _tail.load(std::memory_order_relaxed);
    const int64_t head = _head.load(std::memory_order_acquire);

    // Check if the ring queue is empty
    if (((head - tail) & _mask) == 0)
        return false;

    // Get the item value
    item = _buffer[tail & _mask];

    // Increase the tail cursor
    _tail.store(tail + 1, std::memory_order_release);

    return true;
}

} // namespace CppCommon
