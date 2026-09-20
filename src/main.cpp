#include "idk/engine/Engine.hpp"
#include "idk/platform/PlatformContext.hpp"
#include "idk/gfx/GfxService.hpp"
#include "GameService.hpp"

int main(int argc, char **argv)
{
    (void)argc; (void)argv;

    idk::PlatformConfig cfg{"GameWindow", 1280, 720};
    idk::PlatformContextSdl3 ctx(cfg);

    idk::Engine engine(&ctx);
    engine.addService<idk::gfx::GfxService>();
    engine.addService<idk::GameService>();
    engine.start();

    return 0;
}

