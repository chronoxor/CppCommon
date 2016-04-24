/*!
    \file system_location.inl
    \brief Source location wrapper inline implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

namespace CppCommon {

inline std::string SourceLocation::to_string() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

inline std::ostream& operator<<(std::ostream& os, const SourceLocation& instance)
{
    if ((instance.filename() == nullptr) || (instance.line() == 0))
        return os;

    return os << instance.filename() << ':' << instance.line();
}

} // namespace CppCommon
