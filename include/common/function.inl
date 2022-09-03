/*!
    \file function.inl
    \brief Allocation free function inline implementation
    \author Ivan Shynkarenka
    \date 18.08.2017
    \copyright MIT License
*/

namespace CppCommon {

template <class R, class... Args, size_t Capacity>
inline Function<R(Args...), Capacity>::Function() noexcept
    : _data(),
      _invoker(nullptr),
      _manager(nullptr)
{
}

template <class R, class... Args, size_t Capacity>
inline Function<R(Args...), Capacity>::Function(std::nullptr_t) noexcept
    : Function<R(Args...), Capacity>()
{
}

template <class R, class... Args, size_t Capacity>
inline Function<R(Args...), Capacity>::Function(const Function& function) noexcept
    : Function<R(Args...), Capacity>()
{
    if (function)
    {
        function._manager(&_data, &function._data, Operation::Clone);
        _invoker = function._invoker;
        _manager = function._manager;
    }
}

template <class R, class... Args, size_t Capacity>
inline Function<R(Args...), Capacity>::Function(Function&& function) noexcept
    : Function<R(Args...), Capacity>()
{
    function.swap(*this);
}

template <class R, class... Args, size_t Capacity>
template <class TFunction>
inline Function<R(Args...), Capacity>::Function(TFunction&& function) noexcept
    : Function<R(Args...), Capacity>()
{
    using function_type = typename std::decay<TFunction>::type;

    // Check implementation storage parameters
    static_assert((StorageSize >= sizeof(function_type)), "Function::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(function_type)) == 0), "Function::StorageAlign must be adjusted!");

    // Create the implementation instance
    new (&_data) function_type(std::forward<TFunction>(function));

    _invoker = &Invoke<function_type>;
    _manager = &Manage<function_type>;
}

template <class R, class... Args, size_t Capacity>
inline Function<R(Args...), Capacity>::~Function() noexcept
{
    if (_manager)
        _manager(&_data, nullptr, Operation::Destroy);
}

template <class R, class... Args, size_t Capacity>
inline Function<R(Args...), Capacity>& Function<R(Args...), Capacity>::operator=(std::nullptr_t) noexcept
{
    if (_manager)
    {
        _manager(&_data, nullptr, Operation::Destroy);
        _manager = nullptr;
        _invoker = nullptr;
    }
    return *this;
}

template <class R, class... Args, size_t Capacity>
inline Function<R(Args...), Capacity>& Function<R(Args...), Capacity>::operator=(const Function& function) noexcept
{
    Function(function).swap(*this);
    return *this;
}

template <class R, class... Args, size_t Capacity>
inline Function<R(Args...), Capacity>& Function<R(Args...), Capacity>::operator=(Function&& function) noexcept
{
    Function(std::move(function)).swap(*this);
    return *this;
}

template <class R, class... Args, size_t Capacity>
template <typename TFunction>
inline Function<R(Args...), Capacity>& Function<R(Args...), Capacity>::operator=(TFunction&& function) noexcept
{
    Function(std::forward<TFunction>(function)).swap(*this);
    return *this;
}

template <class R, class... Args, size_t Capacity>
template <typename TFunction>
inline Function<R(Args...), Capacity>& Function<R(Args...), Capacity>::operator=(std::reference_wrapper<TFunction> function) noexcept
{
    Function(function).swap(*this);
    return *this;
}

template <class R, class... Args, size_t Capacity>
inline R Function<R(Args...), Capacity>::operator()(Args... args)
{
    if (!_invoker)
        throw std::bad_function_call();

    return _invoker(&_data, std::forward<Args>(args)...);
}

template <class R, class... Args, size_t Capacity>
template <typename TFunction>
inline R Function<R(Args...), Capacity>::Invoke(void* data, Args&&... args) noexcept
{
    TFunction& function = *static_cast<TFunction*>(data);
    return function(std::forward<Args>(args)...);
}

template <class R, class... Args, size_t Capacity>
template <typename TFunction>
inline void Function<R(Args...), Capacity>::Manage(void* dst, void* src, Operation op) noexcept
{
    switch (op)
    {
        case Operation::Clone:
            new (dst) TFunction(*static_cast<TFunction*>(src));
            break;
        case Operation::Destroy:
            static_cast<TFunction*>(dst)->~TFunction();
            break;
    }
}

template <class R, class... Args, size_t Capacity>
inline void Function<R(Args...), Capacity>::swap(Function& function) noexcept
{
    using std::swap;
    swap(_data, function._data);
    swap(_manager, function._manager);
    swap(_invoker, function._invoker);
}

template <class R, class... Args, size_t Capacity>
void swap(Function<R(Args...), Capacity>& function1, Function<R(Args...), Capacity>& function2) noexcept
{
    function1.swap(function2);
}

} // namespace CppCommon
