#include "idk/UdpService.hpp"
#include <iostream>

enum UserCmd: uint8_t
{
    None,
    SendPacket,
    SendMultiPacket,
    Quit
};


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

    uint16_t SERVER_PORT = 4001;
    NET_DatagramSocket *udpSocket = NET_CreateDatagramSocket(NULL, 0, 0);
    NET_Address *serverAddr = NET_ResolveHostname("127.0.0.1");

    if (!serverAddr)
    {
        SDL_Log("Failed to resolve host: %s", SDL_GetError());
        return -1;
    }

    bool running = true;
    while (running)
    {
        UserCmd usercmd = UserCmd::None;
        std::string strcmd = "None";

        std::cout << "> ";
        std::cin >> strcmd;

        if (strcmd == "SendPacket")
        {
            usercmd = UserCmd::SendPacket;
        }
        if (strcmd == "SendMultiPacket")
        {
            usercmd = UserCmd::SendMultiPacket;
        }
        if (strcmd == "Quit")
        {
            usercmd = UserCmd::Quit;
        }


        int repCount = 0;
        const char *payload = "Packet";
        int payloadSize = (int)strlen(payload) + 1;

        switch (usercmd)
        {
            case UserCmd::SendPacket:
                repCount = 1;
                break;
            case UserCmd::SendMultiPacket:
                repCount = 10;
                break;
            case UserCmd::Quit:
                running = false;
                break;
            default:
                std::cout << "Unknown command\n";
                break;
        }

        for (int i=0; i<repCount; i++)
        {
            if (!NET_SendDatagram(udpSocket, serverAddr, SERVER_PORT, payload, payloadSize))
            {
                SDL_Log("Failed to send datagram: %s", SDL_GetError());
            }
        }

        // SDL_Event e;
        // while (SDL_PollEvent(&e))
        // {
        //     if (e.type == SDL_EVENT_QUIT)
        //     {
        //         running = false;
        //     }
        // }

        // SDL_Delay(100);
    }

    NET_Quit();
    SDL_Quit();

    return 0;
}
