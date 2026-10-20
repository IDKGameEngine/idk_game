#include "GameServer.hpp"
#include <algorithm>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <thread>
#include <unistd.h>

namespace
{
    volatile std::sig_atomic_t running = 1, reload = 0;
    void stop(int) { running = 0; }
    void reloadConfig(int) { reload = 1; }
}

int mygame::runServer(const Options &options)
{
    std::signal(SIGINT, stop);
    std::signal(SIGTERM, stop);
    std::signal(SIGHUP, reloadConfig);
    idk::EngineTimingConfig config;
    if (const char *error = config.loadFile(options.config, !options.explicitConfig))
    {
        std::fprintf(stderr, "Server timing: %s\n", error);
        return 1;
    }
    static Content content;
    idk::level::Error error;
    if (!content.initialize(*options.level ? options.level : nullptr, options.root, options.demo, error))
    {
        std::fprintf(stderr, "Server content: %s\n", error.message);
        return 1;
    }
    static GameServer::Storage storage;
    GameServer server(content, storage);
    uint16_t port = options.port;
    bool opened = false;
    // Private servers select an unused loopback port and report it through a pipe.
    for (unsigned attempt = 0; attempt < (options.readyFd >= 0 && !options.explicitPort ? 64U : 1U); ++attempt)
    {
        if (server.initialize(port, options.readyFd >= 0 ? "127.0.0.1" : options.binding, config, error))
        {
            opened = true; break;
        }
        server.stop();
        if (!server.bindFailed()) break;
        if (++port == 0) port = 30000;
    }
    if (!opened)
    {
        std::fprintf(stderr, "Server startup: %s\n", error.message);
        return 1;
    }
    if (options.readyFd >= 0)
    {
        if (write(options.readyFd, &port, sizeof(port)) != sizeof(port)) return 1;
        close(options.readyFd);
    }
    std::printf("Game server listening on UDP %u (%s), fixed=%g Hz, physics=%g Hz\n",
                port, content.isLevel() ? "level" : "demo", config.fixedRateHz, config.physicsRateHz);
    std::fflush(stdout);
    double previous = idk::net::nowSeconds(), accumulated = 0;
    while (running && (!options.ticks || server.tick() < options.ticks))
    {
        if (reload)
        {
            reload = 0;
            idk::EngineTimingConfig candidate;
            if (const char *message = candidate.loadFile(options.config, !options.explicitConfig))
                std::fprintf(stderr, "Server timing reload failed: %s\n", message);
            else
            {
                accumulated *= config.fixedRateHz / candidate.fixedRateHz;
                config = candidate;
                std::puts("Server timing reloaded");
            }
        }
        double now = idk::net::nowSeconds();
        accumulated += std::min(now - previous, idk::EngineTimingConfig::MaxDeltaTimeSec);
        previous = now;
        if (!server.poll(now)) { std::fputs("Server transport failed\n", stderr); return 1; }
        double step = 1.0 / config.fixedRateHz;
        while (accumulated >= step && (!options.ticks || server.tick() < options.ticks))
        {
            accumulated -= step;
            if (!server.step(float(step), config.physicsRateHz, error))
            {
                std::fprintf(stderr, "Server simulation: %s\n", error.message); return 1;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
