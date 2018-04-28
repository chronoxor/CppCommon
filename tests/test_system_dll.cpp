//
// Created by Ivan Shynkarenka on 08.06.2017
//

#include "test.h"

#include "system/dll.h"

// Plugins definitions
#include "interface/interface.h"

using namespace CppCommon;

TEST_CASE("DLL plugin function", "[CppCommon][System]")
{
    DLL plugin;
    REQUIRE(!plugin);
    REQUIRE(!plugin.IsLoaded());

    // Load the plugin
    plugin.Load("plugin-function");
    REQUIRE(plugin);
    REQUIRE(plugin.IsLoaded());

    // Resolve the plugin function
    REQUIRE(plugin.IsResolve("PluginRandom"));
    auto random = plugin.Resolve<int ()>("PluginRandom");
    REQUIRE(random != nullptr);

    // Call the plugin function
    for (int i = 0; i < 10; ++i)
        REQUIRE(random() >= 0);

    // Unload the plugin
    plugin.Unload();
    REQUIRE(!plugin);
    REQUIRE(!plugin.IsLoaded());
}

TEST_CASE("DLL plugin interface", "[CppCommon][System]")
{
    // Load the plugin
    DLL plugin("plugin-interface");
    REQUIRE(plugin);
    REQUIRE(plugin.IsLoaded());

    // Resolve the plugin create & release interface functions
    REQUIRE(plugin.IsResolve("PluginRandomCreate"));
    auto create = plugin.Resolve<bool (IRandom**)>("PluginRandomCreate");
    REQUIRE(create != nullptr);
    REQUIRE(plugin.IsResolve("PluginRandomRelease"));
    auto release = plugin.Resolve<bool (IRandom*)>("PluginRandomRelease");
    REQUIRE(release != nullptr);

    // Create the plugin interface
    IRandom *pRandom = nullptr;
    REQUIRE(create(&pRandom));

    // Call the plugin interface method
    for (int i = 0; i < 10; ++i)
        REQUIRE(pRandom->random() >= 0);

    // Release the plugin interface
    REQUIRE(release(pRandom));

    // Unload the plugin
    plugin.Unload();
    REQUIRE(!plugin);
    REQUIRE(!plugin.IsLoaded());
}
