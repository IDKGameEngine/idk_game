#pragma once

#include "Content.hpp"
#include "Options.hpp"
#include "LocalServer.hpp"
#include "libidk/Service.hpp"
#include "idk/gfx/RenderTypes.hpp"

namespace idk::gfx { class RenderEngine; }

namespace mygame
{
    // Presentation and input only. All movement/physics runs in GameServer.
    class GameImpl final: public idk::Service
    {
    public:
        struct Storage
        {
            alignas(16) std::byte levels[2][4 * 1024 * 1024];
            alignas(16) std::byte scratch[8 * 1024 * 1024];
        };
        GameImpl(const Content &content, const Options &options, uint16_t port, Storage &storage,
                 LocalServer *local = nullptr)
            : mContent(content), mOptions(options), mPort(port), mStorage(storage), mLocal(local) {}
        void onInit(idk::EngineAPI &) override;
        void onShutdown(idk::EngineAPI &) override;
        void onUpdate(idk::EngineAPI &) override;
        void onPreRender(idk::EngineAPI &) override;
        bool failed() const { return mFailed; }

    private:
        const Content &mContent;
        const Options &mOptions;
        uint16_t mPort;
        Storage &mStorage;
        LocalServer *mLocal;
        idk::net::Client mClient;
        idk::net::ResourceMap<idk::gfx::ModelHandle, 1> mModels;
        idk::gfx::ModelHandle mModel;
        idk::gfx::MeshHandle mBox;
        idk::gfx::MaterialHandle mPlayerMaterial, mExitMaterial;
        idk::level::Level mLevel;
        idk::net::EntityId mScene = 0, mOwnEntity = 0;
        uint64_t mRevision = 0;
        uint32_t mFrames = 0;
        uint32_t mPendingButtons = 0;
        unsigned mBank = 0;
        double mSnapshotAt = 0, mSnapshotInterval = 1.0 / 30, mNextInput = 0;
        double mFixedRate = 0, mPhysicsRate = 0;
        float mYaw = 0, mPitch = 0;
        bool mCaptured = false, mTabHeld = false, mFailed = false, mComplete = false;
        bool loadScene(idk::gfx::RenderEngine &, size_t);
        void fail(idk::EngineAPI &, const char *);
        idk::net::Transform interpolated(const idk::net::EntityState &, double now) const;
    };
}
