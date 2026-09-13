#include "libidk/platform/PlatformContext.hpp"
#include "libidk/platform/SDL3Events.hpp"
#include "libidk/platform/SDL3FileSystem.hpp"
#include "libidk/platform/SDL3Input.hpp"
#include "libidk/platform/SDL3Time.hpp"
#include "libidk/platform/SDL3Video.hpp"

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

    idk::PlatformContext ctx;
    ctx.giveFeature<idk::SDL3Events>();
    ctx.giveFeature<idk::SDL3Time>();
    ctx.giveFeature<idk::SDL3FileSystem>();
    ctx.giveFeature<idk::SDL3Video>("GameWindow", 1280, 720);

    while (ctx.running())
    {
        ctx.update();
    }

    return 0;
}
