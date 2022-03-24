/*!
    \file string_encoding.cpp
    \brief Encoding utilities example
    \author Ivan Shynkarenka
    \date 13.08.2016
    \copyright MIT License
*/

#include "string/encoding.h"

#include <iostream>

#if defined(unix) || defined(__unix) || defined(__unix__)
#include <locale.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#define CP_UTF16 65002
#endif

int main(int argc, char** argv)
{
    // Allow the console to output Unicode strings using std::wcout
#if defined(unix) || defined(__unix) || defined(__unix__)
    setlocale(LC_ALL, "");
#elif defined(_WIN32) || defined(_WIN64)
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
    SetConsoleCP(CP_UTF16);
    SetConsoleOutputCP(CP_UTF16);
#endif

    std::string utf8 = "\xD0\x9F\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82\x20\xD0\xBC\xD0\xB8\xD1\x80\x21";
    std::wstring sys = CppCommon::Encoding::FromUTF8(utf8);
    std::wcout << sys << std::endl;
    return 0;
}
