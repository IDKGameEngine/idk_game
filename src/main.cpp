#include "libidk/platform/AudioBackend.hpp"
#include "libidk/platform/EventBackend.hpp"
#include "libidk/platform/FilesystemBackend.hpp"
#include "libidk/platform/InputBackend.hpp"
#include "libidk/platform/TimeBackend.hpp"
#include "libidk/platform/VideoBackend.hpp"

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

    idk::IPlatformContext ctx;
    ctx.giveFeature<idk::EventBackend>();
    ctx.giveFeature<idk::TimeBackend>();
    ctx.giveFeature<idk::FilesystemBackend>();
    ctx.giveFeature<idk::VideoBackend>("GameWindow", 1280, 720);

    while (ctx.running())
    {
        ctx.update();
    }

    return 0;
}
