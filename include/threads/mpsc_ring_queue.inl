/*!
    \file mpsc_ring_queue.inl
    \brief Multiple producers / single consumer wait-free ring queue class implementation
    \author Ivan Shynkarenka
    \date 21.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline MPSCRingQueue<T>::MPSCRingQueue(int64_t capacity, int64_t concurrency) : _capacity(capacity), _concurrency(concurrency), _consumer(capacity * concurrency)
{
    // Initialize producers' ring queue
    for (int64_t i = 0; i < concurrency; ++i)
        _producers.push_back(std::make_shared<Producer>(capacity));
}

template<typename T>
inline MPSCRingQueue<T>::~MPSCRingQueue()
{
    // Remove all nodes from the consumer's ring queue
    Dequeue([](const T&){});
}

template<typename T>
inline bool MPSCRingQueue<T>::Enqueue(const T& item)
{
    // Get producer index for the current thread based on RDTS value
    uint64_t timestamp = RDTS::current();
    uint64_t index = timestamp % _concurrency;

    // Lock the chosen producer using its spin-lock
    std::lock_guard<SpinLock> lock(_producers[index]->lock);

    // Enqueue the item into the producer's ring queue
    return _producers[index]->queue.Enqueue(item);
}

template<typename T>
inline bool MPSCRingQueue<T>::Dequeue(T& item)
{
    // Try to dequeue one item from the consumer's ring queue
    if (_consumer.Dequeue(item))
        return true;

    // Flush all available items from producers' ring queues to the consumer ring queue
    bool consumed = false;
    for (int64_t i = 0;; ++i)
    {
        // Check if we need to stop or continue consuming
        if (i == _concurrency)
        {
            // Stop if nothing is consumed or reset the consumed flag
            if (!consumed)
                break;
            else
                consumed = false;

            // If at least one item was produced and all of them were consumed 
            // then reset the current producer's counter
            i = 0;  
        }

        // Try to dequeue one item from the current producer's ring queue
        T temp;
        if (_producers[i]->queue.Dequeue(temp))
        {
            // Set the consumed flag
            consumed = true;

            // Try to enqueue the item into the consumer ring queue
            if (!_consumer.Enqueue(temp))
            {
                // If the consumer ring queue is full then dequeue result item 
                // and enqueue the current item into it
                _consumer.Dequeue(item);
                _consumer.Enqueue(temp);
                return true;
            }
        }
    }

    // Force to dequeue one item from the consumer's ring queue
    return _consumer.Dequeue(item);
}

template<typename T>
inline bool MPSCRingQueue<T>::Dequeue(const std::function<void(const T&)>& handler)
{
    bool result = false;

    // Consume all available items from the consumer ring queue
    T temp;
    while (_consumer.Dequeue(temp))
    {
        handler(temp);
        result = true;
    }

    // Consume all available items from producers' ring queues
    bool consumed = false;
    for (int64_t i = 0;; ++i)
    {
        // Check if we need to stop or continue consuming
        if (i == _concurrency)
        {
            // Stop if nothing is consumed or reset the consumed flag
            if (!consumed)
                break;
            else
                consumed = false;

            // If at least one item was produced and all of them were consumed 
            // then reset the current producer's counter
            i = 0;  
        }

        // Try to dequeue one item from the current producer's ring queue
        if (_producers[i]->queue.Dequeue(temp))
        {
            // Set the consumed flag
            consumed = true;

            // Consume the item
            handler(temp);
            result = true;
        }
    }

    return result;
}

} // namespace CppCommon
