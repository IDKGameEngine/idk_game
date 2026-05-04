#include "idk/OsAdapter.hpp"

#include "idk/core/cfgparser.hpp"
#include "idk/core/file.hpp"

#include "idk/engine.hpp"

#include "idk/io_service.hpp"
#include "idk/gfx_service.hpp"
#include "idk/game_service.hpp"


int idk::OsAdapter::AppMain(int argc, char **argv)
{
    VLOG_INFO("[idk::OsAdapter::AppMain]");

    (void)argc;
    (void)argv;

    idk::CfgParser cfg("asset/gfx.cfg");
    // cfg.print();
    woop_treenode(cfg.root_);

    auto *gfx_srv  = new idk::GfxService({"A Game Probably", 1024, 1024});
    auto *io_srv   = new idk::IoService();
    auto *game_srv = new idk::GameService(idk::GfxApi(gfx_srv));

    idk::Engine engine({gfx_srv, io_srv}, {game_srv});

    // idk::MMapFile file("asset/noise/voronoi.jpg");
    // printf("[MMapFile] base=0x%p, size=%lu\n", file.base, file.size);

    return 0;
}

