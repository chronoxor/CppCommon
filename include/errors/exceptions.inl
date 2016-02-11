/*!
    \file exceptions.inl
    \brief Exceptions inline implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

namespace CppCommon {

inline const char* Exception::what() const
{
    if (_cache.empty())
        to_string();
    return _cache.c_str();
}

template<class T>
inline T&& operator+(const std::tuple<SourceLocation, StackTrace>& context, T&& instance)
{
    instance._location = std::get<0>(context);
    instance._trace = std::get<1>(context);
    return std::forward<T>(instance);
}

} // namespace CppCommon
