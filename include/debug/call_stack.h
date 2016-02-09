/*!
    \file call_stack.h
    \brief Call stack snapshot provider definition
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_DEBUG_CALL_STACK_H
#define CPPCOMMON_DEBUG_CALL_STACK_H

#include <string>
#include <vector>

//! Current call stack macro
/*!
    Create a new call stack snapshot provider.
*/
#define __CALL_STACK__ CppCommon::CallStack()

namespace CppCommon {

//! Call stack snapshot provider
/*!
    Create a call stack snapshot with easy-to-use interface.

    Thread-safe.
*/
class CallStack
{
public:
    //! Call stack frame
    struct Frame
    {
        void* address;
        std::string module;
        std::string function;
        std::string filename;
        int line;

        //! Get string from the current call stack frame
        std::string to_string() const;

        //! Output call stack frame into the given output stream
        friend std::ostream& operator<<(std::ostream& os, const Frame& instance)
        { os << instance.to_string(); return os; }
    };

public:
    //! Default class constructor
    /*!
        \param skip - Skip frames count (default is 0)
    */
    CallStack(int skip = 0);
    CallStack(const CallStack&) = default;
    CallStack(CallStack&&) = default;
    ~CallStack() = default;

    CallStack& operator=(const CallStack&) = default;
    CallStack& operator=(CallStack&&) = default;

    //! Get call stack frames
    const std::vector<Frame>& frames() const noexcept { return _frames; }

    //! Get string from the current call stack
    std::string to_string() const;

    //! Output call stack into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const CallStack& instance)
    { os << instance.to_string(); return os; }

private:
    std::vector<Frame> _frames;
};

} // namespace CppCommon

/*! \example debug_call_stack.cpp Call stack snapshot provider example */

#endif // CPPCOMMON_DEBUG_SOURCE_LOCATION_H
