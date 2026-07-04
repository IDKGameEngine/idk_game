#include "idk_engine/Engine.hpp"
#include "idk_engine/World.hpp"
#include "idk/GameService.hpp"
#include "idk/GfxService.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    if (!idk::platform::AppInit())
    {
        return 1;
    }

    static idk::GfxService gfxSrv;
    static idk::GameService gameSrv;
    static idk::Engine engine({&gameSrv, &gfxSrv});

    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
