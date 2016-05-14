/*!
    \file thread.inl
    \brief Thread abstraction inline implementation
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template <class Fn, class... Args>
inline std::thread Thread::Start(Fn&& fn, Args&&... args)
{
    return std::thread([=]()
    {
        // Setup exception handler for the new thread
        ExceptionsHandler::SetupThread();

        // Call the base thread function
        fn(args...);
    });
}

} // namespace CppCommon
