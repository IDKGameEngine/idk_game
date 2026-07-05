#include "idk_engine/Engine.hpp"
#include "idk_engine/EngineMsg.hpp"
#include "idk_engine/PlatformService.hpp"
#include "idk_engine/NetService.hpp"
#include "idk_engine/WorldService.hpp"
#include "idk/GameService.hpp"
#include "idk/GfxService.hpp"

#include "idk_engine/EntityManager.hpp"
#include "idk/message/EventChannel.hpp"

// #include "idk/core/message/LocalPort.hpp"
// static idk::message::LocalPortRegion<idk::gfx::MeshDescriptor> TEST_PORT_REGION;
// using TestPortAddress = idk::message::LocalPortAddress<idk::gfx::MeshDescriptor, &TEST_PORT_REGION>;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    static idk::EventChannel<idk::engine::EntityEvent> entityEventBus;
    idk::EngineMsg::get() = {
        .entityEventTxer = &entityEventBus,
        .otherEventTxer = nullptr
    };

    static idk::engine::PlatformService platSrv(false);
    static idk::engine::NetService netSrv;
    static idk::engine::WorldService worldSrv;
    static idk::GfxService gfxSrv(&platSrv);
    static idk::GameService gameSrv;
    static idk::Engine engine({&platSrv, &netSrv, &worldSrv, &gameSrv, &gfxSrv});

    { netSrv.startGameClient(); }

    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
