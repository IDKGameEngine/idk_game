#include "idk/engine/Engine.hpp"
#include "idk/editor/Editor.hpp"
#include "Game.hpp"


int main(int argc, char **argv)
{
    (void)argc; (void)argv;

    idk::Engine engine;
    mygame::Game game;
    engine.run(&game);

    return 0;
}
