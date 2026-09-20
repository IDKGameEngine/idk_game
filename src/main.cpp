#include "idk/engine/Engine.hpp"
#include "idk/platform/PlatformContext.hpp"
#include "Game.hpp"


int main(int argc, char **argv)
{
    (void)argc; (void)argv;

    idk::PlatformConfig cfg{"GameWindow", 1280, 720};
    idk::PlatformContextSdl3 ctx(cfg);
    idk::Engine engine(&ctx);

    mygame::Game game;
    engine.run(&game);

    return 0;
}
