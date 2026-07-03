#include "idk/engine.hpp"
#include "idk/engine/world.hpp"
#include "idk/GameService.hpp"
#include "idk/GfxService.hpp"
#include "idk/IoService.hpp"

int main(int argc, char **argv)
{
    (void)argc; (void)argv;

    VLOG_INFO("[main]");
    srand(clock());

    if (!idk::platform::AppInit())
    {
        return 1;
    }

    static idk::GfxService gfxSrv;
    static idk::IoService ioSrv;
    static idk::GameService gameSrv(&gfxSrv);

    idk::Engine engine({&ioSrv, &gameSrv, &gfxSrv});

    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
