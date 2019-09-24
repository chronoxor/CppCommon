/*!
    \file dll.inl
    \brief Dynamic link library inline implementation
    \author Ivan Shynkarenka
    \date 07.06.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T>
inline T* DLL::Resolve(const std::string& name) const
{
    return (T*)ResolveAddress(name);
}

inline std::string DLL::prefix()
{
#if defined(__CYGWIN__)
    return "cyg";
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MINGW32__) || defined(__MINGW64__)
    return "lib";
#else
    return "";
#endif
}

inline std::string DLL::extension()
{
#if defined(__APPLE__)
    return ".dylib";
#elif (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
    return ".so";
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    return ".dll";
#else
    return "";
#endif
}

inline void swap(DLL& dll1, DLL& dll2) noexcept
{
    dll1.swap(dll2);
}

} // namespace CppCommon
