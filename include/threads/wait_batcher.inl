/*!
    \file wait_batcher.inl
    \brief Multiple producers / multiple consumers wait batcher inline implementation
    \author Ivan Shynkarenka
    \date 13.03.2019
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline WaitBatcher<T>::WaitBatcher() : _closed(false)
{
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
inline size_t WaitBatcher<T>::size() const
{
    Locker<CriticalSection> locker(_cs);
    return _batch.size();
}

template<typename T>
inline bool WaitBatcher<T>::Enqueue(const T& item)
{
    T temp = item;
    return Enqueue(std::forward<T>(temp));
}

template<typename T>
inline bool WaitBatcher<T>::Enqueue(T&& item)
{
    Locker<CriticalSection> locker(_cs);
    if (_closed)
        return false;
    _batch.emplace_back(item);
    _cv.NotifyOne();
    return true;
}

template<typename T>
template <class InputIterator>
inline bool WaitBatcher<T>::Enqueue(InputIterator first, InputIterator last)
{
    Locker<CriticalSection> locker(_cs);
    if (_closed)
        return false;
    _batch.insert(_batch.end(), first, last);
    _cv.NotifyOne();
    return true;
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
            return true;
        }

        _cv.Wait(_cs, [this]() { return (_closed || !_batch.empty()); });

    } while (!_closed || !_batch.empty());

    return false;
}

template<typename T>
inline void WaitBatcher<T>::Close()
{
    Locker<CriticalSection> locker(_cs);
    _closed = true;
    _cv.NotifyAll();
}

} // namespace CppCommon
