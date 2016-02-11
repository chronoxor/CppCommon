/*!
    \file stack_trace.h
    \brief Stack trace snapshot provider definition
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_DEBUG_STACK_TRACE_H
#define CPPCOMMON_DEBUG_STACK_TRACE_H

#include <string>
#include <vector>

//! Current stack trace snapshot macro
/*!
    Capture the current stack trace snapshot.
*/
#define __STACK__ CppCommon::StackTrace(0)

namespace CppCommon {

//! Stack trace snapshot provider
/*!
    Capture the current stack trace snapshot with easy-to-use interface.

    Thread-safe.
*/
class StackTrace
{
    friend class Exception;

public:
    //! Stack trace frame
    struct Frame
    {
        void* address;
        std::string module;
        std::string function;
        std::string filename;
        int line;

        //! Get string from the current stack trace frame
        std::string to_string() const;

        //! Output stack trace frame into the given output stream
        friend std::ostream& operator<<(std::ostream& os, const Frame& instance)
        { os << instance.to_string(); return os; }
    };

public:
    //! Capture the current stack trace snapshot
    /*!
        \param skip - Skip frames count
    */
    explicit StackTrace(int skip);
    StackTrace(const StackTrace&) = default;
    StackTrace(StackTrace&&) = default;
    ~StackTrace() = default;

    StackTrace& operator=(const StackTrace&) = default;
    StackTrace& operator=(StackTrace&&) = default;

    //! Get stack trace frames
    const std::vector<Frame>& frames() const noexcept { return _frames; }

    //! Get string from the current stack trace snapshot
    std::string to_string() const;

    //! Output stack trace into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const StackTrace& instance)
    { os << instance.to_string(); return os; }

private:
    std::vector<Frame> _frames;

    StackTrace() {}
};

} // namespace CppCommon

/*! \example debug_stack_trace.cpp Stack trace snapshot provider example */

#endif // CPPCOMMON_DEBUG_STACK_TRACE_H
