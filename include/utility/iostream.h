/*!
    \file iostream.h
    \brief I/O stream helpers definition
    \author Ivan Shynkarenka
    \date 04.08.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_UTILITY_IOSTREAM_H
#define CPPCOMMON_UTILITY_IOSTREAM_H

#include <iostream>

namespace CppCommon {

//! Write character stream manipulator
class WriteChar
{
public:
    explicit WriteChar(char ch) : _ch(ch) {}

    friend std::ostream& operator<<(std::ostream& stream, const WriteChar& writer)
    { return stream << '\'' << writer._ch << '\''; }
    friend std::wostream& operator<<(std::wostream& stream, const WriteChar& writer)
    { return stream << '\'' << writer._ch << '\''; }

private:
    char _ch;
};

//! Write wide character stream manipulator
class WriteWChar
{
public:
    explicit WriteWChar(wchar_t ch) : _ch(ch) {}

    friend std::ostream& operator<<(std::ostream& stream, const WriteWChar& writer)
    { return stream << '\'' << (char)writer._ch << '\''; }
    friend std::wostream& operator<<(std::wostream& stream, const WriteWChar& writer)
    { return stream << '\'' << writer._ch << '\''; }

private:
    wchar_t _ch;
};

//! Write string stream manipulator
class WriteString
{
public:
    template <size_t N>
    explicit WriteString(const char (&str)[N]) : _str(str), _size(N) {}

    friend std::ostream& operator<<(std::ostream& stream, const WriteString& writer)
    {
        stream << '"';
        stream.write(writer._str, writer._size);
        stream << '"';
        return stream;
    }
    friend std::wostream& operator<<(std::wostream& stream, const WriteString& writer)
    {
        stream << '"';
        for (size_t i = 0; i < writer._size; ++i)
            stream << writer._str[i];
        stream << '"';
        return stream;
    }

private:
    const char* _str;
    size_t _size;
};

//! Write wide string stream manipulator
class WriteWString
{
public:
    template <size_t N>
    explicit WriteWString(const wchar_t (&str)[N]) : _str(str), _size(N) {}

    friend std::ostream& operator<<(std::ostream& stream, const WriteWString& writer)
    {
        stream << '"';
        for (size_t i = 0; i < writer._size; ++i)
            stream << (char)writer._str[i];
        stream << '"';
        return stream;
    }
    friend std::wostream& operator<<(std::wostream& stream, const WriteWString& writer)
    {
        stream << '"';
        stream.write(writer._str, writer._size);
        stream << '"';
        return stream;
    }

private:
    const wchar_t* _str;
    size_t _size;
};

} // namespace CppCommon

#endif // CPPCOMMON_UTILITY_IOSTREAM_H
