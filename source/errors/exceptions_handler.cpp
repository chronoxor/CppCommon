/*!
    \file exceptions_handler.cpp
    \brief Exceptions handler implementation
    \author Ivan Shynkarenka
    \date 22.04.2016
    \copyright MIT License
*/

#include "errors/exceptions_handler.h"

#include "errors/exceptions.h"
#include "system/timestamp.h"

#include <cwchar>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#if defined(DBGHELP_SUPPORT)
#include <DbgHelp.h>
#endif
#elif defined(unix) || defined(__unix) || defined(__unix__)

#endif

namespace CppCommon {

class ExceptionsHandler::Impl
{
public:
    Impl() : _initialized(false) {}

    void SetupProcess()
    {
        // Check for double initialization
        if (_initialized)
            return;

#if defined(_WIN32) || defined(_WIN64)

#elif defined(unix) || defined(__unix) || defined(__unix__)

#endif

        _initialized = true;
    }

    void SetupThread()
    {
#if defined(_WIN32) || defined(_WIN64)

#elif defined(unix) || defined(__unix) || defined(__unix__)

#endif
    }

private:
    bool _initialized;

#if defined(_WIN32) || defined(_WIN64)
    void  CreateDumpFile(EXCEPTION_POINTERS* pExcPtrs)
    {
#if defined(DBGHELP_SUPPORT)
        // Get the current module path
        wchar_t path[4096];
        if (GetModuleFileNameW(nullptr, path, sizeof(path) / sizeof(path[0])) == 0)
            throwex SystemException("Cannot get the current module filename!");
        *(std::wcsrchr(path, '\\') + 1) = '\0';

        // Generate dump file name based on the current timestamp
        std::wstring filename = std::wstring(path) + L"crash-" + std::to_wstring(timestamp()) + L".dmp";

        // Create the dump file
        HANDLE hFile = CreateFileW(filename.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hFile == INVALID_HANDLE_VALUE)
            throwex SystemException("Cannot create the dump file - " + filename);

        MINIDUMP_EXCEPTION_INFORMATION mei;
        MINIDUMP_CALLBACK_INFORMATION mci;

        // Prepare dump file information
        mei.ThreadId = GetCurrentThreadId();
        mei.ExceptionPointers = pExcPtrs;
        mei.ClientPointers = FALSE;
        mci.CallbackRoutine = nullptr;
        mci.CallbackParam = nullptr;
#endif
    }
#endif
};

std::unique_ptr<ExceptionsHandler::Impl> ExceptionsHandler::_pimpl(new Impl());

void ExceptionsHandler::SetupProcess()
{
    _pimpl->SetupProcess();
}

void ExceptionsHandler::SetupThread()
{
    _pimpl->SetupThread();
}

} // namespace CppCommon
