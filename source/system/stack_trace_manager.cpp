/*!
    \file stack_trace_manager.cpp
    \brief Stack trace manager implementation
    \author Ivan Shynkarenka
    \date 10.02.2016
    \copyright MIT License
*/

#include "system/stack_trace_manager.h"

#include "utility/validate_aligned_storage.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#if defined(DBGHELP_SUPPORT)
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4091) // C4091: 'keyword' : ignored on left of 'type' when no variable is declared
#endif
#include <dbghelp.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#endif
#endif

namespace CppCommon {

//! @cond INTERNALS

class StackTraceManager::Impl
{
public:
    Impl() : _initialized(false) {}

    void Initialize()
    {
        // Check for double initialization
        if (_initialized)
            return;

#if defined(_WIN32) || defined(_WIN64)
#if defined(DBGHELP_SUPPORT)
        // Provide required symbol options
        SymSetOptions(SYMOPT_PUBLICS_ONLY);

        // Get the current process handle
        HANDLE hProcess = GetCurrentProcess();

        // Initializes symbol handler for the current process
        if (!InitializeSymbols(hProcess))
            throwex SystemException("Cannot initialize symbol handler for the current process!");
#endif
#endif

        _initialized = true;
    }

    void Cleanup()
    {
        // Check for double cleanup
        if (!_initialized)
            return;

#if defined(_WIN32) || defined(_WIN64)
#if defined(DBGHELP_SUPPORT)
        // Get the current process handle
        HANDLE hProcess = GetCurrentProcess();

        // Cleanup symbol handler for the current process
        if (!SymCleanup(hProcess))
            throwex SystemException("Cannot cleanup symbol handler for the current process!");
#endif
#endif

        _initialized = false;
    }

private:
    bool _initialized;

#if defined(_WIN32) || defined(_WIN64)
#if defined(DBGHELP_SUPPORT)
    bool InitializeSymbols(HANDLE hProcess)
    {
        const int attempts = 10;
        const int sleep = 100;
        for (int attempt = 0; attempt < attempts; ++attempt)
        {
            if (SymInitialize(hProcess, nullptr, TRUE))
                return true;

            Sleep(sleep);
        }
        return false;
    }
#endif
#endif
};

//! @endcond

StackTraceManager::StackTraceManager()
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "StackTraceManager::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "StackTraceManager::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl();
}

StackTraceManager::~StackTraceManager()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

void StackTraceManager::Initialize() { GetInstance().impl().Initialize(); }
void StackTraceManager::Cleanup() { GetInstance().impl().Cleanup(); }

} // namespace CppCommon
