/*!
    \file source_location.inl
    \brief Source location inline implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

namespace CppCommon {

inline std::ostream& operator<<(std::ostream& os, const SourceLocation& source_location)
{
    if ((source_location.filename() == nullptr) || (source_location.line() == 0))
        return os;

    return os << source_location.filename() << ':' << source_location.line();
}

} // namespace CppCommon

#if defined(FMT_VERSION)
template <> struct fmt::formatter<CppCommon::SourceLocation> : ostream_formatter {};
#endif
