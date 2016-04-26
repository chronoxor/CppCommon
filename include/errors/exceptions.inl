/*!
    \file exceptions.inl
    \brief Exceptions inline implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

namespace CppCommon {

inline const char* Exception::what() const noexcept
{
    try
    {
        if (_cache.empty())
            to_string();
        return _cache.c_str();
    }
    catch (...)
    {
        return "Out of memory!";
    }
}

template<class T>
inline T&& operator+(const SourceLocation& location, T&& instance)
{
    instance._location = location;
    return std::forward<T>(instance);
}

} // namespace CppCommon
