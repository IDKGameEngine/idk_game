#pragma once

#include "Content.hpp"
#include "Options.hpp"
#include "idk/EngineTiming.hpp"
#include "idk/level/Player.hpp"
#include "idk/gameplay/PlayerController.hpp"

namespace mygame
{
    class GameServer final: public idk::Immobile
    {
    public:
        struct Storage
        {
            alignas(16) std::byte levels[2][4 * 1024 * 1024];
            alignas(16) std::byte physics[64 * 1024];
        };
        GameServer(const Content &content, Storage &storage): mContent(content), mStorage(storage) {}
        ~GameServer();
        bool initialize(uint16_t port, const char *binding, const idk::EngineTimingConfig &, idk::level::Error &);
        bool poll(double now) { return mNetwork.poll(now); }
        bool step(float dt, double physicsRate, idk::level::Error &);
        void stop();
        const idk::net::Server &network() const { return mNetwork; }
        uint64_t tick() const { return mTick; }
        bool bindFailed() const { return mBindFailed; }

    private:
        struct PlayerSlot
        {
            idk::level::Player body;
            idk::gameplay::PlayerController controller{body};
            idk::net::EntityId entity = 0;
            idk::net::ClientId owner = 0;
            uint32_t held = 0;
            bool inside = false;
        };
        const Content &mContent;
        Storage &mStorage;
        idk::net::Server mNetwork;
        idk::level::PhysicsWorld mPhysics;
        idk::level::Level mLevel;
        PlayerSlot mPlayers[idk::net::MaxClients];
        idk::net::EntityId mScene = 0, mCompleted = 0;
        uint64_t mTick = 0;
        unsigned mBank = 0;
        size_t mLevelIndex = 0;
        float mFallLimit = -100;
        bool mBindFailed = false;
        bool loadScene(size_t index, idk::level::Error &);
        bool respawn(PlayerSlot &);
        void publishPlayer(PlayerSlot &);
    };

    int runServer(const Options &);
}
