#include "idk_engine/Engine.hpp"
#include "idk_engine/PlatformService.hpp"
#include "idk/GameService.hpp"
#include "idk/GfxService.hpp"

// #include "libidk/message/LocalPort.hpp"
// static idk::message::LocalPortRegion<idk::gfx::MeshDescriptor> TEST_PORT_REGION;
// using TestPortAddress = idk::message::LocalPortAddress<idk::gfx::MeshDescriptor, &TEST_PORT_REGION>;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    static idk::engine::PlatformService platSrv(false);
    static idk::GfxService gfxSrv(&platSrv);
    static idk::GameService gameSrv;
    static idk::Engine engine({&platSrv, &gameSrv, &gfxSrv});

    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
