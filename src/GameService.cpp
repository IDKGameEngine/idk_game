#include "GameService.hpp"
#include "libidk/platform-sdl3/PlatformContext.hpp"
#include "libidk/platform/IAudioBackend.hpp"


idk::GameService::GameService(EngineContext &ctx)
:   EngineService(ctx)
{
    // mCtl.moveSpeed = 400.0f;
    // mCtl.lookSpeed = 10.0f;

    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
}


void idk::GameService::init(idk::Engine &E)
{
    auto *ctx = E.getPlatformContext();

    if (auto *audio = ctx->getFeature<idk::IAudioBackend>())
    {
        auto *snd = audio->createSound("audio/hurt3.wav");
        audio->startSound(snd);
    }
    else
    {
        VLOG_FATAL("[GameService::init] Cannot obtain audio backend");
    }
}


void idk::GameService::update(idk::Engine &E)
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

