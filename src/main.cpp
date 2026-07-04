#include "idk_engine/Engine.hpp"
#include "idk_engine/EntityService.hpp"
#include "idk_engine/PlatformService.hpp"
#include "idk_engine/NetService.hpp"
#include "idk/GameService.hpp"
#include "idk/GfxService.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    // bool isClient = true;
    bool isServer = (argc==2 && std::string(argv[1])=="--server");

    static idk::engine::PlatformService platSrv(isServer == true);
    static idk::engine::EntityService entitySrv;
    static idk::engine::NetService netSrv;
    static idk::GfxService gfxSrv(&platSrv);
    static idk::GameService gameSrv;
    static idk::Engine engine({&platSrv, &entitySrv, &netSrv, &gameSrv, &gfxSrv});

    if (isServer) { netSrv.startGameServer(); }
    else          { netSrv.startGameClient(); }
    
    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
