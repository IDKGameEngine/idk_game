#pragma once

#include <cstdint>

namespace mygame
{
    enum class Mode { Local, Server, Connect };
    struct Options
    {
        Mode mode = Mode::Local;
        const char *host = nullptr, *binding = nullptr;
        char level[4096]{}, root[4096]{}, config[4096]{}, demo[4096]{}, serverExecutable[4096]{};
        uint16_t port = 28080;
        uint32_t frames = 0, ticks = 0;
        int readyFd = -1;
        bool help = false, explicitConfig = false, explicitPort = false;
        bool parse(int argc, char **argv, bool dedicated = false);
        static void usage();
    };
}
