#include "idk/game_service.hpp"
#include "idk/core/log.hpp"
#include "idk/gfx/camera.hpp"


idk::GameService::GameService(const idk::GfxApi &gfxapi)
:   Service(idk::PeriodicTimer(1000.0 / 60.0)),
    mGfx(gfxapi),
    dmove(0.0f),
    dlook(0.0f)
{
    // static gfx::BgColorSetResponse res;
    // gfx::BgColorSetRequest req(glm::vec4(0.5f));
    // mGfx.BgColorSet(req, &res);
}


void idk::GameService::_startup(idk::IEngine*)
{
    VLOG_INFO("[idk::GameService::_startup]");
}


void idk::GameService::_update(idk::IEngine*)
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


void idk::GameService::_shutdown(idk::IEngine*)
{
    VLOG_INFO("[idk::GameService::_shutdown]");
}

