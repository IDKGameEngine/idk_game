#include "idk/version.h"
#include "idk/AppEntry.hpp"
#include "idk/AppRuntime.hpp"

#include "idk/core/file.hpp"
#include "idk/engine/engine.hpp"
#include "idk/gfx/renderer.hpp"
#include "idk/game/game.hpp"
#include "idk/idk_gfxapi.hpp"


int idk::AppEntry::AppEntry(int argc, char **argv)
{
    VLOG_INFO("[AppEntry::AppEntry]");

    (void)argc;
    (void)argv;

    auto *ren  = new idk::gfx::RenderEngine({"A Game Probably", 1024, 1024});
    auto *game = new idk::Game(idk::GfxApi(ren));

    idk::Engine engine(ren, {game});
    engine.start();

    // idk::MMapFile file("assets/noise/voronoi.jpg");
    // printf("[MMapFile] base=0x%p, size=%lu\n", file.base, file.size);

    return 0;
}

