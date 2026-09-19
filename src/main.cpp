#include "libidk/log.hpp"
#include "libidk/platform-sdl3/PlatformContext.hpp"
#include "libidk/platform/IAudioBackend.hpp"
#include "idk/engine/Engine.hpp"


int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    idk::PlatformConfig cfg{"GameWindow", 1280, 720};
    idk::PlatformContextSdl3 ctx(cfg);

    idk::Engine engine(&ctx);

    auto *audio = ctx.getFeature<idk::IAudioBackend>();
    IDK_ASSERT(audio != nullptr, "RUH ROH");
    if (audio)
    {
        auto *snd = audio->createSound("audio/hurt3.wav");
        audio->startSound(snd);
    }

    engine.start();

    return 0;
}
