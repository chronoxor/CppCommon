/*!
    \file errors_exceptions_handler.cpp
    \brief Exceptions handler example
    \author Ivan Shynkarenka
    \date 22.04.2016
    \copyright MIT License
*/

#include "errors/exceptions_handler.h"
#include "system/stack_trace_manager.h"
#include "threads/thread.h"

#include <cfloat>
#include <csignal>
#include <exception>
#include <iostream>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

void GenerateSIGABRT()
{
    abort();
}

void GenerateSIGFPE()
{
    // Code taken from http://www.devx.com/cplus/Article/34993/1954

#if defined(_MSC_VER)
    // Set the x86 floating-point control word according to what
    // exceptions you want to trap. Always call _clearfp before
    // setting the control word
    _clearfp();

    // Because the second parameter in the following call is 0, it
    // only returns the floating-point control word.
    unsigned int cw;
    _controlfp_s(&cw, 0, 0);

    // Set the exception masks off for exceptions that you want to
    // trap. When a mask bit is set, the corresponding floating-point
    // exception is blocked from being generating.
    cw &= ~(EM_OVERFLOW | EM_UNDERFLOW | EM_ZERODIVIDE | EM_DENORMAL | EM_INVALID);

    // For any bit in the second parameter (mask) that is 1, the
    // corresponding bit in the first parameter is used to update
    // the control word.
    unsigned int cwOriginal;
    _controlfp_s(&cwOriginal, cw, MCW_EM);

    // MCW_EM is defined in float.h.
    // Restore the original value when done:
    // _controlfp(cwOriginal, MCW_EM);
#endif

    // Divide by zero
    int a = 1;
    int b = 0;
    int c = a / b;
    a = c;
}

void GenerateSIGILL()
{
    raise(SIGILL);
}

void GenerateSIGINT()
{
    raise(SIGINT);
}

void GenerateSIGSEGV()
{
    raise(SIGSEGV);
}

void GenerateSIGTERM()
{
    raise(SIGTERM);
}

void GenerateExit()
{
    exit(0);
}

void GenerateTerminate()
{
    std::terminate();
}

void GenerateExceptionThrow()
{
    throw std::exception();
}

#if !defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4717) // C4717: 'function' : recursive on all control paths, function will cause runtime stack overflow
#endif
void GenerateRecursiveAlloc()
{
    [[maybe_unused]] uint8_t* buffer = new uint8_t[0x1FFFFFFF];
    GenerateRecursiveAlloc();
}
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

class Base;

void function(Base* instance);

class Base
{
public:
    Base() { function(this); }
    virtual void method(void) = 0;
};

class Derived : public Base
{
public:
    void method(void) override {}
};

void function(Base* instance)
{
    instance->method();
}

void GeneratePureVirtualMethodCall()
{
    Derived derived;
}

void GenerateSEH()
{
    int* p = nullptr;
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 6011) // C6011: dereferencing NULL pointer <name>
#endif
    *p = 0;
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
}

void GenerateRaiseException()
{
    RaiseException(123, EXCEPTION_NONCONTINUABLE, 0, nullptr);
}

#if defined(_MSC_VER)
void GenerateInvalidParameter()
{
    char* format = nullptr;
    printf(format);
}
#endif

#endif

void GenerateCustomException(int type)
{
    switch (type)
    {
        case 1:
            GenerateSIGABRT();
            break;
        case 2:
            GenerateSIGFPE();
            break;
        case 3:
            GenerateSIGILL();
            break;
        case 4:
            GenerateSIGINT();
            break;
        case 5:
            GenerateSIGSEGV();
            break;
        case 6:
            GenerateSIGTERM();
            break;
        case 7:
            GenerateExit();
            break;
        case 8:
            GenerateTerminate();
            break;
        case 9:
            GenerateExceptionThrow();
            break;
#if !defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))
        case 10:
            GenerateRecursiveAlloc();
            break;
        case 11:
            GeneratePureVirtualMethodCall();
            break;
        case 12:
            GenerateSEH();
            break;
        case 13:
            GenerateRaiseException();
            break;
#if defined(_MSC_VER)
        case 14:
            GenerateInvalidParameter();
            break;
#endif
#endif
    }
}

int main(int argc, char** argv)
{
    // Generate all exceptions from separate thread
    const bool thread = true;

    // Initialize stack trace manager of the current process
    CppCommon::StackTraceManager::Initialize();
    // Setup exceptions handler for the current process
    CppCommon::ExceptionsHandler::SetupProcess();

    std::cout << "1 - SIGABRT" << std::endl;
    std::cout << "2 - SIGFPE" << std::endl;
    std::cout << "3 - SIGILL" << std::endl;
    std::cout << "4 - SIGINT" << std::endl;
    std::cout << "5 - SIGSEGV" << std::endl;
    std::cout << "6 - SIGTERM" << std::endl;
    std::cout << "7 - exit" << std::endl;
    std::cout << "8 - terminate" << std::endl;
    std::cout << "9 - exception throw" << std::endl;
#if !defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))
    std::cout << "10 - new operator fault" << std::endl;
    std::cout << "11 - pure virtual method call" << std::endl;
    std::cout << "12 - SEH exception" << std::endl;
    std::cout << "13 - RaiseException()" << std::endl;
#if defined(_MSC_VER)
    std::cout << "14 - invalid parameter" << std::endl;
#endif
#endif
    std::cout << "Choose an exception type: ";

    std::string line;
    getline(std::cin, line);
    int type = std::stoi(line);

    // Generate selected exception from separate or main thread
    if (thread)
        CppCommon::Thread::Start(GenerateCustomException, type).join();
    else
        GenerateCustomException(type);

    // Cleanup stack trace manager of the current process
    CppCommon::StackTraceManager::Cleanup();

    return 0;
}
