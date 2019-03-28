/*!
    \file mpsc_ring_queue.inl
    \brief Multiple producers / single consumer wait-free ring queue class inline implementation
    \author Ivan Shynkarenka
    \date 21.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline MPSCRingQueue<T>::MPSCRingQueue(size_t capacity, size_t concurrency) : _capacity(capacity - 1), _concurrency(concurrency), _consumer(0)
{
    // Initialize producers' ring queue
    for (size_t i = 0; i < concurrency; ++i)
        _producers.push_back(std::make_shared<Producer>(capacity));
}

template<typename T>
inline size_t MPSCRingQueue<T>::size() const noexcept
{
    size_t size = 0;
    for (const auto& producer : _producers)
        size += producer->queue.size();
    return size;
}

template<typename T>
inline bool MPSCRingQueue<T>::Enqueue(const T& item)
{
    T temp = item;
    return Enqueue(std::forward<T>(temp));
}

template<typename T>
inline bool MPSCRingQueue<T>::Enqueue(T&& item)
{
    // Get producer index for the current thread based on RDTS value
    size_t index = Timestamp::rdts() % _concurrency;

    // Lock the chosen producer using its spin-lock
    Locker<SpinLock> lock(_producers[index]->lock);

    // Enqueue the item into the producer's ring queue
    return _producers[index]->queue.Enqueue(std::forward<T>(item));
}

template<typename T>
inline bool MPSCRingQueue<T>::Dequeue(T& item)
{
    // Try to dequeue one item from the one of producer's ring queue
    for (size_t i = 0; i < _concurrency; ++i)
    {
        if (_producers[_consumer++ % _concurrency]->queue.Dequeue(item))
            return true;
    }

    return false;
}

template<typename T>
inline bool MPSCRingQueue<T>::Dequeue(const std::function<void(const T&)>& handler)
{
    assert((handler) && "Batch handler must be valid!");

    bool result = false;

    // Consume all available items from producers' ring queues
    for (auto& producer : _producers)
    {
        T item;
        while (producer->queue.Dequeue(item))
        {
            handler(item);
            result = true;
        }
    }

    return result;
}

} // namespace CppCommon
