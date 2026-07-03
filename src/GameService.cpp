#include "idk/GameService.hpp"
#include "idk/core/camera.hpp"
#include "idk/core/log.hpp"
#include "idk/engine/InputState.hpp"


idk::GameService::GameService(idk::GfxService *gfxsrv)
:   Service("GameService", idk_typeid<GameService>()),
    mGfx(gfxsrv)
{
    uint64_t tickRateHz = mCfg["TICKRATE_HZ"].getValueU64();
    timer_.setRateHz(tickRateHz);
    VLOG_INFO("[GameService::GameService] tickRateHz={}", tickRateHz);

    mCtl.moveSpeed = 400.0f;
    mCtl.lookSpeed = 10.0f;
}


void idk::GameService::startup(idk::IEngine*)
{
    VLOG_INFO("[GameService::startup]");
}


void idk::GameService::update(idk::IEngine *E)
{
    (void)E;

    mGfx->getRenderer().setLerpAlpha(timer_.getExpiryAlpha());

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
        auto &cam = mGfx->getRenderer().getCamera();
        auto &T = cam.getTransform();

        T.Translate(dt * dMove.x * T.GetRight());
        T.Translate(dt * dMove.y * T.GetUp());
        T.Translate(dt * dMove.z * T.GetForward());
        // T.RotateLocal();

        T.PitchLocal(dt * dPitch);
        T.YawWorld(dt * dYaw);

        mGfx->getRenderer().setLerpAlpha(0);
        mGfx->getRenderer().swapCamera();
    }
}


void idk::GameService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[GameService::shutdown]");
}

