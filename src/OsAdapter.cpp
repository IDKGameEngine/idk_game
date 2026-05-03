#include "idk/OsAdapter.hpp"
#include "idk/engine.hpp"

#include "idk/core/file.hpp"
#include "idk/gfx/renderer.hpp"
#include "idk/game/game.hpp"
#include "idk/gfxapi.hpp"


int idk::OsAdapter::AppMain(int argc, char **argv)
{
    VLOG_INFO("[idk::OsAdapter::AppMain]");

    (void)argc;
    (void)argv;

    auto *ren  = new idk::gfx::RenderEngine({"A Game Probably", 1024, 1024});
    auto *game = new idk::Game(idk::GfxApi(ren));

    idk::Engine engine(ren, {game});

    // idk::MMapFile file("asset/noise/voronoi.jpg");
    // printf("[MMapFile] base=0x%p, size=%lu\n", file.base, file.size);

    return 0;
}

