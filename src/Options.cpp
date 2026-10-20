#include "Options.hpp"
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

namespace
{
    bool number(const char *text, uint32_t &value)
    {
        auto result = std::from_chars(text, text + std::strlen(text), value);
        return result.ec == std::errc{} && !*result.ptr && value;
    }
    bool path(char (&output)[4096], const char *base, const char *suffix)
    {
        int length = std::snprintf(output, sizeof(output), "%s/%s", base, suffix);
        return length > 0 && size_t(length) < sizeof(output);
    }
}

void mygame::Options::usage()
{
    std::puts("Usage: game [--connect HOST | --server] [--port N] [--level FILE]\n"
              "            [--level-root DIRECTORY] [--config FILE] [--frames N]\n"
              "       game_server [--bind ADDRESS] [--port N] [--level FILE]\n"
              "                   [--level-root DIRECTORY] [--config FILE] [--ticks N]\n"
              "Without --connect or --server, starts a private local server process.\n"
              "--frames limits client frames after joining; --ticks limits server ticks.\n"
              "Default content is the model demo. --level selects the networked level game.\n"
              "WASD move, Shift run, Space jump, R restart, Tab cursor, Esc quit. F5 reloads timing.");
}

bool mygame::Options::parse(int argc, char **argv, bool dedicated)
{
    mode = dedicated ? Mode::Server : Mode::Local;
    const char *levelPath = nullptr, *rootPath = nullptr, *configPath = nullptr;
    bool selected = false;
    for (int i = 1; i < argc; ++i)
    {
        const char *arg = argv[i];
        if (!std::strcmp(arg, "--help")) { help = true; return true; }
        if (!std::strcmp(arg, "--server") && !selected)
        {
            mode = Mode::Server; selected = true;
        }
        else if (!std::strcmp(arg, "--connect") && !dedicated && !selected && i + 1 < argc)
        {
            mode = Mode::Connect; selected = true; host = argv[++i];
            if (!*host) return false;
        }
        else if (!std::strcmp(arg, "--level") && !levelPath && i + 1 < argc) levelPath = argv[++i];
        else if (!std::strcmp(arg, "--level-root") && !rootPath && i + 1 < argc) rootPath = argv[++i];
        else if (!std::strcmp(arg, "--config") && !configPath && i + 1 < argc) configPath = argv[++i];
        else if (!std::strcmp(arg, "--bind") && !binding && i + 1 < argc) binding = argv[++i];
        else if (!std::strcmp(arg, "--frames") && !frames && i + 1 < argc)
        {
            if (!number(argv[++i], frames)) return false;
        }
        else if (!std::strcmp(arg, "--ticks") && !ticks && i + 1 < argc)
        {
            if (!number(argv[++i], ticks)) return false;
        }
        else if (!std::strcmp(arg, "--port") && !explicitPort && i + 1 < argc)
        {
            uint32_t value = 0;
            if (!number(argv[++i], value) || value > 65535) return false;
            port = uint16_t(value); explicitPort = true;
        }
        else if (!std::strcmp(arg, "--ready-fd") && readyFd < 0 && i + 1 < argc)
        {
            uint32_t value = 0;
            if (!number(argv[++i], value) || value > 1024) return false;
            readyFd = int(value);
        }
        else { std::fprintf(stderr, "Invalid argument: %s\n", arg); return false; }
    }
    if ((rootPath && !levelPath) || (mode == Mode::Server && frames) ||
        (mode != Mode::Server && (ticks || binding || readyFd >= 0))) return false;
    char base[4096];
    ssize_t length = readlink("/proc/self/exe", base, sizeof(base) - 1);
    if (length <= 0 || size_t(length) >= sizeof(base) - 1) return false;
    base[length] = '\0';
    char *slash = std::strrchr(base, '/');
    if (!slash) return false;
    *slash = '\0';
    if (!path(serverExecutable, base, IDK_SERVER_FILENAME) || !path(demo, base, "content/model/sample/instanced-cubes.glb"))
        return false;
    if (levelPath && !realpath(levelPath, level)) return false;
    if (rootPath) { if (!realpath(rootPath, root)) return false; }
    else if (!path(root, base, "content")) return false;
    explicitConfig = configPath != nullptr;
    if (configPath) { if (!realpath(configPath, config)) return false; }
    else if (!path(config, base, "engine.cfg")) return false;
    return true;
}
