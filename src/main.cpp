#include "idk_engine/engine.hpp"
#include "idk_engine/world.hpp"
#include "idk_engine/IoService.hpp"
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
    static idk::GameService gameSrv(&gfxSrv);
    static idk::IoService ioSrv;
    static idk::Engine engine({&gameSrv, &gfxSrv, &ioSrv});

    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
