/*!
    \file dll.cpp
    \brief Dynamic link library implementation
    \author Ivan Shynkarenka
    \date 07.06.2017
    \copyright MIT License
*/

#include "filesystem/dll.h"

#include "errors/fatal.h"
#include "string/format.h"

#include <cassert>

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <dlfcn.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

//! @cond INTERNALS

class DLL::Impl
{
public:
    Impl() : _dll(nullptr) {}

    ~Impl()
    {
        try
        {
            if (IsLoaded())
                Unload();
        }
        catch (DLLException& ex)
        {
            fatality(DLLException(ex.string()).Attach(_path));
        }
    }

    const Path path() const { return _path; }

    bool IsLoaded() const
    {
        return (_dll != nullptr);
    }

    bool IsResolving(const std::string& name)
    {
        assert(IsLoaded() && "DLL must be loaded!");
        if (!IsLoaded())
            return false;

        return Resolve(name) != nullptr;
    }

    void Assign(const Path& path)
    {
        assert(!IsLoaded() && "DLL must not be loaded when assigning a new path!");
        if (IsLoaded())
            Unload();

        _path = path;

        // Append DLL extension if not provided
        if (_path.extension() != DLL::extension())
            _path /= DLL::extension();
    }

    void Load()
    {
        assert(!IsLoaded() && "DLL is already loaded!");
        if (IsLoaded())
            return;

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        _dll = dlopen(_path.string().c_str(), RTLD_NOW);
        if (_dll == nullptr)
            throwex DLLException("Cannot load DLL file: {}!"_format(dlerror())).Attach(_path);
#elif defined(_WIN32) || defined(_WIN64)
        _dll = LoadLibraryExW(_path.wstring().c_str(), nullptr, 0);
        if (_dll == nullptr)
            throwex DLLException("Cannot load DLL file!").Attach(_path);
#endif
    }

    void Load(const Path& path)
    {
        Assign(path);
        Load();
    }

    void Unload()
    {
        assert(IsLoaded() && "DLL is not loaded!");
        if (!IsLoaded())
            return;

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = dlclose(_dll);
        if (result != 0)
            throwex DLLException("Cannot unload DLL file: {}!"_format(dlerror())).Attach(_path);
#elif defined(_WIN32) || defined(_WIN64)
        if (!FreeLibrary(_dll))
            throwex DLLException("Cannot unload DLL file!").Attach(_path);
#endif
        _dll = nullptr;
    }

    void* Resolve(const std::string& name)
    {
        assert(IsLoaded() && "DLL is not loaded!");
        if (!IsLoaded())
            return nullptr;

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return dlsym(_dll, name.c_str());
#elif defined(_WIN32) || defined(_WIN64)
        return GetProcAddress(_dll, name.c_str());
#endif
    }

private:
    Path _path;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    void* _dll;
#elif defined(_WIN32) || defined(_WIN64)
    HMODULE _dll;
#endif
};

//! @endcond

DLL::DLL() : _pimpl(std::make_unique<Impl>())
{
}

DLL::DLL(const Path& path) : _pimpl(std::make_unique<Impl>())
{
    _pimpl->Assign(path);
}

DLL::DLL(const DLL& dll) : _pimpl(std::make_unique<Impl>())
{
    _pimpl->Assign(dll.path());
}

DLL::DLL(DLL&& dll) noexcept : _pimpl(std::move(dll._pimpl))
{
}

DLL::~DLL()
{
}

DLL& DLL::operator=(const Path& path)
{
    _pimpl->Assign(path);
    return *this;
}

DLL& DLL::operator=(const DLL& dll)
{
    _pimpl->Assign(dll.path());
    return *this;
}

DLL& DLL::operator=(DLL&& dll) noexcept
{
    _pimpl = std::move(dll._pimpl);
    return *this;
}

const Path DLL::path() const
{
    return _pimpl->path();
}

bool DLL::IsLoaded() const
{
    return _pimpl->IsLoaded();
}

bool DLL::IsResolving(const std::string& name)
{
    return _pimpl->IsResolving(name);
}

void DLL::Load()
{
    _pimpl->Load();
}

void DLL::Load(const Path& path)
{
    _pimpl->Load(path);
}

void DLL::Unload()
{
    _pimpl->Unload();
}

void* DLL::Resolve(const std::string& name)
{
    return _pimpl->Resolve(name);
}

} // namespace CppCommon
