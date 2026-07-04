#include "idk_engine/Engine.hpp"
#include "idk_engine/World.hpp"
#include "idk_engine/PlatformService.hpp"
#include "idk_engine/NetService.hpp"
#include "idk/GameService.hpp"
#include "idk/GfxService.hpp"

alignas(max_align_t) static uint8_t platBuf[sizeof(idk::engine::PlatformService)];
static idk::engine::PlatformService *platSrv;


int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    // bool isClient = true;
    bool isServer = (argc==2 && std::string(argv[1])=="--server");

    platSrv = new (platBuf) idk::engine::PlatformService(isServer == true);
    static idk::engine::NetService netSrv;
    static idk::GfxService gfxSrv(platSrv);
    static idk::GameService gameSrv;
    static idk::Engine engine({platSrv, &netSrv, &gameSrv, &gfxSrv});

    if (isServer) { netSrv.startGameServer(); }
    else          { netSrv.startGameClient(); }
    
    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
