/*!
    \file resource.h
    \brief Resource smart deleter pattern definition
    \author Ivan Shynkarenka
    \date 01.10.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_UTILITY_RESOURCE_H
#define CPPCOMMON_UTILITY_RESOURCE_H

#include <memory>

namespace CppCommon {

//! Resource smart deleter pattern
/*!
    Resource smart deleter pattern allows to create unique smart pointer with
    a given resource and cleaner delegate which is used to automatic resource
    clean when it goes out of scope.

    Thread-safe.

    Example:
    \code{.cpp}
    int test()
    {
        // Create a file resource
        auto file = resource(fopen("test", "rb"), [](FILE* file) { fclose(file); });

        // Work with the file resource
        int result = fgetc(file.get());

        // File resource will be cleaned automatically when we go out of scope
        return result;
    }
    \endcode

    \param handle - Resource handle
    \param cleaner - Cleaner function
*/
template <typename T, typename TCleaner>
auto resource(T handle, TCleaner cleaner)
{
    return std::unique_ptr<std::remove_pointer<T>::type, TCleaner>(handle, cleaner);
}

//! Resource smart deleter pattern (void* specialization)
/*!
    \param handle - Resource handle
    \param cleaner - Cleaner function
*/
template <typename TCleaner>
auto resource(void* handle, TCleaner cleaner)
{
    return std::unique_ptr<void, TCleaner>(handle, cleaner);
}

} // namespace CppCommon

#endif // CPPCOMMON_UTILITY_RESOURCE_H
