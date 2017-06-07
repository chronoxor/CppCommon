/*!
    \file function.cpp
    \brief Sample plugin function
    \author Ivan Shynkarenka
    \date 07.06.2017
    \copyright MIT License
*/

#include "system/dll.h"

#include <cstring>
#include <iostream>

extern "C" EXPORT size_t PluginFunction(const char* str)
{
    std::cout << "PluginFunction(" << str << ")" << std::endl;
    return std::strlen(str);
}
