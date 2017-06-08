/*!
    \file system_dll.cpp
    \brief Dynamic link library example
    \author Ivan Shynkarenka
    \date 08.06.2017
    \copyright MIT License
*/

#include "system/dll.h"

// Plugins definitions
#include "interface/interface.h"

#include <iostream>

int main(int argc, char** argv)
{
    // Load the plugin
    CppCommon::DLL plugin("plugin-function");
    if (plugin)
    {
        // Resolve the plugin function
        auto random = plugin.Resolve<int ()>("PluginRandom");
        if (random != nullptr)
        {
            // Call the plugin function
            for (int i = 0; i < 10; ++i)
                std::cout << "random() = " << random() << std::endl;
        }

        // Unload the plugin
        plugin.Unload();
    }

    // Load the plugin
    plugin.Load("plugin-interface");
    if (plugin)
    {
        // Resolve the plugin create & release interface functions
        auto create = plugin.Resolve<bool (IRandom**)>("PluginRandomCreate");
        auto release = plugin.Resolve<bool (IRandom*)>("PluginRandomRelease");
        if ((create != nullptr) && (release != nullptr))
        {
            // Create the plugin interface
            IRandom* pRandom;
            if (create(&pRandom))
            {
                // Call the plugin interface method
                for (int i = 0; i < 10; ++i)
                    std::cout << "Random.random() = " << pRandom->random() << std::endl;

                // Release the plugin interface
                release(pRandom);
            }
        }

        // Unload the plugin
        plugin.Unload();
    }

    return 0;
}
