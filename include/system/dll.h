/*!
    \file dll.h
    \brief Dynamic link library definition
    \author Ivan Shynkarenka
    \date 07.06.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_DLL_H
#define CPPCOMMON_SYSTEM_DLL_H

#include "filesystem/path.h"
#include "system/exceptions.h"

#include <memory>

//! DLL export macro
/*!
    DLL export macro is used to mark functions or classes that
    should be exported into dynamic linked library:

    \code{.cpp}
    EXPORT int MyFunction();
    class EXPORT MyClass {};
    \endcode
*/
#if defined(_WIN32) || defined(_WIN64)
  #ifdef EXPORTS
    #define EXPORT __declspec(dllexport)
  #else
    #define EXPORT __declspec(dllimport)
  #endif
#else
  #define EXPORT
#endif

//! DLL API macro
/*!
    DLL API is used to mark exported API functions:

    \code{.cpp}
    API int PluginFunction();
    \endcode
*/
#define API extern "C" EXPORT

namespace CppCommon {

//! Dynamic link library
/*!
    Dynamic link library wraps dll operations (load, resolve, unload).

    Not thread-safe.
*/
class DLL
{
public:
    //! Initialize the dynamic link library with an empty path
    DLL();
    //! Initialize the dynamic link library with a given path and optionally load it
    /*!
        \param path - Dynamic link library path
        \param load - Load library flag (default is true)
    */
    DLL(const Path& path, bool load = true);
    DLL(const DLL& dll);
    DLL(DLL&& dll) noexcept;
    ~DLL();

    DLL& operator=(const Path& path);
    DLL& operator=(const DLL& dll);
    DLL& operator=(DLL&& dll) noexcept;

    //! Check if the dynamic link library is loaded
    explicit operator bool() const { return IsLoaded(); }

    //! Get the dynamic link library path
    const Path path() const;

    //! Is dynamic link library loaded?
    bool IsLoaded() const;

    //! Is dynamic link library resolve the given symbol?
    bool IsResolve(const std::string& name) const;

    //! Load dynamic link library
    /*!
        \return 'true' if the library was successfully loaded, 'false' if the library was not loaded
    */
    bool Load();
    //! Load dynamic link library with a given path
    /*!
        \param path - Dynamic link library path
        \return 'true' if the library was successfully loaded, 'false' if the library was not loaded
    */
    bool Load(const Path& path);

    //! Unload dynamic link library
    /*!
        If the dynamic link library cannot be unloaded the method will raise
        a dynamic link library exception!
    */
    void Unload();

    //! Resolve dynamic link library symbol by the given name
    /*!
        \param name - Symbol name
        \return A pointer to the resolved symbol or nullptr in case of symbol resolution failed
    */
    template <typename T>
    T* Resolve(const std::string& name) const;

    //! Get the dynamic link library prefix
    /*!
        Cygwin: cyg
        MinGW: lib
        Linux: lib
        OSX: lib
        Windows:

        \return Dynamic link library prefix
    */
    static std::string prefix();
    //! Get the dynamic link library extension
    /*!
        Linux: .so
        OSX: .dylib
        Windows: .dll

        \return Dynamic link library extension
    */
    static std::string extension();

    //! Swap two instances
    void swap(DLL& dll) noexcept;
    friend void swap(DLL& dll1, DLL& dll2) noexcept;

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 48;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;

    //! Resolve dynamic link library symbol by the given name
    /*!
        \param name - Symbol name
        \return A pointer to the resolved symbol or nullptr in case of symbol resolution failed
    */
    void* ResolveAddress(const std::string& name) const;
};

/*! \example system_dll.cpp Dynamic link library example */

} // namespace CppCommon

#include "dll.inl"

#endif // CPPCOMMON_SYSTEM_DLL_H
