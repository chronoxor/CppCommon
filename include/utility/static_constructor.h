/*!
    \file static_constructor.h
    \brief Static constructor pattern definition
    \author Ivan Shynkarenka
    \date 31.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_UTILITY_STATIC_CONSTRUCTOR_H
#define CPPCOMMON_UTILITY_STATIC_CONSTRUCTOR_H

namespace CppCommon {

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress" // GCC: warning: the address of 'expression', will always evaluate as 'true'
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4127) // C4127: conditional expression is constant
#endif

//! Static constructor pattern
/*!
    Static constructor pattern is used to emulate static constructor and
    destructor for C++ class.

    Thread-safe.

    Example:
    \code{.cpp}
    class MyClass
    {
    private:
        static void StaticConstructor()
        {
            CppCommon::StaticConstructor<&MyClass::StaticConstructor, &MyClass::StaticDestructor>::instance();
            ...
        }

        static void StaticDestructor()
        {
            ...
        }
    };
    \endcode
*/
template <void (*construct)(), void (*destruct)() = nullptr>
class StaticConstructor
{
    struct constructor;

public:
    //! Static constructor instance
    static constructor instance;

private:
    struct constructor
    {
        constructor()
        {
            if (construct != nullptr)
                construct();
        }

        ~constructor()
        {
            if (destruct != nullptr)
                destruct();
        }

        void operator()() {}
    };
};

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

/*! \example utility_static_constructor.cpp Static constructor pattern example */

} // namespace CppCommon

#include "static_constructor.inl"

#endif // CPPCOMMON_UTILITY_STATIC_CONSTRUCTOR_H
