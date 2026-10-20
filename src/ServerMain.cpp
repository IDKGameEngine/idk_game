#include "GameServer.hpp"

int main(int argc, char **argv)
{
    mygame::Options options;
    if (!options.parse(argc, argv, true)) { options.usage(); return 2; }
    if (options.help) { options.usage(); return 0; }
    return mygame::runServer(options);
}
