/*!
    \file mpsc_ring_queue.inl
    \brief Multiple producers / single consumer wait-free ring queue class implementation
    \author Ivan Shynkarenka
    \date 21.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline MPSCRingQueue<T>::MPSCRingQueue(int64_t capacity, int64_t concurrency) : _capacity(capacity), _concurrency(concurrency), _consumer(concurrency * capacity)
{
    // Initialize producers ring buffers
    for (int64_t i = 0; i < concurrency; ++i)
        _producers.emplace_back(capacity);
}

template<typename T>
inline MPSCRingQueue<T>::~MPSCRingQueue()
{
    // Remove all nodes from the consumer ring buffer
    Dequeue([](const T&){});
}

template<typename T>
inline int64_t MPSCRingQueue<T>::GetProducerId() const
{
#ifdef _MSC_VER
    return __rdtsc();
#endif
}

template<typename T>
inline bool MPSCRingQueue<T>::Enqueue(const T& item)
{
    return _producers[GetProducerId() % _concurrency].Enqueue(item);
}

template<typename T>
inline bool MPSCRingQueue<T>::Dequeue(T& item)
{
    // Try to dequeue one item from the consumer ring buffer
    if (_consumer.Dequeue(item))
        return true;

    // Copy all available items from producers ring buffers to the consumer one
    T temp;
    for (int64_t i = 0; i < _concurrency; ++i)
        while (_producers[i].Dequeue(temp))
            _consumer.Enqueue(temp);

    // Force to dequeue one item from the consumer ring buffer
    return _consumer.Dequeue(item);
}

template<typename T>
inline bool MPSCRingQueue<T>::Dequeue(const std::function<void(const T&)>& handler)
{
    bool result = false;

    // Try to dequeue all items from the consumer ring buffer
    T temp;
    while (_consumer.Dequeue(temp))
    {
        handler(temp);
        result = true;
    }

    // Dequeue all available items from producers ring buffers
    for (int64_t i = 0; i < _concurrency; ++i)
    {
        while (_producers[i].Dequeue(temp))
        {
            handler(temp);
            result = true;
        }
    }

    return result;
}

} // namespace CppCommon
