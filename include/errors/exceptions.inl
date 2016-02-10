/*!
    \file exceptions.inl
    \brief Exceptions inline implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

namespace CppCommon {

inline const char* Exception::what() const throw()
{
    if (_cache.empty())
        to_string();
    return _cache.c_str();
}

template<class T>
inline T&& operator+(const std::tuple<SourceLocation, StackTrace>& context, T&& instance)
{
    instance._location = context.get<0>();
    instance._trace = context.get<1>();
    return instance;
}

} // namespace CppCommon
