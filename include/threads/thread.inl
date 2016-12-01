/*!
    \file thread.inl
    \brief Thread inline implementation
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template <class Fn, class... Args>
inline std::thread Thread::Start(Fn&& fn, Args&&... args)
{
    return std::thread([fn = fn, args...]()
    {
        // Setup exception handler for the new thread
        ExceptionsHandler::SetupThread();

        // Call the base thread function
        fn(std::move(args)...);
    });
}

} // namespace CppCommon
