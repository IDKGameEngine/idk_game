#include "idk/UdpService.hpp"
#include "idk/engine/UdpTxer.hpp"
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (!SDL_Init(SDL_INIT_EVENTS))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }


    bool running = true;
    idk::UdpTxer txer(4001);

    while (running)
    {
        static char usercmd[256];
        static char usermsg[256];

        memset(usercmd, 0, sizeof(usercmd));
        strcpy(usercmd, "quit");

        printf("> ");
        scanf("%s", usercmd);

        if (strcmp(usercmd, "quit") == 0)
        {
            break;
        }

        if (strcmp(usercmd, "send") == 0)
        {
            memset(usermsg, 0, sizeof(usermsg));
            scanf("%[^\n]", usermsg);
            txer.sendmsg(usermsg, strlen(usermsg) + 1);
        }
    }

    NET_Quit();
    SDL_Quit();

    return 0;
}
