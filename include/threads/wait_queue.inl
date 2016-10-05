/*!
    \file wait_queue.inl
    \brief Multiple producers / multiple consumers wait queue inline implementation
    \author Ivan Shynkarenka
    \date 04.10.2016
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline WaitQueue<T>::WaitQueue() : _closed(false)
{
}

template<typename T>
inline WaitQueue<T>::~WaitQueue()
{
    Close();
}

template<typename T>
inline bool WaitQueue<T>::closed() const
{
    Locker<CriticalSection> locker(_cs);
    return _closed;
}

template<typename T>
inline size_t WaitQueue<T>::size() const
{
    Locker<CriticalSection> locker(_cs);
    return _queue.size();
}

template<typename T>
inline bool WaitQueue<T>::Enqueue(const T& item)
{
    Locker<CriticalSection> locker(_cs);
    if (_closed)
        return false;
    _queue.push(item);
    _cv.NotifyOne();
    return true;
}

template<typename T>
inline bool WaitQueue<T>::Enqueue(T&& item)
{
    Locker<CriticalSection> locker(_cs);
    if (_closed)
        return false;
    _queue.emplace(item);
    _cv.NotifyOne();
    return true;
}

template<typename T>
inline bool WaitQueue<T>::Dequeue(T& item)
{
    Locker<CriticalSection> locker(_cs);

    if (_closed && _queue.empty())
        return false;

    do
    {
        if (!_queue.empty())
        {
            item = _queue.front();
            _queue.pop();
            return true;
        }

        _cv.Wait(_cs, [this]() { return (_closed || !_queue.empty()); });

    } while (!_closed || !_queue.empty());

    return false;
}

template<typename T>
inline void WaitQueue<T>::Close()
{
    Locker<CriticalSection> locker(_cs);
    _closed = true;
    _cv.NotifyAll();
}

} // namespace CppCommon
