#include "idk/engine.hpp"
#include "idk/engine/world.hpp"
#include "idk/GameService.hpp"
#include "idk/GfxService.hpp"
#include "idk/IoService.hpp"
#include "idk/UdpClientService.hpp"

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
    static idk::UdpClientService udpClientSrv;
    static idk::Engine engine({&gameSrv, &gfxSrv, &ioSrv, &udpClientSrv});

    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
