#include "idk_engine/Engine.hpp"
#include "idk/GameService.hpp"
#include "idk/GfxService.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    static idk::platform::Platform plat;
    static idk::GfxService gfxSrv(plat);
    static idk::GameService gameSrv;
    static idk::Engine engine({&gfxSrv, &gameSrv});

    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
