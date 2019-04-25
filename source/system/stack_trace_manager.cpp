/*!
    \file stack_trace_manager.cpp
    \brief Stack trace manager implementation
    \author Ivan Shynkarenka
    \date 10.02.2016
    \copyright MIT License
*/

#include "system/stack_trace_manager.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#if defined(DBGHELP_SUPPORT)
#include <dbghelp.h>
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

            printf("Fixed!\n");

            Sleep(sleep);
        }
        return false;
    }
#endif
#endif
};

//! @endcond

StackTraceManager::StackTraceManager() : _pimpl(std::make_unique<Impl>())
{
}

StackTraceManager::~StackTraceManager()
{
}

void StackTraceManager::Initialize()
{
    GetInstance()._pimpl->Initialize();
}

void StackTraceManager::Cleanup()
{
    GetInstance()._pimpl->Cleanup();
}

} // namespace CppCommon
