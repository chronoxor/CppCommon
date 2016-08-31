/*!
    \file static_constructor.inl
    \brief Static constructor pattern inline implementation
    \author Ivan Shynkarenka
    \date 31.08.2016
    \copyright MIT License
*/

namespace CppCommon {

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

template <void (*construct)(), void (*destruct)()>
typename StaticConstructor<construct, destruct>::constructor StaticConstructor<construct, destruct>::instance;

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

} // namespace CppCommon
