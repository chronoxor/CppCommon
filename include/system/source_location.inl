/*!
    \file source_location.inl
    \brief Source location inline implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

namespace CppCommon {

inline std::ostream& operator<<(std::ostream& os, const SourceLocation& instance)
{
    if ((instance.filename() == nullptr) || (instance.line() == 0))
        return os;

    return os << instance.filename() << ':' << instance.line();
}

} // namespace CppCommon
