/*!
    \file stack_trace.inl
    \brief Stack trace snapshot provider inline implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

namespace CppCommon {

inline std::string StackTrace::Frame::to_string() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

inline std::string StackTrace::to_string() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

} // namespace CppCommon
