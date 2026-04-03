#include "idk/game/game.hpp"


idk::Game::Game(const idk::GfxApi &gfxapi)
:   mGfx(gfxapi),
    dmove(0.0f),
    dlook(0.0f),
    tprev(SDL_GetTicks())
{
    mGfx.bgColorSet(glm::vec4(0.5f));
    mInput.moveSpeed = 0.00001f;
}

void idk::Game::onUpdate(idk::IEngine*)
{
    uint64_t tcurr, tdiff;

    tcurr = SDL_GetTicks();
    tdiff = (tcurr - tprev);
    tprev = tcurr;

    mInput.update();
    mInput.getMotion(dmove, dlook);

    mGfx.bgColorAdd(glm::vec4(dmove.x, dmove.z, 0.0f, 0.0f));
}


void idk::Game::onShutdown(idk::IEngine*)
{
    VLOG_INFO("idk::Game::onShutdown");
}
