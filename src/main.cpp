#include "idk/engine/engine.hpp"
#include "idk/gfx/gfx.hpp"
#include "idk/gfx/renderer.hpp"
#include "idk/game/game.hpp"

#include "idk/core/double_buffer.hpp"

#include <barrier>
#include <thread>

static idk::core::DoubleBuffer<idk::gfx::CmdData>    gfxbuffer;
static idk::core::DblBufferReader<idk::gfx::CmdData> gfxread(gfxbuffer);
static idk::core::DblBufferWriter<idk::gfx::CmdData> gfxwrite(gfxbuffer);

static std::barrier mainloop_sync_(2);
static std::barrier shutdown_sync_(2);


int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    idk::Engine engine;

    std::thread t0(idk::game::main, &engine);
    t0.detach();

    idk::gfx::main(&engine);

    return 0;
}


#include "idk/engine/controller.hpp"
idk::TestCharacterController pcontrol;



void idk::gfx::main(idk::core::IEngine *engine)
{
    idk::gfx::RenderEngine ren(
        {"A Game Probably", 1280, 720},
        gfxread
    );

    while (engine->get_stat() != EngineStat::Dead)
    {
        ren.beginFrame();

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                VLOG_INFO("SDL_EVENT_QUIT");
                engine->shutdown();
            }
        }

        ren.endFrame();
        engine->update();

        mainloop_sync_.arrive_and_wait();
    }

    shutdown_sync_.arrive_and_wait();
}



void idk::game::main(idk::core::IEngine *engine)
{
    gfxwrite->push({gfx::CmdType::BgColorSet, glm::vec4(0.5f)});

    pcontrol.moveSpeed = 0.00001f;
    glm::vec3 dmove, dlook;


    while (engine->get_stat() != EngineStat::Dead)
    {
        pcontrol.update();
        pcontrol.getMotion(dmove, dlook);

        gfxwrite->push({
            gfx::CmdType::BgColorAdd,
            glm::vec4(dmove.x, dmove.z, 0.0f, 0.0f)
        });

        gfxbuffer.swapBuffers();
        mainloop_sync_.arrive_and_wait();
    }

    shutdown_sync_.arrive_and_wait();
}
