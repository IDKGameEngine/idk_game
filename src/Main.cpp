#include "idk/engine/Engine.hpp"
#include "GameImpl.hpp"


int main(int argc, char **argv)
{
    (void)argc; (void)argv;

    idk::Engine engine;
    mygame::GameImpl game;
    engine.run(&game);

    return 0;
}
