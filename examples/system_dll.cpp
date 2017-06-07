/*!
    \file system_dll.cpp
    \brief Dynamic link library example
    \author Ivan Shynkarenka
    \date 08.06.2017
    \copyright MIT License
*/

#include "system/dll.h"

#include <iostream>

int main(int argc, char** argv)
{
    // Load a sample plugin
    CppCommon::DLL plugin("sample");
    if (plugin)
    {
        // Resolve a plugin function
        auto function = (size_t (*)(const char*))plugin.Resolve("PluginFunction");

        // Call the plugin function
        function("test");
    }
    return 0;
}
