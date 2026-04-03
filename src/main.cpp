#include "idk/version.h"

#include "idk/core/file.hpp"
#include "idk/engine/engine.hpp"
#include "idk/gfx/renderer.hpp"
#include "idk/game/game.hpp"
#include "idk/idk_gfxapi.hpp"


int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    idk::Engine engine(2U);

    idk::MMapFile file("assets/noise/voronoi.jpg");
    printf("[MMapFile] base=0x%p, size=%lu\n", file.base, file.size);

    auto *ren  = new idk::gfx::RenderEngine({"A Game Probably", 1280, 720});
    auto *game = new idk::Game(idk::GfxApi(ren));

    engine.addService(game);
    engine.start(ren);

    return 0;
}



// void idk::gfx::main(idk::core::IEngine *engine)
// {
//     idk::gfx::RenderEngine ren(
//         {"A Game Probably", 1280, 720},
//         gfxread
//     );

//     while (engine->get_stat() != EngineStat::Dead)
//     {
//         SDL_Event e;
//         while (SDL_PollEvent(&e))
//         {
//             if (e.type == SDL_EVENT_QUIT)
//             {
//                 VLOG_INFO("SDL_EVENT_QUIT");
//                 engine->shutdown();
//             }
//         }

//         ren.onUpdate();
//         engine->update();

//         mainloop_sync_.arrive_and_wait();
//     }

//     shutdown_sync_.arrive_and_wait();
// }

