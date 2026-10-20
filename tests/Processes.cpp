#include "LocalServer.hpp"
#include "libidk/message/RemoteRxTx.hpp"
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>

#define CHECK(value) do { if (!(value)) { \
    std::fprintf(stderr, "Check failed: %s (%d)\n", #value, __LINE__); return false; } } while (false)

namespace
{
    class Child: public idk::Immobile
    {
        pid_t mPid = -1;
    public:
        ~Child()
        {
            if (mPid > 0)
            {
                kill(mPid, SIGKILL);
                while (waitpid(mPid, nullptr, 0) < 0 && errno == EINTR) {}
            }
        }
        bool start(const char *game, const mygame::Options &options, uint16_t port, bool connect, bool badVideo = false)
        {
            mPid = fork();
            if (mPid == 0)
            {
                setenv("SDL_VIDEODRIVER", badVideo ? "idk_invalid_driver" : "offscreen", 1);
                setenv("SDL_AUDIODRIVER", "dummy", 1);
                char portText[16];
                std::snprintf(portText, sizeof(portText), "%u", port);
                const char *args[16]{game, "--frames", "8", "--port", portText, "--config", options.config};
                unsigned count = 7;
                if (connect) { args[count++] = "--connect"; args[count++] = "127.0.0.1"; }
                if (*options.level)
                {
                    args[count++] = "--level"; args[count++] = options.level;
                    args[count++] = "--level-root"; args[count++] = options.root;
                }
                execv(game, const_cast<char *const *>(args));
                _exit(127);
            }
            return mPid > 0;
        }
        int wait()
        {
            for (unsigned i = 0; i < 10000; ++i)
            {
                int status = 0;
                pid_t result = waitpid(mPid, &status, WNOHANG);
                if (result == mPid)
                {
                    mPid = -1;
                    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
                }
                if (result < 0 && errno != EINTR) return -1;
                usleep(1000);
            }
            return -1;
        }
    };

    bool scenario(const char *game, mygame::Options &options)
    {
        mygame::LocalServer server;
        uint16_t port = 0;
        CHECK(server.start(options, port) && server.alive());
        mygame::Options conflict = options;
        conflict.port = port;
        conflict.explicitPort = true;
        mygame::LocalServer duplicate;
        uint16_t unused = 0;
        CHECK(!duplicate.start(conflict, unused) && server.alive());
        Child a, b;
        CHECK(a.start(game, options, port, true) && b.start(game, options, port, true));
        CHECK(a.wait() == 0 && b.wait() == 0 && server.alive());
        server.stop();
        CHECK(!server.alive());
        // A successful local game owns and reaps its dedicated child.
        Child local;
        CHECK(local.start(game, options, port, false) && local.wait() == 0);
        idk::RemoteRxTxer probe;
        CHECK(probe.open(nullptr, 0, port, "127.0.0.1"));
        probe.close();
        // Even failure after server startup must release the child's socket.
        Child failure;
        CHECK(failure.start(game, options, port, false, true) && failure.wait() == 1);
        CHECK(probe.open(nullptr, 0, port, "127.0.0.1"));
        return true;
    }
}

int main(int argc, char **argv)
{
    if (argc != 4) return 2;
    mygame::Options options;
    if (!options.parse(1, argv)) return 1;
    if (!realpath(argv[2], options.config)) return 1;
    options.explicitConfig = true;
    if (!scenario(argv[1], options)) return 1;
    if (!realpath(argv[3], options.level)) return 1;
    if (!scenario(argv[1], options)) return 1;
    std::puts("Game processes: demo/level, two remote clients, private server, port conflict and failure cleanup passed");
    return 0;
}
