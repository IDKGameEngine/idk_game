#include "GameImpl.hpp"
#include "idk/engine/Engine.hpp"
#include "idk/platform/AudioManager.hpp"


static idk::ObjectHandle snd;

void mygame::GameImpl::onInit(idk::EngineAPI &api)
{
    auto *audio = api.mAudio;
    snd = audio->createSound("audio/hurt3.wav");
    audio->startSound(snd);
}


void mygame::GameImpl::onShutdown(idk::EngineAPI &api)
{
    (void)api;
}


void mygame::GameImpl::onPreFrame(idk::EngineAPI &api)
{
    auto *audio = api.mAudio;

    if (audio->isSoundFinished(snd))
    {
        VLOG_INFO("[GameImpl::onUpdate] Sound finished!");
    }

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


void mygame::GameImpl::onPostFrame(idk::EngineAPI&)
{

}


void mygame::GameImpl::onEvent(idk::EngineAPI &api, const void*)
{
    (void)api;
}

