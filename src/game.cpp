#include "idk/game/game.hpp"


idk::Game::Game(const idk::GfxApi &gfxapi)
:   mGfx(gfxapi),
    dmove(0.0f),
    dlook(0.0f)
{
    mGfx.bgColorSet(glm::vec4(0.5f));
}


void idk::Game::onUpdate(idk::IEngine*)
{
    if (!mTimer.expired())
    {
        return;
    }

    float dt = 1000.0f / float(mTimer.rateHz);
    mInput.moveSpeed = 0.001f * dt;

    mInput.update();
    mInput.getMotion(dmove, dlook);

    mGfx.bgColorAdd(glm::vec4(dmove.x, dmove.z, 0.0f, 0.0f));
}


void idk::Game::onShutdown(idk::IEngine*)
{
    VLOG_INFO("idk::Game::onShutdown");
}
