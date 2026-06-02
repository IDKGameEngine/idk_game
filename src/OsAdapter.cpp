#include "idk/OsAdapter.hpp"

#include "idk/core/cfgparser.hpp"
#include "idk/core/file.hpp"
#include "idk/core/threadpool.hpp"
#include "idk/engine.hpp"
#include "idk/game_service.hpp"
#include "idk/gfx_service.hpp"
#include "idk/io_service.hpp"

static uint8_t gfxSrvBuf[sizeof(idk::GfxService)];
static uint8_t ioSrvBuf[sizeof(idk::IoService)];
static uint8_t gameSrvBuf[sizeof(idk::GameService)];

int idk::OsAdapter::AppMain(int argc, char **argv)
{
    VLOG_INFO("[idk::OsAdapter::AppMain]");

    (void)argc;
    (void)argv;

    static idk::core::ThreadPool<4> threadpool;

    idk::CfgParser cfg("asset/gfx.cfg");
    cfg.print();

    auto *gfx_srv =
        new (gfxSrvBuf) idk::GfxService({"A Game Probably", 1920, 1080});
    auto *io_srv = new (ioSrvBuf) idk::IoService();
    auto *game_srv = new (gameSrvBuf) idk::GameService(idk::GfxApi(gfx_srv));

    idk::Engine engine({gfx_srv, io_srv}, {game_srv});

    // idk::MMapFile file("asset/noise/voronoi.jpg");
    // printf("[MMapFile] base=0x%p, size=%lu\n", file.base, file.size);

    return 0;
}
