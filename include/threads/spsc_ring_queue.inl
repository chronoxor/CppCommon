/*!
    \file spsc_ring_queue.inl
    \brief Single producer / single consumer wait-free ring queue inline implementation
    \author Ivan Shynkarenka
    \date 15.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline SPSCRingQueue<T>::SPSCRingQueue(size_t capacity) : _capacity(capacity - 1), _mask(capacity - 1), _buffer(new T[capacity]), _head(0), _tail(0)
{
    assert((capacity > 1) && "Ring queue capacity must be greater than one!");
    assert(((capacity & (capacity - 1)) == 0) && "Ring queue capacity must be a power of two!");

    memset(_pad0, 0, sizeof(cache_line_pad));
    memset(_pad1, 0, sizeof(cache_line_pad));
    memset(_pad2, 0, sizeof(cache_line_pad));
    memset(_pad3, 0, sizeof(cache_line_pad));
}

template<typename T>
inline size_t SPSCRingQueue<T>::size() const noexcept
{
    const size_t head = _head.load(std::memory_order_acquire);
    const size_t tail = _tail.load(std::memory_order_acquire);

    return head - tail;
}

template<typename T>
inline bool SPSCRingQueue<T>::Enqueue(const T& item)
{
    T temp = item;
    return Enqueue(std::forward<T>(temp));
}

template<typename T>
inline bool SPSCRingQueue<T>::Enqueue(T&& item)
{
    const size_t head = _head.load(std::memory_order_relaxed);
    const size_t tail = _tail.load(std::memory_order_acquire);

    // Check if the ring queue is full
    if (((head - tail + 1) & _mask) == 0)
        return false;

    // Store the item value
    _buffer[head & _mask] = std::move(item);

    // Increase the head cursor
    _head.store(head + 1, std::memory_order_release);

    return true;
}

template<typename T>
inline bool SPSCRingQueue<T>::Dequeue(T& item)
{
    const size_t tail = _tail.load(std::memory_order_relaxed);
    const size_t head = _head.load(std::memory_order_acquire);

    // Check if the ring queue is empty
    if (((head - tail) & _mask) == 0)
        return false;

    // Get the item value
    item = std::move(_buffer[tail & _mask]);

    // Increase the tail cursor
    _tail.store(tail + 1, std::memory_order_release);

    return true;
}

} // namespace CppCommon
