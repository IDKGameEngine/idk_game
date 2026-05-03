#include "idk/game/game.hpp"
#include "idk/core/log.hpp"


idk::Game::Game(const idk::GfxApi &gfxapi)
:   Service(idk::PeriodicTimer(1000.0 / 60.0)),
    mGfx(gfxapi),
    dmove(0.0f),
    dlook(0.0f)
{
    // static gfx::BgColorSetResponse res;
    // gfx::BgColorSetRequest req(glm::vec4(0.5f));

    // mGfx.BgColorSet(req, &res);
}


void idk::Game::_startup(idk::IEngine*)
{
    VLOG_INFO("idk::Game::_startup");
}


void idk::Game::_update(idk::IEngine*)
{
    mGfx.FlushCommandQueue();

    // static gfx::BgColorAddResponse res;
    // gfx::BgColorAddRequest req(glm::vec4(dmove.x, dmove.z, 0.0f, 0.0f));

    // float dt = 1000.0f / timer_.getPeriodMs<float>();
    // mInput.moveSpeed = 0.001f * dt;

    // mInput.update();
    // mInput.getMotion(dmove, dlook);

    // mGfx.BgColorAdd(req, &res);


    // static gfx::AddComputeProgramResponse res2;
    // mGfx.AddComputeProgram(
    //     gfx::AddComputeProgramRequest("asset/shader/automata.comp"),
    //     &res2
    // );
}


void idk::Game::_shutdown(idk::IEngine*)
{
    VLOG_INFO("idk::Game::_shutdown");
}

