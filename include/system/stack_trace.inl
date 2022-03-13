/*!
    \file stack_trace.inl
    \brief Stack trace snapshot provider inline implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#if defined(FMT_VERSION)
template <> struct fmt::formatter<CppCommon::StackTrace> : ostream_formatter {};
#endif
