#include "libidk/platform-sdl3/SdlPlatform.hpp"
#include "libidk/log.hpp"
#include "idk/gfx/VulkanRenderer.hpp"

#include <steam/steam_api.h>

#include <cstdlib>

static void InitSteamLinuxRuntime()
{
    SteamErrMsg errMsg = { 0 };
    if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK)
    {
        VLOG_FATAL("SteamAPI_InitEx() failure: {}", errMsg);
    }
}


int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (std::getenv("SteamEnv"))
    {
        VLOG_INFO("Detected Steam runtime environment");
        InitSteamLinuxRuntime();
    }
    else
    {
        VLOG_INFO("Detected native runtime environment");
    }

    const idk::platform::AppConfig platformConfig {
        .title = "GameWindow",
        .initialWindowSize = { 1280, 720 },
    };
    auto platform = idk::sdl3::makePlatform(platformConfig);
    idk::gfx::VulkanRenderer renderer(platform->mainWindow());

    while (!platform->quitRequested())
    {
        platform->pollEvents();
        renderer.render();
    }

    return 0;
}
