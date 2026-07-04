// #include "idk_engine/UdpClientService.hpp"
// #include "idk/core/Platform.hpp"
// #include "idk_engine/UdpTxer.hpp"
// #include "idk_engine/config/udp/TimeSyncAddress.hpp"
// #include <iostream>

// #include <SDL3/SDL.h>
// #include <SDL3_net/SDL_net.h>

// int main(int argc, char **argv)
// {
//     (void)argc;
//     (void)argv;

//     if (!SDL_Init(SDL_INIT_EVENTS))
//     {
//         VLOG_FATAL("{}", SDL_GetError());
//     }
//     if (!NET_Init())
//     {
//         VLOG_FATAL("{}", SDL_GetError());
//     }


//     bool running = true;
//     static idk::config::udp::TimeSyncData timeSyncData;
//     idk::UdpTxer2<idk::config::udp::ServerTimeSyncAddress> timeSyncTxer;
//     // idk::UdpRxer2<idk::config::udp::ClientTimeSyncAddress> timeSyncRxer;

//     while (running)
//     {
//         // static char usercmd[256];
//         // static char usermsg[256];

//         // memset(usercmd, 0, sizeof(usercmd));
//         // strcpy(usercmd, "quit");

//         // printf("> ");
//         // (void)scanf("%s", usercmd);

//         // if (strcmp(usercmd, "quit") == 0)
//         // {
//         //     break;
//         // }

//         // if (strcmp(usercmd, "send") == 0)
//         // {
//         //     memset(usermsg, 0, sizeof(usermsg));
//         //     (void)scanf("%[^\n]", usermsg);
//         //     txer.sendmsg(usermsg, strlen(usermsg) + 1);
//         // }
//         timeSyncData.clientSendTime = idk::Platform::getSysTimeMs();
//         if (timeSyncTxer.sendmsg(timeSyncData))
//         {
//             // while (!timeSyncRxer.recvmsg(timeSyncData))
//             // {

//             // }

//             uint64_t clientSendTime = timeSyncData.clientSendTime;
//             uint64_t serverSendTime = timeSyncData.serverSendTime;
//             VLOG_INFO("clientSendTime, serverSendTime: {}, {}", clientSendTime, serverSendTime);
//         }


//         SDL_Delay(200);
//     }

//     NET_Quit();
//     SDL_Quit();

//     return 0;
// }
