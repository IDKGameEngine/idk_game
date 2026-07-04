#include "idk/GameService.hpp"
#include "idk/core/camera.hpp"
#include "idk/core/log.hpp"
#include "idk_engine/InputState.hpp"


idk::GameService::GameService()
:   IDK_SERVICE_CTOR(GameService)
{
    uint64_t tickRateHz = mCfg["TICKRATE_HZ"].toU64();
    timer_.setRateHz(tickRateHz);
    VLOG_INFO("[GameService::GameService] tickRateHz={}", tickRateHz);

    mCtl.moveSpeed = 400.0f;
    mCtl.lookSpeed = 10.0f;
}


void idk::GameService::update(idk::IEngine *E)
{
    auto *gfx = E->getService<idk::GfxService>();
    if (!gfx) { return; }
    
    auto &ren = gfx->getRenderer();
    ren.setLerpAlpha(timer_.getExpiryAlpha());

    if (timer_.expired())
    {
        timer_.reset();

        static glm::vec3 dMove;
        static float     dPitch;
        static float     dYaw;

        mCtl.update();
        mCtl.getMotion(dMove, dPitch, dYaw);
        mCtl.clearMotion();

        float dt = timer_.getPeriodSec<float>();
        auto &cam = ren.getCamera();
        auto &T = cam.getTransform();

        T.Translate(dt * dMove.x * T.GetRight());
        T.Translate(dt * dMove.y * T.GetUp());
        T.Translate(dt * dMove.z * T.GetForward());
        // T.RotateLocal();

        T.PitchLocal(dt * dPitch);
        T.YawWorld(dt * dYaw);

        ren.setLerpAlpha(0);
        ren.swapCamera();
    }
}


void idk::GameService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[GameService::shutdown]");
}

