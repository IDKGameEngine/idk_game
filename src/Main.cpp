#include "idk/engine/Engine.hpp"
#include "GameImpl.hpp"
#include "idk/level/Playtest.hpp"
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits.h>


int main(int argc, char **argv)
{
    const char* levelPath = nullptr;
    uint32_t frames = 0;
    for (int i = 1; i < argc; ++i)
    {
        if (!std::strcmp(argv[i], "--help"))
        {
            std::puts("Usage: game [--level path.idklvl] [--frames N]\nWithout --level, runs the model demo.");
            return 0;
        }
        if (!std::strcmp(argv[i], "--level") && i + 1 < argc && !levelPath)
            levelPath = argv[++i];
        else if (!std::strcmp(argv[i], "--frames") && i + 1 < argc && !frames)
        {
            const char* value = argv[++i];
            auto result = std::from_chars(value, value + std::strlen(value), frames);
            if (result.ec != std::errc{} || *result.ptr || !frames)
            {
                std::fputs("--frames requires a positive integer.\n", stderr);
                return 2;
            }
        }
        else
        {
            std::fprintf(stderr, "Invalid argument: %s\n", argv[i]);
            return 2;
        }
    }
    if (frames && !levelPath) { std::fputs("--frames requires --level.\n", stderr); return 2; }
    char absolute[PATH_MAX];
    if (levelPath)
    {
        if (!realpath(levelPath, absolute)) { std::perror("Runtime level path"); return 1; }
        idk::Engine engine;
        idk::level::Playtest playtest(absolute, frames);
        engine.startApplication(&playtest);
        return playtest.failed() ? 1 : 0;
    }

    idk::Engine engine;
    mygame::GameImpl game;
    engine.startApplication(&game);

    return 0;
}
