/*!
    \file debug_stack_trace.cpp
    \brief Stack trace snapshot provider example
    \author Ivan Shynkarenka
    \date 11.02.2016
    \copyright MIT License
*/

#include "debug/stack_trace.h"
#include "debug/stack_trace_manager.h"
#include "threads/thread.h"

#include <iostream>
#include <thread>

void function1()
{
    std::cout << "Thread Id: " << __THREAD__ << std::endl;
    std::cout << "Stack trace: " << std::endl << __STACK__ << std::endl;
    CppCommon::Thread::Yield();
}

void function2()
{
    function1();
    CppCommon::Thread::Yield();
}

void function3()
{
    function2();
    CppCommon::Thread::Yield();
}

int main(int argc, char** argv)
{
    // Initialize stack trace manager of the current process
    CppCommon::StackTraceManager::Initialize();

    // Show the stack trace from the main thread
    function3();

    // Show the stack trace from the child thread
    std::thread(function3).join();

    // Cleanup stack trace manager of the current process
    CppCommon::StackTraceManager::Cleanup();

    return 0;
}
