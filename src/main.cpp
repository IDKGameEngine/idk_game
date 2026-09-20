#include "libidk/log.hpp"
#include "libidk/platform-sdl3/PlatformContext.hpp"
#include "idk/engine/Engine.hpp"

#include "idk/gfx/GfxService.hpp"
#include "GameService.hpp"


int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    idk::PlatformConfig cfg{"GameWindow", 1280, 720};
    idk::PlatformContextSdl3 ctx(cfg);

    idk::Engine engine(&ctx);
    engine.addComponent<idk::gfx::GfxService>();
    engine.addComponent<idk::GameService>();
    engine.start();

    return 0;
}
