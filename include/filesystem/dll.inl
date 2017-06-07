/*!
    \file dll.inl
    \brief Dynamic link library inline implementation
    \author Ivan Shynkarenka
    \date 07.06.2017
    \copyright MIT License
*/

namespace CppCommon {

inline std::string DLL::extension()
{
#if defined(__APPLE__)
    return ".dylib";
#elif defined(unix) || defined(__unix) || defined(__unix__)
    return ".so";
#elif defined(_WIN32) || defined(_WIN64)
    return ".dll";
#else
    return "";
#endif
}

} // namespace CppCommon
