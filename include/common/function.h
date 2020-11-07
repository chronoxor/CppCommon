/*!
    \file function.h
    \brief Allocation free function definition
    \author Ivan Shynkarenka
    \date 18.08.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_FUNCTION_H
#define CPPCOMMON_FUNCTION_H

#include <functional>
#include <memory>

namespace CppCommon {

//! Allocation free function stub
template <class, size_t Capacity = 1024>
class Function;

//! Allocation free function
/*!
    Allocation free function uses internal stack-based buffer to keep
    the closure. This allows to avoid slow heap allocation in function
    constructor as it performed in std::function implementation.

    Invocation overhead is similar to std::function implementation.
*/
template <class R, class... Args, size_t Capacity>
class Function<R(Args...), Capacity>
{
public:
    Function() noexcept;
    Function(std::nullptr_t) noexcept;
    Function(const Function& function) noexcept;
    Function(Function&& function) noexcept;
    template <class TFunction>
    Function(TFunction&& function) noexcept;
    ~Function() noexcept;

    Function& operator=(std::nullptr_t) noexcept;
    Function& operator=(const Function& function) noexcept;
    Function& operator=(Function&& function) noexcept;
    template <typename TFunction>
    Function& operator=(TFunction&& function) noexcept;
    template <typename TFunction>
    Function& operator=(std::reference_wrapper<TFunction> function) noexcept;

    //! Check if the function is valid
    explicit operator bool() const noexcept { return (_manager != nullptr); }

    //! Invoke the function
    R operator()(Args... args);

    //! Swap two instances
    void swap(Function& function) noexcept;
    template <class UR, class... UArgs, size_t UCapacity>
    friend void swap(Function<UR(UArgs...), UCapacity>& function1, Function<UR(UArgs...), UCapacity>& function2) noexcept;

private:
    enum class Operation { Clone, Destroy };

    using Invoker = R (*)(void*, Args&&...);
    using Manager = void (*)(void*, void*, Operation);

    static const size_t StorageSize = Capacity - sizeof(Invoker) - sizeof(Manager);
    static const size_t StorageAlign = 8;
    using Storage = typename std::aligned_storage<StorageSize, StorageAlign>::type;

    Storage _data;
    Invoker _invoker;
    Manager _manager;

    template <typename TFunction>
    static R Invoke(void* data, Args&&... args) noexcept;

    template <typename TFunction>
    static void Manage(void* dst, void* src, Operation op) noexcept;
};

/*! \example common_function.cpp Allocation free function example */

} // namespace CppCommon

#include "function.inl"

#endif // CPPCOMMON_FUNCTION_H
