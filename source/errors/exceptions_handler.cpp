/*!
    \file exceptions_handler.cpp
    \brief Exceptions handler implementation
    \author Ivan Shynkarenka
    \date 22.04.2016
    \copyright MIT License
*/

#include "errors/exceptions_handler.h"

#include "errors/exceptions.h"
#include "system/stack_trace.h"
#include "system/timestamp.h"

#include <cstring>
#include <exception>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <new.h>
#include <signal.h>
#include <windows.h>
#if defined(DBGHELP_SUPPORT)
#include <DbgHelp.h>
#endif
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <signal.h>
#include <unistd.h>
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
        // Install top-level SEH handler
        SetUnhandledExceptionFilter(SehHandler);

        // Catch pure virtual function calls
        // Because there is one _purecall_handler for the whole process,
        // calling this function immediately impacts all threads. The last
        // caller on any thread sets the handler.
        // http://msdn.microsoft.com/en-us/library/t296ys27.aspx
        _set_purecall_handler(PureCallHandler);

        // Catch new operator memory allocation exceptions
        _set_new_handler(NewHandler);

        // Catch invalid parameter exceptions
        _set_invalid_parameter_handler(InvalidParameterHandler);

        // Set up C++ signal handlers
        _set_abort_behavior(_CALL_REPORTFAULT, _CALL_REPORTFAULT);

        // Catch an abnormal program termination
        signal(SIGABRT, SigabrtHandler);

        // Catch an illegal instruction error
        signal(SIGINT, SigintHandler);

        // Catch a termination request
        signal(SIGTERM, SigtermHandler);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        // Catch terminate() calls
        _old_terminate_hander = std::set_terminate(TerminateHandler);

        // Catch unexpected() calls
        _old_unexpected_hander = std::set_unexpected(UnexpectedHandler);

        // Prepare signal action structure
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_sigaction = SignalHanlder;
        sa.sa_flags = SA_SIGINFO;

        // Clear old signal handlers
        memset(_old_signal_hanlders, 0, sizeof(_old_signal_hanlders));

        // Define signals to catch
        int signals[] =
        {
            SIGABRT,
            SIGALRM,
            SIGBUS,
            SIGFPE,
            SIGHUP,
            SIGILL,
            SIGINT,
            SIGPIPE,
            SIGPOLL,
            SIGPROF,
            SIGQUIT,
            SIGSEGV,
            SIGSYS,
            SIGTERM,
            SIGXCPU,
            SIGXFSZ
        };

        // Setup corresponding signals handlers
        for (int i = 0; i < sizeof(signals) / sizeof(signals[0]); ++i)
        {
            int result = sigaction(signals[i], &sa, &_old_signal_hanlders[i]);
            if (result != 0)
                throwex SystemException("Failed to setup signal handler - " + std::to_string(signals[i]));
        }
#endif

        _initialized = true;
    }

    void SetupThread()
    {
#if defined(_WIN32) || defined(_WIN64)
        // Catch terminate() calls
        // In a multithreaded environment, terminate functions are maintained
        // separately for each thread. Each new thread needs to install its own
        // terminate function. Thus, each thread is in charge of its own termination handling.
        // http://msdn.microsoft.com/en-us/library/t6fk7h29.aspx
        set_terminate(TerminateHandler);

        // Catch unexpected() calls
        // In a multithreaded environment, unexpected functions are maintained
        // separately for each thread. Each new thread needs to install its own
        // unexpected function. Thus, each thread is in charge of its own unexpected handling.
        // http://msdn.microsoft.com/en-us/library/h46t5b69.aspx
        set_unexpected(UnexpectedHandler);

        // Catch a floating point exception
        typedef void (*sigh)(int);
        signal(SIGFPE, (sigh)SigfpeHandler);

        // Catch an illegal instruction
        signal(SIGILL, SigillHandler);

        // Catch an illegal storage access error
        signal(SIGSEGV, SigsegvHandler);
#endif
    }

private:
    // Initialization flag for the current process
    bool _initialized;

#if defined(_WIN32) || defined(_WIN64)

    // Structured exception handler
    static LONG WINAPI SehHandler(PEXCEPTION_POINTERS pExceptionPtrs)
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Unhandled SEH exception"), StackTrace(1));

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);

        // Unreacheable code
        return EXCEPTION_EXECUTE_HANDLER;
    }

    // CRT terminate() call handler
    static void __cdecl TerminateHandler()
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Abnormal program termination (terminate() function was called)"), StackTrace(1));

        // Retrieve exception information
        EXCEPTION_POINTERS* pExceptionPtrs = nullptr;
        GetExceptionPointers(0, &pExceptionPtrs);

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Delete exception information
        DeleteExceptionPointers(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);
    }

    // CRT unexpected() call handler
    static void __cdecl UnexpectedHandler()
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Unexpected error (unexpected() function was called)"), StackTrace(1));

        // Retrieve exception information
        EXCEPTION_POINTERS* pExceptionPtrs = nullptr;
        GetExceptionPointers(0, &pExceptionPtrs);

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Delete exception information
        DeleteExceptionPointers(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);
    }

    // CRT Pure virtual method call handler
    static void __cdecl PureCallHandler()
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Pure virtual function call"), StackTrace(1));

        // Retrieve exception information
        EXCEPTION_POINTERS* pExceptionPtrs = nullptr;
        GetExceptionPointers(0, &pExceptionPtrs);

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Delete exception information
        DeleteExceptionPointers(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);
    }

    // CRT invalid parameter handler
    static void __cdecl InvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Invalid parameter exception"), StackTrace(1));

        // Retrieve exception information
        EXCEPTION_POINTERS* pExceptionPtrs = nullptr;
        GetExceptionPointers(0, &pExceptionPtrs);

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Delete exception information
        DeleteExceptionPointers(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);
    }

    // CRT new operator fault handler
    static int __cdecl NewHandler(size_t)
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("'new' operator memory allocation exception"), StackTrace(1));

        // Retrieve exception information
        EXCEPTION_POINTERS* pExceptionPtrs = nullptr;
        GetExceptionPointers(0, &pExceptionPtrs);

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Delete exception information
        DeleteExceptionPointers(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);

        // Unreacheable code
        return 0;
    }

    // CRT SIGABRT signal handler
    static void SigabrtHandler(int signum)
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Caught abort (SIGABRT) signal"), StackTrace(1));

        // Retrieve exception information
        EXCEPTION_POINTERS* pExceptionPtrs = nullptr;
        GetExceptionPointers(0, &pExceptionPtrs);

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Delete exception information
        DeleteExceptionPointers(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);
    }

    // CRT SIGFPE signal handler
    static void SigfpeHandler(int signum, int subcode)
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Caught floating point exception (SIGFPE) signal"), StackTrace(1));

        // Retrieve exception information
        EXCEPTION_POINTERS* pExceptionPtrs = (PEXCEPTION_POINTERS)_pxcptinfoptrs;

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);
    }

    // CRT sigill signal handler
    static void SigillHandler(int signum)
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Caught illegal instruction (SIGILL) signal"), StackTrace(1));

        // Retrieve exception information
        EXCEPTION_POINTERS* pExceptionPtrs = nullptr;
        GetExceptionPointers(0, &pExceptionPtrs);

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Delete exception information
        DeleteExceptionPointers(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);
    }

    // CRT sigint signal handler
    static void SigintHandler(int signum)
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Caught interruption (SIGINT) signal"), StackTrace(1));

        // Retrieve exception information
        EXCEPTION_POINTERS* pExceptionPtrs = nullptr;
        GetExceptionPointers(0, &pExceptionPtrs);

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Delete exception information
        DeleteExceptionPointers(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);
    }

    // CRT SIGSEGV signal handler
    static void SigsegvHandler(int signum)
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Caught invalid storage access (SIGSEGV) signal"), StackTrace(1));

        // Retrieve exception information
        EXCEPTION_POINTERS* pExceptionPtrs = nullptr;
        GetExceptionPointers(0, &pExceptionPtrs);

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);
    }

    // CRT SIGTERM signal handler
    static void SigtermHandler(int signum)
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Caught termination request (SIGTERM) signal"), StackTrace(1));

        // Retrieve exception information
        EXCEPTION_POINTERS* pExceptionPtrs = nullptr;
        GetExceptionPointers(0, &pExceptionPtrs);

        // Write dump file
        CreateDumpFile(pExceptionPtrs);

        // Delete exception information
        DeleteExceptionPointers(pExceptionPtrs);

        // Terminate process
        TerminateProcess(GetCurrentProcess(), 1);
    }

    static void GetExceptionPointers(DWORD dwExceptionCode, EXCEPTION_POINTERS** ppExceptionPointers)
    {
        CONTEXT ContextRecord;
        memset(&ContextRecord, 0, sizeof(CONTEXT));

        EXCEPTION_RECORD ExceptionRecord;
        memset(&ExceptionRecord, 0, sizeof(EXCEPTION_RECORD));

#if defined(_M_IX86)
        // On x86, we reserve some extra stack which won't be used.  That is to
        // preserve as much of the call frame as possible when the function with
        // the buffer overrun entered __security_check_cookie with a JMP instead
        // of a CALL, after the calling frame has been released in the epilogue
        // of that function.
        volatile ULONG dw[(sizeof(CONTEXT) + sizeof(EXCEPTION_RECORD)) / sizeof(ULONG)];

        // Save the state in the context record immediately.  Hopefully, since
        // opts are disabled, this will happen without modifying ECX, which has
        // the local cookie which failed the check.
        __asm
        {
            mov dword ptr [ContextRecord.Eax  ], eax
            mov dword ptr [ContextRecord.Ecx  ], ecx
            mov dword ptr [ContextRecord.Edx  ], edx
            mov dword ptr [ContextRecord.Ebx  ], ebx
            mov dword ptr [ContextRecord.Esi  ], esi
            mov dword ptr [ContextRecord.Edi  ], edi
            mov word ptr  [ContextRecord.SegSs], ss
            mov word ptr  [ContextRecord.SegCs], cs
            mov word ptr  [ContextRecord.SegDs], ds
            mov word ptr  [ContextRecord.SegEs], es
            mov word ptr  [ContextRecord.SegFs], fs
            mov word ptr  [ContextRecord.SegGs], gs
            pushfd
            pop [ContextRecord.EFlags]

            // Set the context EBP/EIP/ESP to the values which would be found
            // in the caller to __security_check_cookie.
            mov eax, [ebp]
            mov dword ptr [ContextRecord.Ebp], eax
            mov eax, [ebp + 4]
            mov dword ptr [ContextRecord.Eip], eax
            lea eax, [ebp + 8]
            mov dword ptr [ContextRecord.Esp], eax

            // Make sure the dummy stack space looks referenced.
            mov eax, dword ptr dw
        }

        ContextRecord.ContextFlags       = CONTEXT_CONTROL;
        ExceptionRecord.ExceptionAddress = (PVOID)(ULONG_PTR)ContextRecord.Eip;
#elif defined(_M_X64)
        ULONG64           ControlPc;
        ULONG64           EstablisherFrame;
        ULONG64           ImageBase;
        PRUNTIME_FUNCTION FunctionEntry;
        PVOID             HandlerData;

        RtlCaptureContext(&ContextRecord);

        ControlPc     = ContextRecord.Rip;
        FunctionEntry = RtlLookupFunctionEntry(ControlPc, &ImageBase, nullptr);

        if (FunctionEntry != nullptr)
            RtlVirtualUnwind(UNW_FLAG_NHANDLER, ImageBase, ControlPc, FunctionEntry, &ContextRecord, &HandlerData, &EstablisherFrame, nullptr);

        ContextRecord.Rip = (ULONGLONG)_ReturnAddress();
        ContextRecord.Rsp = (ULONGLONG)_AddressOfReturnAddress() + 8;
        ExceptionRecord.ExceptionAddress = (PVOID)ContextRecord.Rip;
#else
        #error Unsupported architecture
#endif
        ExceptionRecord.ExceptionCode = dwExceptionCode;
        ExceptionRecord.ExceptionAddress = _ReturnAddress();

        CONTEXT* pContextRecord = new CONTEXT;
        memcpy(pContextRecord, &ContextRecord, sizeof(CONTEXT));

        EXCEPTION_RECORD* pExceptionRecord = new EXCEPTION_RECORD;
        memcpy(pExceptionRecord, &ExceptionRecord, sizeof(EXCEPTION_RECORD));

        *ppExceptionPointers = new EXCEPTION_POINTERS;
        (*ppExceptionPointers)->ContextRecord = pContextRecord;
        (*ppExceptionPointers)->ExceptionRecord = pExceptionRecord;
    }

    static void DeleteExceptionPointers(EXCEPTION_POINTERS* pExceptionPointers)
    {
        delete pExceptionPointers->ContextRecord;
        delete pExceptionPointers->ExceptionRecord;
        delete pExceptionPointers;
    }

    static void CreateDumpFile(EXCEPTION_POINTERS* pExcPtrs)
    {
#if defined(DBGHELP_SUPPORT)
        // Get the current module path
        char path[4096];
        if (GetModuleFileNameA(nullptr, path, sizeof(path) / sizeof(path[0])) == 0)
            throwex SystemException("Cannot get the current module filename!");
        *(std::strrchr(path, '\\') + 1) = '\0';

        // Generate dump file name based on the current timestamp
        std::string filename = std::string(path) + "crash-" + std::to_string(timestamp()) + ".dmp";

        // Create the dump file
        HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hFile == INVALID_HANDLE_VALUE)
            throwex SystemException("Cannot create a dump file - " + filename);

        MINIDUMP_EXCEPTION_INFORMATION mei;
        MINIDUMP_CALLBACK_INFORMATION mci;

        // Prepare dump file information
        mei.ThreadId = GetCurrentThreadId();
        mei.ExceptionPointers = pExcPtrs;
        mei.ClientPointers = FALSE;
        mci.CallbackRoutine = nullptr;
        mci.CallbackParam = nullptr;

        // Write dump file information
        if (!MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &mei, nullptr, &mci))
        {
            CloseHandle(hFile);
            throwex SystemException("Cannot write a dump file - " + filename);
        }

        // Close dump file
        if (!CloseHandle(hFile))
            throwex SystemException("Cannot close a dump file - " + filename);
#endif
    }

#elif defined(unix) || defined(__unix) || defined(__unix__)

    static std::terminate_handler  _old_terminate_hander;
    static std::unexpected_handler _old_unexpected_hander;
    static struct sigaction        _old_signal_hanlders[64];

    // terminate() call handler
    static void TerminateHandler()
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Abnormal program termination (terminate() function was called)"), StackTrace(1));

        // Call the old handler or terminate process
        if (_old_terminate_hander != nullptr)
            _old_terminate_hander()
        else
            kill(getpid(), SIGKILL);
    }

    // unexpected() call handler
    static void UnexpectedHandler()
    {
        // Output error
        OutputError(__LOCATION__ + SystemException("Unexpected error (unexpected() function was called)"), StackTrace(1));

        // Call the old handler or terminate process
        if (_old_unexpected_hander != nullptr)
            _old_unexpected_hander()
        else
            kill(getpid(), SIGKILL);
    }

    // Signal handler
    static void SignalHanlder(int signo, siginfo_t* info, void* context)
    {
        // Output error
        switch (signo)
        {
            case SIGABRT:
                OutputError(__LOCATION__ + SystemException("Caught abnormal program termination (SIGABRT) signal"), StackTrace(1));
                break;
            case SIGALRM:
                OutputError(__LOCATION__ + SystemException("Caught alarm clock (SIGALRM) signal"), StackTrace(1));
                break;
            case SIGBUS:
                OutputError(__LOCATION__ + SystemException("Caught memory access error (SIGBUS) signal"), StackTrace(1));
                break;
            case SIGFPE:
                OutputError(__LOCATION__ + SystemException("Caught floating point exception (SIGFPE) signal"), StackTrace(1));
                break;
            case SIGHUP:
                OutputError(__LOCATION__ + SystemException("Caught hangup instruction (SIGHUP) signal"), StackTrace(1));
                break;
            case SIGILL:
                OutputError(__LOCATION__ + SystemException("Caught illegal instruction (SIGILL) signal"), StackTrace(1));
                break;
            case SIGINT:
                OutputError(__LOCATION__ + SystemException("Caught terminal interrupt (SIGINT) signal"), StackTrace(1));
                break;
            case SIGPIPE:
                OutputError(__LOCATION__ + SystemException("Caught pipe write error (SIGPIPE) signal"), StackTrace(1));
                break;
            case SIGPOLL:
                OutputError(__LOCATION__ + SystemException("Caught pollable event (SIGPOLL) signal"), StackTrace(1));
                break;
            case SIGPROF:
                OutputError(__LOCATION__ + SystemException("Caught profiling timer expired error (SIGPROF) signal"), StackTrace(1));
                break;
            case SIGQUIT:
                OutputError(__LOCATION__ + SystemException("Caught terminal quit (SIGQUIT) signal"), StackTrace(1));
                break;
            case SIGSEGV:
                OutputError(__LOCATION__ + SystemException("Caught illegal storage access error (SIGSEGV) signal"), StackTrace(1));
                break;
            case SIGSYS:
                OutputError(__LOCATION__ + SystemException("Caught bad system call (SIGSYS) signal"), StackTrace(1));
                break;
            case SIGTERM:
                OutputError(__LOCATION__ + SystemException("Caught termination request (SIGTERM) signal"), StackTrace(1));
                break;
            case SIGXCPU:
                OutputError(__LOCATION__ + SystemException("Caught CPU time limit exceeded (SIGXCPU) signal"), StackTrace(1));
                break;
            case SIGXFSZ:
                OutputError(__LOCATION__ + SystemException("Caught file size limit exceeded (SIGXFSZ) signal"), StackTrace(1));
                break;
            default:
                OutputError(__LOCATION__ + SystemException("Caught unknown signal - " + std::to_string(signo)), StackTrace(1));
                break;
        }

        // Call the old handler or terminate process
        if (_old_signal_hanlders[signo].sa_sigaction != nullptr)
            _old_signal_hanlders[signo].sa_sigaction(signo, info, context);
        else if (_old_signal_hanlders[signo].sa_handler != nullptr)
            _old_signal_hanlders[signo].sa_handler(signo);
        else
            kill(getpid(), SIGKILL);
    }

#endif

    static void OutputError(const SystemException& exception, const StackTrace& trace)
    {
        std::cerr << exception;
        std::cerr << "Stack trace:" << std::endl;
        std::cerr << trace;
    }
};

std::terminate_handler ExceptionsHandler::Impl::_old_terminate_hander = nullptr;
std::unexpected_handler ExceptionsHandler::Impl::_old_unexpected_hander = nullptr;
struct sigaction ExceptionsHandler::Impl::_old_signal_hanlders[64];

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
