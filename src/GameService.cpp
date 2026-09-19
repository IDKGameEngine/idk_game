#include "idk/GameService.hpp"
#include "idk/gfx/GfxService.hpp"


idk::GameService::GameService(EngineContext &ctx)
:   EngineComponent(ctx)
{
    // mCtl.moveSpeed = 400.0f;
    // mCtl.lookSpeed = 10.0f;

    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
    // mEntityManager.createEntity();
}


void idk::GameService::update()
{
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

