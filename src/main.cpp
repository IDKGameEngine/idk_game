#include "libidk/log.hpp"
#include "libidk/platform-sdl3/PlatformContext.hpp"
#include "libidk/platform/IAudioBackend.hpp"
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

    // auto *audio = ctx.getFeature<idk::IAudioBackend>();
    // IDK_ASSERT(audio != nullptr, "RUH ROH");
    // if (audio)
    // {
    //     audio->createSound("gfx/font/atlas.png");
    // }

    while (ctx.running())
    {
        ctx.update();
    }

    return 0;
}
