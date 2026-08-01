#include "idk_engine/Engine.hpp"
#include <SDL3/SDL.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_CreateWindow("send_ctrl", 256, 256, 0);

    idk::EngineControlData data;
    // idk::PortTxer<idk::RemoteTxer, idk::EngineControlData> port("127.0.0.1", 5001);
    idk::PortTxer<idk::LocalTxer, idk::EngineControlData> port("EngineCtrlPort");

    bool running = true;
    bool dirty = false;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            if (e.type == SDL_EVENT_KEY_UP)
            {
                if (e.key.scancode == SDL_SCANCODE_X) { VLOG_INFO("SEND X"); data.x=(data.x+1)%2; dirty=true; }
                if (e.key.scancode == SDL_SCANCODE_Y) { VLOG_INFO("SEND Y"); data.y=(data.y+1)%2; dirty=true; }
                if (e.key.scancode == SDL_SCANCODE_Z) { VLOG_INFO("SEND Z"); data.z=(data.z+1)%2; dirty=true; }
            }
        }

        if (dirty)
        {
            dirty = false;
            port.sendData(data);
        }
    }

    return 0;
}
