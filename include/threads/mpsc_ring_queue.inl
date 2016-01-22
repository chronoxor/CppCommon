/*!
    \file mpsc_ring_queue.inl
    \brief Multiple producers / single consumer wait-free ring queue class implementation
    \author Ivan Shynkarenka
    \date 21.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline MPSCRingQueue<T>::MPSCRingQueue(int64_t capacity, int64_t concurrency) : _capacity(capacity), _concurrency(concurrency)
{
    // Initialize producers' ring buffers
    for (int64_t i = 0; i < concurrency; ++i)
        _producers.push_back(std::make_shared<Producer>(capacity));
}

template<typename T>
inline MPSCRingQueue<T>::~MPSCRingQueue()
{
    // Remove all nodes from the consumer's ring buffer
    Dequeue([](const T&){});
}

template<typename T>
inline bool MPSCRingQueue<T>::Enqueue(const T& item)
{
    // Get producer index for the current thread based on RDTS value
    uint64_t timestamp = RDTS::current();
    uint64_t index = RDTS::current() % _concurrency;

    // Lock the chosen producer using its spin-lock
    std::lock_guard<SpinLock> lock(_producers[index]->lock);

    // Enqueue the item into the producer's ring buffer
    return _producers[index]->queue.Enqueue(Item(timestamp, item));
}

template<typename T>
inline bool MPSCRingQueue<T>::Dequeue(T& item)
{
    // Try to dequeue one item from the consumer's priority queue
    if (Consume(item))
        return true;

    // Flush all available items from producers' ring buffers to the consumer priority queue
    Flush();

    // Force to dequeue one item from the consumer's priority queue
    return Consume(item);
}

template<typename T>
inline bool MPSCRingQueue<T>::Dequeue(const std::function<void(const T&)>& handler)
{
    bool result = false;

    // Flush all available items from producers' ring buffers to the consumer priority queue
    Flush();

    // Try to dequeue all items from the consumer's priority queue
    T temp;
    while (Consume(temp))
    {
        handler(temp);
        result = true;
    }

    return result;
}

template<typename T>
inline bool MPSCRingQueue<T>::Consume(T& item)
{
    if (_consumer.empty())
        return false;

    item = _consumer.top().value;
    _consumer.pop();
    return true;
}

template<typename T>
inline void MPSCRingQueue<T>::Flush()
{
    Item temp;
    for (int64_t i = 0; i < _concurrency; ++i)
        while (_producers[i]->queue.Dequeue(temp))
            _consumer.push(temp);
}

} // namespace CppCommon
