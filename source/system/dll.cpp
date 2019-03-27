/*!
    \file dll.cpp
    \brief Dynamic link library implementation
    \author Ivan Shynkarenka
    \date 07.06.2017
    \copyright MIT License
*/

#include "system/dll.h"

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
        catch (const DLLException& ex)
        {
            fatality(DLLException(ex.string()).Attach(_path));
        }
    }

    const Path path() const { return _path; }

    bool IsLoaded() const
    {
        return (_dll != nullptr);
    }

    bool IsResolve(const std::string& name) const
    {
        assert(IsLoaded() && "DLL must be loaded!");
        if (!IsLoaded())
            return false;

        return ResolveAddress(name) != nullptr;
    }

    void Assign(const Path& path)
    {
        assert(!IsLoaded() && "DLL must not be loaded when assigning a new path!");
        if (IsLoaded())
            Unload();

        _path = path;

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        // Concatenate DLL executable path
        if (_path.IsRelative())
            _path = Path::executable().parent().Append(_path);
#endif

        // Concatenate DLL prefix if not provided
        std::string filename = _path.filename().string();
        std::string prefix = DLL::prefix();
        if (std::strncmp(filename.c_str(), prefix.c_str(), prefix.size()) != 0)
            _path.ReplaceFilename(prefix + filename);

        // Concatenate DLL extension if not provided
        if (_path.extension() != DLL::extension())
            _path.Concat(DLL::extension());
    }

    bool Load()
    {
        assert(!IsLoaded() && "DLL is already loaded!");
        if (IsLoaded())
            Unload();

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        _dll = dlopen(_path.string().c_str(), RTLD_NOW);
#elif defined(_WIN32) || defined(_WIN64)
        _dll = LoadLibraryExW(_path.wstring().c_str(), nullptr, 0);
#endif
        return (_dll != nullptr);
    }

    bool Load(const Path& path)
    {
        Assign(path);
        return Load();
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

    void* ResolveAddress(const std::string& name) const
    {
        assert(IsLoaded() && "DLL is not loaded!");
        if (!IsLoaded())
            return nullptr;

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return dlsym(_dll, name.c_str());
#elif defined(_WIN32) || defined(_WIN64)
        return (void*)GetProcAddress(_dll, name.c_str());
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

DLL::DLL(const Path& path, bool load) : _pimpl(std::make_unique<Impl>())
{
    _pimpl->Assign(path);

    if (load)
        _pimpl->Load();
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

bool DLL::IsResolve(const std::string& name) const
{
    return _pimpl->IsResolve(name);
}

bool DLL::Load()
{
    return _pimpl->Load();
}

bool DLL::Load(const Path& path)
{
    return _pimpl->Load(path);
}

void DLL::Unload()
{
    _pimpl->Unload();
}

void* DLL::ResolveAddress(const std::string& name) const
{
    return _pimpl->ResolveAddress(name);
}

} // namespace CppCommon
