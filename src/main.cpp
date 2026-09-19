#include "libidk/log.hpp"

#include "libidk/platform/PlatformContextSdl3.hpp"

// #include "Sdl3ContextFactory.hpp"
// int main()
// {
//     auto platform = idk::platform::createSdlPlatform({
//         .windowTitle = "GameWindow",
//         .windowWidth = 1280,
//         .windowHeight = 720
//     });

//     while (platform->running())
//     {
//         platform->update();
//     }

//     return 0;
// }


#include <steam/steam_api.h>

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

    std::srand(clock());

    idk::PlatformConfig cfg{"GameWindow", 1280, 720};
    idk::PlatformContextSdl3 ctx(cfg);

    while (ctx.running())
    {
        ctx.update();
    }

    return 0;
}
