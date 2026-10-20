#include "idk/engine/Engine.hpp"
#include "GameImpl.hpp"
#include "GameServer.hpp"
#include <cstdio>

int main(int argc, char **argv)
{
    mygame::Options options;
    if (!options.parse(argc, argv)) { options.usage(); return 2; }
    if (options.help) { options.usage(); return 0; }
    // Dedicated mode never constructs Engine or initializes graphics/audio.
    if (options.mode == mygame::Mode::Server) return mygame::runServer(options);
    static mygame::Content content;
    idk::level::Error error;
    if (!content.initialize(*options.level ? options.level : nullptr, options.root, options.demo, error))
    {
        std::fprintf(stderr, "Game content: %s\n", error.message);
        return 1;
    }
    mygame::LocalServer local;
    uint16_t port = options.port;
    if (options.mode == mygame::Mode::Local && !local.start(options, port))
    {
        std::fputs("Could not start local game server. Check its diagnostics and installed game_server executable.\n", stderr);
        return 1;
    }
    idk::Engine engine(options.explicitConfig ? options.config : nullptr);
    static mygame::GameImpl::Storage storage;
    mygame::GameImpl game(content, options, port, storage, options.mode == mygame::Mode::Local ? &local : nullptr);
    bool started = engine.startApplication(&game);
    local.stop();
    return started && !game.failed() ? 0 : 1;
}
