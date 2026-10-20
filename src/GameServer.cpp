#include "GameServer.hpp"
#include <glm/gtc/quaternion.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>

using namespace mygame;

GameServer::~GameServer() { stop(); }

bool GameServer::initialize(uint16_t port, const char *binding, const idk::EngineTimingConfig &timing, idk::level::Error &error)
{
    if (!timing.valid()) return error.set("Invalid server timing.");
    mBindFailed = !mNetwork.listen(port, mContent.manifest(), binding);
    if (mBindFailed) return error.set(mNetwork.error());
    if (mContent.isLevel())
    {
        idk::Arena physics(mStorage.physics);
        if (!mPhysics.initialize(physics, error) || !loadScene(0, error)) { stop(); return false; }
    }
    return true;
}

void GameServer::stop()
{
    mNetwork.stop();
    for (auto &player: mPlayers)
    {
        player.body.shutdown();
        player.entity = player.owner = player.held = 0;
    }
    mPhysics.shutdown();
    mLevel = {};
    mScene = mCompleted = mTick = 0;
}

bool GameServer::respawn(PlayerSlot &player)
{
    if (player.entity) mNetwork.world().destroy(player.entity);
    idk::net::Transform transform;
    if (mContent.isLevel())
    {
        player.controller.reset(mLevel.entry.position, mLevel.entry.rotation.y, mLevel.entry.rotation.x);
        player.inside = mLevel.atExit(player.body.position());
    }
    else transform.position[0] = float(&player - mPlayers) * 2.5f;
    player.entity = mNetwork.world().spawn(player.owner, mContent.isLevel() ? PlayerModel : DemoModel, transform);
    if (mContent.isLevel()) publishPlayer(player);
    return player.entity != 0;
}

void GameServer::publishPlayer(PlayerSlot &player)
{
    auto position = player.body.position();
    auto rotation = glm::angleAxis(glm::radians(player.controller.yaw()), glm::vec3(0, 1, 0));
    idk::net::Transform transform{{position.x, position.y, position.z}, {rotation.x, rotation.y, rotation.z, rotation.w}};
    mNetwork.world().setTransform(player.entity, transform);
}

bool GameServer::loadScene(size_t index, idk::level::Error &error)
{
    unsigned bank = mScene ? 1 - mBank : 0;
    idk::Arena arena(mStorage.levels[bank]);
    idk::level::Level candidate;
    if (!mContent.load(index, arena, candidate, error) || !mPhysics.prepareTerrain(candidate, error)) return false;
    mPhysics.commitTerrain();
    mLevel = candidate;
    mLevelIndex = index;
    mBank = bank;
    mFallLimit = mLevel.entry.position.y;
    for (const auto &object: mLevel.terrain)
        for (const auto &vertex: object.vertices) mFallLimit = std::min(mFallLimit, vertex.position.y);
    mFallLimit -= 50;
    if (mScene) mNetwork.world().destroy(mScene);
    if (mCompleted) mNetwork.world().destroy(mCompleted);
    mCompleted = 0;
    mScene = mNetwork.world().spawn(0, SceneBase + idk::net::ResourceId(index));
    for (auto &player: mPlayers)
        if (player.owner && !respawn(player)) return error.set("Player replication pool exhausted.");
    std::printf("Server: scene %zu committed\n", index);
    return mScene != 0 || error.set("Scene replication pool exhausted.");
}

bool GameServer::step(float dt, double physicsRate, idk::level::Error &error)
{
    if (!std::isfinite(dt) || dt <= 0 || dt > idk::EngineTimingConfig::MaxDeltaTimeSec ||
        !std::isfinite(physicsRate) || physicsRate < 30 || physicsRate > 1000)
        return error.set("Invalid simulation timestep or physics rate.");
    int transition = -1;
    for (size_t i = 0; i < idk::net::MaxClients; ++i)
    {
        auto &player = mPlayers[i];
        auto owner = mNetwork.clientAt(i);
        if (player.owner != owner)
        {
            if (player.entity) mNetwork.world().destroy(player.entity);
            player.body.shutdown();
            player.entity = 0;
            player.owner = owner;
            player.held = 0;
            if (owner)
            {
                if (mContent.isLevel() && !player.body.initialize(mPhysics, error)) return false;
                if (!respawn(player)) return error.set("Player replication pool exhausted.");
                std::printf("Server: client %u joined, entity %llu\n", owner, static_cast<unsigned long long>(player.entity));
            }
        }
        if (!owner) continue;
        idk::net::InputState input;
        bool hasInput = mNetwork.input(player.entity, input);
        // Missing/expired input cancels movement but is not a button release.
        // Preserve edges across respawn until a real release packet arrives.
        uint32_t pressed = hasInput ? input.buttons & ~player.held : 0;
        if (hasInput) player.held = input.buttons;
        if (pressed & Restart)
        {
            if (!respawn(player)) return error.set("Could not respawn player.");
            player.held = input.buttons;
            if (mCompleted) mNetwork.world().destroy(mCompleted);
            mCompleted = 0;
            continue;
        }
        if (!mContent.isLevel())
        {
            auto transform = mNetwork.world().find(player.entity)->transform;
            float length = std::hypot(input.axes[0], input.axes[1]);
            float scale = (input.buttons & Sprint ? 6 : 3) * dt / std::max(1.0f, length);
            transform.position[0] = std::clamp(transform.position[0] + input.axes[0] * scale, -20.0f, 20.0f);
            transform.position[2] = std::clamp(transform.position[2] - input.axes[1] * scale, -20.0f, 20.0f);
            float angle = std::atan2(transform.rotation[1], transform.rotation[3]) * 2 + .3f * dt;
            transform.rotation[1] = std::sin(angle / 2);
            transform.rotation[3] = std::cos(angle / 2);
            mNetwork.world().setTransform(player.entity, transform);
            continue;
        }
        player.body.setPhysicsRate(physicsRate);
        if (hasInput)
        {
            float yaw = input.axes[2] * 180.0f; // Absolute yaw, so UDP loss cannot lose look deltas.
            player.controller.look(yaw - player.controller.yaw(), 0);
            player.controller.move(input.axes[0], input.axes[1], (input.buttons & Sprint) != 0);
            if (pressed & Jump) player.controller.jump();
        }
        else player.controller.clearInput();
        player.controller.step(dt);
        if (player.body.position().y < mFallLimit)
        {
            if (!respawn(player)) return error.set("Could not respawn fallen player.");
        }
        publishPlayer(player);
        bool inside = mLevel.atExit(player.body.position());
        if (inside && !player.inside)
        {
            int destination = mContent.next(mLevelIndex);
            if (destination >= 0) transition = destination;
            else if (!mCompleted) mCompleted = mNetwork.world().spawn(0, Completed);
        }
        player.inside = inside;
    }
    if (transition >= 0 && !loadScene(size_t(transition), error))
    {
        // All fallible transition work precedes commit; the old scene remains usable.
        std::fprintf(stderr, "Server transition failed: %s\n", error.message);
    }
    return mNetwork.publish(++mTick) || error.set("Could not publish server snapshot.");
}
