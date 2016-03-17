/*!
    \file system_error.inl
    \brief System error wrapper inline implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

namespace CppCommon {

inline std::string SystemError::to_string()
{
    return to_string(GetLast());
}

} // namespace CppCommon
