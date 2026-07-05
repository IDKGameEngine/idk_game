#include "idk_engine/Engine.hpp"
#include "idk_engine/NetService.hpp"
#include "idk_engine/PlatformService.hpp"
#include "idk_engine/WorldService.hpp"
#include "idk_gfx/GfxService.hpp"
#include "idk_game/GameService.hpp"


int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    static idk::engine::PlatformService platSrv(false);
    static idk::engine::NetService netSrv;
    static idk::engine::WorldService worldSrv;
    static idk::GfxService gfxSrv(&platSrv);
    static idk::GameService gameSrv;
    static idk::Engine engine({&platSrv, &netSrv, &worldSrv, &gameSrv, &gfxSrv});

    netSrv.startGameClient();

    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
