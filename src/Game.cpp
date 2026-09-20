#include "Game.hpp"
#include "idk/engine/Engine.hpp"

#include "idk/platform/PlatformContext.hpp"
#include "idk/platform/AudioManager.hpp"


mygame::Game::Game()
{
    // mCtl.moveSpeed = 400.0f;
    // mCtl.lookSpeed = 10.0f;

    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
}


void mygame::Game::onInit(idk::Engine &E)
{
    auto *ctx = E.getPlatformContext();

    if (auto *audio = ctx->getFeature<idk::AudioManager>())
    {
        auto *snd = audio->createSound("audio/hurt3.wav");
        audio->startSound(snd);
    }
    else
    {
        VLOG_FATAL("[GameService::init] Cannot obtain audio backend");
    }
}


void mygame::Game::onUpdate(idk::Engine &E)
{
    (void)E;

    // auto *gfx = E->getService<idk::GfxService>();
    // if (!gfx) { return; }
    
    // auto &ren = gfx->getRenderer();
    // // ren.setLerpAlpha(mTimer.getExpiryAlpha());

    // if (mTimer.expired())
    // {
    //     mTimer.reset();

    //     static glm::vec3 dMove;
    //     static float     dPitch;
    //     static float     dYaw;

    //     mCtl.update();
    //     mCtl.getMotion(dMove, dPitch, dYaw);
    //     mCtl.clearMotion();

    //     float dt = mTimer.getPeriodSec<float>();
    //     auto &cam = ren.getCamera();
    //     auto &T = cam.getTransform();

    //     T.Translate(dt * dMove.x * T.GetRight());
    //     T.Translate(dt * dMove.y * T.GetUp());
    //     T.Translate(dt * dMove.z * T.GetForward());
    //     // T.RotateLocal();

    //     T.PitchLocal(dt * dPitch);
    //     T.YawWorld(dt * dYaw);

    //     // ren.setLerpAlpha(0);
    //     ren.swapCamera();
    // }
}


void mygame::Game::onShutdown(idk::Engine &E)
{
    (void)E;
}

