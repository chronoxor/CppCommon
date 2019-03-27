/*!
    \file wait_batcher.inl
    \brief Multiple producers / multiple consumers wait batcher inline implementation
    \author Ivan Shynkarenka
    \date 13.03.2019
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline WaitBatcher<T>::WaitBatcher(size_t capacity, size_t initial) : _closed(false),  _capacity(capacity)
{
    _batch.reserve(initial);
}

template<typename T>
inline WaitBatcher<T>::~WaitBatcher()
{
    Close();
}

template<typename T>
inline bool WaitBatcher<T>::closed() const
{
    Locker<CriticalSection> locker(_cs);
    return _closed;
}

template<typename T>
inline size_t WaitBatcher<T>::capacity() const
{
    if (_capacity > 0)
        return _capacity;

    Locker<CriticalSection> locker(_cs);
    return _batch.capacity();
}

template<typename T>
inline size_t WaitBatcher<T>::size() const
{
    Locker<CriticalSection> locker(_cs);
    return _batch.size();
}

template<typename T>
inline bool WaitBatcher<T>::Enqueue(const T& item)
{
    Locker<CriticalSection> locker(_cs);

    if (_closed)
        return false;

    do
    {
        if ((_capacity == 0) || (_batch.size() < _capacity))
        {
            _batch.push_back(item);
            _cv1.NotifyOne();
            return true;
        }

        _cv2.Wait(_cs, [this]() { return (_closed || (_capacity == 0) || (_batch.size() < _capacity)); });

    } while (!_closed);

    return false;
}

template<typename T>
inline bool WaitBatcher<T>::Enqueue(T&& item)
{
    Locker<CriticalSection> locker(_cs);

    if (_closed)
        return false;

    do
    {
        if ((_capacity == 0) || (_batch.size() < _capacity))
        {
            _batch.emplace_back(item);
            _cv1.NotifyOne();
            return true;
        }

        _cv2.Wait(_cs, [this]() { return (_closed || (_capacity == 0) || (_batch.size() < _capacity)); });

    } while (!_closed);

    return false;
}

template<typename T>
template <class InputIterator>
inline bool WaitBatcher<T>::Enqueue(InputIterator first, InputIterator last)
{
    Locker<CriticalSection> locker(_cs);

    if (_closed)
        return false;

    do
    {
        if ((_capacity == 0) || (_batch.size() < _capacity))
        {
            _batch.insert(_batch.end(), first, last);
            _cv1.NotifyOne();
            return true;
        }

        _cv2.Wait(_cs, [this]() { return (_closed || (_capacity == 0) || (_batch.size() < _capacity)); });

    } while (!_closed);

    return false;
}

template<typename T>
inline bool WaitBatcher<T>::Dequeue(std::vector<T>& items)
{
    // Clear the result items vector
    items.clear();

    Locker<CriticalSection> locker(_cs);

    if (_closed && _batch.empty())
        return false;

    do
    {
        if (!_batch.empty())
        {
            // Swap batch items
            std::swap(_batch, items);
            _cv2.NotifyOne();
            return true;
        }

        _cv1.Wait(_cs, [this]() { return (_closed || !_batch.empty()); });

    } while (!_closed || !_batch.empty());

    return false;
}

template<typename T>
inline void WaitBatcher<T>::Close()
{
    Locker<CriticalSection> locker(_cs);
    _closed = true;
    _cv1.NotifyAll();
    _cv2.NotifyAll();
}

} // namespace CppCommon
