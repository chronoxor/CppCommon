/*!
    \file static_constructor.inl
    \brief Static constructor pattern inline implementation
    \author Ivan Shynkarenka
    \date 31.08.2016
    \copyright MIT License
*/

namespace CppCommon {

template <void (*construct)(), void (*destruct)()>
typename StaticConstructor<construct, destruct>::constructor StaticConstructor<construct, destruct>::instance;

} // namespace CppCommon
