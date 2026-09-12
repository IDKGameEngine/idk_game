#include "libidk/platform/PlatformContext.hpp"
#include "libidk/platform/SDL3Events.hpp"
#include "libidk/platform/SDL3FileSystem.hpp"
#include "libidk/platform/SDL3Input.hpp"
#include "libidk/platform/SDL3Time.hpp"
#include "libidk/platform/SDL3Video.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    idk::PlatformContext ctx;
    ctx.giveFeature<idk::SDL3Events>();
    ctx.giveFeature<idk::SDL3Time>();
    ctx.giveFeature<idk::SDL3FileSystem>();
    ctx.giveFeature<idk::SDL3Video>("GameWindow", 1280, 720);

    while (ctx.running())
    {
        ctx.update();
    }

    return 0;
}
