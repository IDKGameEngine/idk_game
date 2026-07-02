#include "idk/game_service.hpp"
#include "idk/core/camera.hpp"
#include "idk/core/cfgparser.hpp"
#include "idk/core/log.hpp"


idk::GameService::GameService(idk::GfxService *gfxsrv)
:   Service(idk_typeid<GameService>()),
    mCfg("asset/GameService.cfg"),
    mGfx(gfxsrv)
{
    uint64_t updateRateHz = mCfg["UPDATE_RATE"].getValueU64();
    timer_.setRateHz(updateRateHz);
    VLOG_INFO("[GameService::GameService] updateRateHz={}", updateRateHz);

    mCtl.moveSpeed = 10.0f;
}


void idk::GameService::startup(idk::IEngine*)
{
    VLOG_INFO("[GameService::startup]");
}


void idk::GameService::update(idk::IEngine *E)
{
    (void)E;

    if (timer_.expired())
    {
        timer_.reset();

        static glm::vec3 dMove;
        static float     dPitch;
        static float     dYaw;

        mCtl.update();
        mCtl.getMotion(dMove, dPitch, dYaw);
        mCtl.clearMotion();

        auto &cam = mGfx->getRenderer().getCamera();
        auto &T = cam.getTransform();

        T.Translate(dMove.x * T.GetRight());
        T.Translate(dMove.y * T.GetUp());
        T.Translate(dMove.z * T.GetForward());
        // T.RotateLocal();

        T.PitchLocal(dPitch);
        T.YawWorld(dYaw);
    }

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


void idk::GameService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[GameService::shutdown]");
}

