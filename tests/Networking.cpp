#include "GameServer.hpp"
#include "idk/level/Build.hpp"
#include "idk/mapedit/MapSession.hpp"
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <unistd.h>

#define CHECK(value) do { if (!(value)) { \
    std::fprintf(stderr, "Check failed: %s (%d): %s\n", #value, __LINE__, error.message); return false; } } while (false)

using namespace mygame;
using namespace idk::net;

namespace
{
    struct Files
    {
        char root[64] = "/tmp/idk-game-network-XXXXXX", a[128]{}, b[128]{}, demo[128]{};
        bool create()
        {
            if (!mkdtemp(root)) return false;
            std::snprintf(a, sizeof(a), "%s/a.idklvl", root);
            std::snprintf(b, sizeof(b), "%s/b.idklvl", root);
            std::snprintf(demo, sizeof(demo), "%s/demo.glb", root);
            return true;
        }
        ~Files() { unlink(a); unlink(b); unlink(demo); rmdir(root); }
    };

    const EntityState *owned(const Client &client)
    {
        for (const auto &entity: std::span(client.snapshot().entities, client.snapshot().count))
            if (entity.owner && entity.owner == client.id()) return &entity;
        return nullptr;
    }

    bool hasResource(const Client &client, ResourceId resource)
    {
        for (const auto &entity: std::span(client.snapshot().entities, client.snapshot().count))
            if (entity.resource == resource) return true;
        return false;
    }

    bool advance(GameServer &server, Client *clients, const InputState *inputs, size_t count, double &now,
                 unsigned ticks, idk::level::Error &error)
    {
        for (unsigned tick = 0; tick < ticks; ++tick)
        {
            now += 1.0 / 30;
            for (size_t i = 0; i < count; ++i)
            {
                clients[i].poll(now);
                if (const auto *entity = owned(clients[i])) CHECK(clients[i].sendInput(entity->id, inputs[i]));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            CHECK(server.poll(now) && server.step(1.0f / 30, 120, error));
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            for (size_t i = 0; i < count; ++i) clients[i].poll(now);
        }
        return true;
    }

    uint16_t start(GameServer &server, idk::level::Error &error)
    {
        uint16_t port = uint16_t(35000 + getpid() % 10000);
        for (unsigned i = 0; i < 100; ++i)
        {
            if (server.initialize(port, "127.0.0.1", {}, error)) return port;
            if (!server.bindFailed()) return 0;
            ++port;
        }
        return 0;
    }

    bool levels(Files &files, idk::level::Error &error)
    {
        alignas(16) static std::byte geometryBytes[64 * 1024], builtBytes[4 * 1024 * 1024];
        idk::Arena geometry(geometryBytes), built(builtBytes);
        idk::mapedit::MapSession session;
        CHECK(session.initialize(geometry));
        session.addShape(idk::asset::ShapeKind::Box);
        session.selected()->position = {0, -.5f, 0};
        session.selected()->scale = {20, 1, 20};
        session.placeMarker(idk::mapedit::ObjectRole::Entry);
        session.selected()->position = {0, .1f, 0};
        session.placeMarker(idk::mapedit::ObjectRole::Exit);
        session.selected()->position = {3, .5f, 0};
        session.selected()->scale = {.5f, 2, 2};
        std::snprintf(session.destination().path, sizeof(session.destination().path), "b.idklvl");
        idk::level::Level level;
        CHECK(idk::level::Build(session, built, level, error) && idk::level::Save(level, files.a, error));
        session.document().get(0)->position.y = 4.5f;
        session.document().get(session.marker(idk::mapedit::ObjectRole::Entry))->position.y = 5.1f;
        session.document().get(session.marker(idk::mapedit::ObjectRole::Exit))->position.y = 5.5f;
        session.destination().path[0] = '\0';
        built.reset();
        CHECK(idk::level::Build(session, built, level, error) && idk::level::Save(level, files.b, error));

        Content content, same;
        CHECK(content.initialize(files.a, files.root, nullptr, error) && content.count() == 2 && content.next(0) == 1);
        CHECK(same.initialize(files.a, files.root, nullptr, error) && same.manifest() == content.manifest());
        static GameServer::Storage storage;
        GameServer server(content, storage);
        uint16_t port = start(server, error);
        CHECK(port);
        Client clients[3];
        InputState inputs[3];
        double now = 100;
        for (unsigned i = 0; i < 2; ++i) CHECK(clients[i].connect("127.0.0.1", port, content.manifest(), now));
        CHECK(advance(server, clients, inputs, 2, now, 20, error));
        CHECK(owned(clients[0]) && owned(clients[1]) && hasResource(clients[0], SceneBase));
        EntityId first = owned(clients[0])->id, second = owned(clients[1])->id;
        inputs[0].axes[0] = 1;
        CHECK(advance(server, clients, inputs, 2, now, 8, error));
        const auto *moving = owned(clients[0]);
        CHECK(moving && moving->transform.position[0] > .5f);
        CHECK(std::abs(owned(clients[1])->transform.position[0]) < .05f);
        CHECK(clients[1].snapshot().find(first)->transform.position[0] == moving->transform.position[0]);
        inputs[0] = {};
        CHECK(advance(server, clients, inputs, 2, now, 20, error)); // Brake before jumping; airborne momentum is retained.
        inputs[0].buttons = Jump;
        CHECK(advance(server, clients, inputs, 2, now, 3, error));
        CHECK(owned(clients[0])->transform.position[1] > .2f);
        CHECK(advance(server, clients, inputs, 2, now, 40, error));
        CHECK(std::abs(owned(clients[0])->transform.position[1]) < .05f); // Holding jump does not repeat.
        inputs[0].buttons = Restart;
        CHECK(advance(server, clients, inputs, 2, now, 5, error));
        CHECK(owned(clients[0])->id != first && !clients[1].snapshot().find(first));
        EntityId reset = owned(clients[0])->id;
        CHECK(advance(server, clients, inputs, 2, now, 5, error));
        CHECK(owned(clients[0])->id == reset && std::abs(owned(clients[0])->transform.position[0]) < .05f);

        inputs[0] = {};
        inputs[0].axes[0] = 1;
        for (unsigned i = 0; i < 60 && !hasResource(clients[0], SceneBase + 1); ++i)
            CHECK(advance(server, clients, inputs, 2, now, 1, error));
        CHECK(hasResource(clients[0], SceneBase + 1) && hasResource(clients[1], SceneBase + 1));
        CHECK(!clients[0].snapshot().find(reset) && !clients[1].snapshot().find(second));
        CHECK(owned(clients[0])->transform.position[1] > 5 && owned(clients[1])->transform.position[1] > 5);
        for (unsigned i = 0; i < 60 && !hasResource(clients[0], Completed); ++i)
            CHECK(advance(server, clients, inputs, 2, now, 1, error));
        CHECK(hasResource(clients[0], Completed) && hasResource(clients[1], Completed));
        inputs[0] = {};
        CHECK(clients[2].connect("127.0.0.1", port, content.manifest(), now));
        CHECK(advance(server, clients, inputs, 3, now, 20, error));
        CHECK(owned(clients[2]) && hasResource(clients[2], SceneBase + 1) && hasResource(clients[2], Completed));
        EntityId leaving = owned(clients[2])->id;
        clients[2].disconnect();
        CHECK(advance(server, clients, inputs, 2, now, 5, error));
        CHECK(!clients[0].snapshot().find(leaving) && !server.network().world().find(leaving));

        CHECK(clients[2].connect("127.0.0.1", port, content.manifest() + 1, now));
        CHECK(advance(server, clients, inputs, 3, now, 10, error));
        CHECK(clients[2].status() == ClientStatus::Rejected && clients[2].rejection() == RejectReason::ManifestMismatch);
        // The current scene remains valid if a level on disk changes mid-session.
        auto *file = std::fopen(files.b, "ab");
        CHECK(file && std::fputc(0, file) != EOF && std::fclose(file) == 0);
        built.reset();
        CHECK(!content.load(1, built, level, error));
        CHECK(hasResource(clients[0], SceneBase + 1));
        server.stop();
        return true;
    }

    bool demo(Files &files, idk::level::Error &error)
    {
        auto *file = std::fopen(files.demo, "wb");
        CHECK(file && std::fputs("test model content", file) >= 0 && std::fclose(file) == 0);
        Content content;
        CHECK(content.initialize(nullptr, files.root, files.demo, error));
        static GameServer::Storage storage;
        GameServer server(content, storage);
        uint16_t port = start(server, error);
        Client client;
        InputState input;
        double now = 100;
        CHECK(port && client.connect("127.0.0.1", port, content.manifest(), now));
        CHECK(advance(server, &client, &input, 1, now, 20, error));
        CHECK(owned(client) && owned(client)->resource == DemoModel);
        EntityId id = owned(client)->id;
        input.axes[0] = input.axes[1] = 1;
        CHECK(advance(server, &client, &input, 1, now, 30, error));
        auto transform = owned(client)->transform;
        CHECK(std::abs(std::hypot(transform.position[0], transform.position[2]) - 3) < .1f);
        CHECK(std::abs(transform.rotation[1]) > .01f);
        input = {}; input.buttons = Restart;
        CHECK(advance(server, &client, &input, 1, now, 5, error));
        CHECK(owned(client)->id != id && std::abs(owned(client)->transform.position[0]) < .01f);
        return true;
    }
}

int main()
{
    idk::level::Error error;
    Files files;
    if (!files.create() || !levels(files, error) || !demo(files, error)) return 1;
    std::puts("Game networking: authoritative movement/jump/respawn, scene transitions, late join, content checks and demo passed");
    return 0;
}
