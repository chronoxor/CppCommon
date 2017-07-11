/*!
    \file mpsc_linked_queue.inl
    \brief Multiple producers / single consumer wait-free linked queue inline implementation
    \author Ivan Shynkarenka
    \date 18.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline MPSCLinkedQueue<T>::MPSCLinkedQueue() : _head(new Node), _tail(_head.load(std::memory_order_relaxed))
{
    memset(_pad0, 0, sizeof(cache_line_pad));
    memset(_pad1, 0, sizeof(cache_line_pad));
    memset(_pad2, 0, sizeof(cache_line_pad));

    // Linked queue is initialized with a fake node as a head node
    Node* front = _head.load(std::memory_order_relaxed);
    front->next.store(nullptr, std::memory_order_relaxed);
}

template<typename T>
inline MPSCLinkedQueue<T>::~MPSCLinkedQueue()
{
    // Remove all nodes from the linked queue
    T item;
    while (Dequeue(item)) {}

    // Remove the last fake node
    Node* front = _head.load(std::memory_order_relaxed);
    delete front;
}

template<typename T>
inline bool MPSCLinkedQueue<T>::Enqueue(const T& item)
{
    T temp = item;
    return Enqueue(std::forward<T>(temp));
}

template<typename T>
inline bool MPSCLinkedQueue<T>::Enqueue(T&& item)
{
    // Create new head node
    Node* node = new Node;
    if (node == nullptr)
        return false;

    // Fill new head node with the given value
    node->value = std::move(item);
    node->next.store(nullptr, std::memory_order_relaxed);

    // Insert new head node into the queue and linked it with the previous one
    Node* prev_head = _head.exchange(node, std::memory_order_acq_rel);
    prev_head->next.store(node, std::memory_order_release);

    return true;
}

template<typename T>
inline bool MPSCLinkedQueue<T>::Dequeue(T& item)
{
    Node* tail = _tail.load(std::memory_order_relaxed);
    Node* next = tail->next.load(std::memory_order_acquire);

    // Check if the linked queue is empty
    if (next == nullptr)
        return false;

    // Get the item value
    item = std::move(next->value);

    // Update tail node with a next one
    _tail.store(next, std::memory_order_release);

    // Remove the previous tail node
    delete tail;

    return true;
}

} // namespace CppCommon
