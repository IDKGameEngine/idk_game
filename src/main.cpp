#include "idk_engine/Engine.hpp"
#include "idk_engine/EntityService.hpp"
#include "idk_engine/PlatformService.hpp"
#include "idk_engine/NetService.hpp"
#include "idk/GameService.hpp"
#include "idk/GfxService.hpp"

// #include "idk/core/message/LocalPort.hpp"
// static idk::message::LocalPortRegion<idk::gfx::MeshDescriptor> TEST_PORT_REGION;
// using TestPortAddress = idk::message::LocalPortAddress<idk::gfx::MeshDescriptor, &TEST_PORT_REGION>;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    static idk::engine::PlatformService platSrv(false);
    static idk::engine::EntityService entitySrv;
    static idk::engine::NetService netSrv;
    static idk::GfxService gfxSrv(&platSrv);
    static idk::GameService gameSrv;
    static idk::Engine engine({&platSrv, &entitySrv, &netSrv, &gameSrv, &gfxSrv});

    { netSrv.startGameClient(); }

    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
