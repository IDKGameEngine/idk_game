#include "idk_engine/Engine.hpp"
#include "idk_engine/PlatformService.hpp"
#include "idk_engine/NetService.hpp"

int main(int argc, char **argv)
{
    (void)argc; (void)argv;

    std::srand(clock());

    static idk::engine::PlatformService platSrv(true);
    static idk::engine::NetService netSrv;
    static idk::Engine engine({&platSrv, &netSrv});

    netSrv.startGameServer();
    
    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
