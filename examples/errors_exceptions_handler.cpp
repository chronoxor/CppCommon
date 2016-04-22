/*!
    \file errors_exceptions_handler.cpp
    \brief Exceptions handler example
    \author Ivan Shynkarenka
    \date 22.04.2016
    \copyright MIT License
*/

#include "errors/exceptions_handler.h"

#include <cfloat>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

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

    // Divide by zero
    float a = 1;
    float b = 0;
    float c = a / b;
    c;
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
    terminate();
}

void GenerateUnexpected()
{
    unexpected();
}

void GenerateSEH()
{
    int* p = nullptr;
#pragma warning(push)
// VS 6011: dereferencing NULL pointer <name>
#pragma warning(disable: 6011)
    *p = 0;
#pragma warning(pop)
}

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

void GenerateInvalidParameter()
{
    char* format = nullptr;
    printf(format);
}

#pragma warning(push)
// VS 4717: 'function' : recursive on all control paths, function will cause runtime stack overflow
#pragma warning(disable: 4717)
void GenerateRecurseAlloc()
{
    char* buffer = new char[0x1FFFFFFF];
    buffer;
    GenerateRecurseAlloc();
}
#pragma warning(pop)

void GenerateCppException()
{
    throw std::exception("My exception");
}

#if defined(_WIN32) || defined(_WIN64)
void GenerateRaiseException()
{
    RaiseException(123, EXCEPTION_NONCONTINUABLE, 0, nullptr);
}
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
            GenerateUnexpected();
            break;
        case 10:
            GenerateSEH();
            break;
        case 11:
            GeneratePureVirtualMethodCall();
            break;
        case 12:
            GenerateInvalidParameter();
            break;
        case 13:
            GenerateRecurseAlloc();
            break;
        case 14:
            GenerateCppException();
            break;
#if defined(_WIN32) || defined(_WIN64)
        case 15:
            GenerateRaiseException();
            break;
#endif
    }
}

void GenerateCustomException(int type, bool thread)
{
    auto function = [=]()
    {
        // Setup exceptions handler for the created thread
        if (thread)
            CppCommon::ExceptionsHandler::SetupThread();

        GenerateCustomException(type);
    };

    if (thread)
        std::thread(function).join();
    else
        function();
}

int main(int argc, char** argv)
{
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
    std::cout << "9 - unexpected" << std::endl;
    std::cout << "10 - SEH exception" << std::endl;
    std::cout << "11 - pure virtual method call" << std::endl;
    std::cout << "12 - invalid parameter" << std::endl;
    std::cout << "13 - new operator fault" << std::endl;
    std::cout << "14 - throw C++ exception" << std::endl;
#if defined(_WIN32) || defined(_WIN64)
    std::cout << "15 - RaiseException()" << std::endl;
#endif
    std::cout << "Choose an exception type: ";

    std::string line;
    if (getline(std::cin, line))
    {
        int type = std::stoi(line);
        GenerateCustomException(type, true);
    }

    return 0;
}
