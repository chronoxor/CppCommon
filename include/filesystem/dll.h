/*!
    \file dll.h
    \brief Dynamic link library definition
    \author Ivan Shynkarenka
    \date 07.06.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_FILESYSTEM_DLL_H
#define CPPCOMMON_FILESYSTEM_DLL_H

#include "filesystem/path.h"

#include <memory>

namespace CppCommon {

//! Dynamic link library
/*!
    Dynamic link library wraps dll operations (load, resolve, unload).

    Not thread-safe.
*/
class DLL
{
public:
    //! Initialize dynamic link library with an empty path
    DLL();
    //! Initialize dynamic link library with a given path
    /*!
        \param path - Dynamic link library path
    */
    DLL(const Path& path);
    DLL(const DLL& dll);
    DLL(DLL&& dll) noexcept;
    ~DLL();

    DLL& operator=(const Path& path);
    DLL& operator=(const DLL& dll);
    DLL& operator=(DLL&& dll) noexcept;

    //! Get the dynamic link library path
    const Path path() const;

    //! Is dynamic link library loaded?
    bool IsLoaded() const;
    //! Is dynamic link library symbol resolving by the given name?
    bool IsResolving(const std::string& name);

    //! Load dynamic link library
    /*!
        If the dynamic link library cannot be loaded the method will raise
        a dynamic link library exception!
    */
    void Load();
    //! Load dynamic link library with a given path
    /*!
        If the dynamic link library cannot be loaded the method will raise
        a dynamic link library exception!

        \param path - Dynamic link library path
    */
    void Load(const Path& path);
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
    void* Resolve(const std::string& name);

    //! Get the dynamic link library extension
    /*!
        Linux: .so
        OSX: .dylib
        Windows: .dll

        \return Dynamic link library extension
    */
    static std::string extension();

private:
    class Impl;
    std::unique_ptr<Impl> _pimpl;
};

/*! \example filesystem_dll.cpp Dynamic link library example */

} // namespace CppCommon

#include "dll.inl"

#endif // CPPCOMMON_FILESYSTEM_DLL_H
