/*!
    \file ring_queue.inl
    \brief Wait-free ring queue class implementation
    \author Ivan Shynkarenka
    \date 15.01.2016
    \copyright MIT License
*/

namespace {
    constexpr bool IsPowerOfTwo(int64_t x) { return x && ((x & (x - 1)) == 0); }
}

namespace CppCommon {

template<typename T, int64_t N>
inline RingQueue<T, N>::RingQueue() : _capacity(N - 1), _mask(N - 1), _buffer(new T[N]), _head(0), _tail(0)
{
    static_assert(N > 1, "Ring queue size must be greater than one!");
    static_assert(IsPowerOfTwo(N), "Ring queue size must be a power of two!");
}

template<typename T, int64_t N>
inline int64_t RingQueue<T, N>::size() const
{
    const int64_t head = _head.load(std::memory_order_relaxed);
    const int64_t tail = _tail.load(std::memory_order_acquire);

    return ((head - tail) & _mask);
}

template<typename T, int64_t N>
inline bool RingQueue<T, N>::Enqueue(const T& item)
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

template<typename T, int64_t N>
inline bool RingQueue<T, N>::Dequeue(T& item)
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
