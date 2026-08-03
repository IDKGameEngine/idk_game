#include "idk/engine/Engine.hpp"
#include "libidk/message/RemoteRxTx.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    // std::srand(clock());

    // idk::EngineStatData prevData;
    // idk::EngineStatData statusData;
    // idk::PeriodicTimer statusTimer(8);
    // idk::RemoteRxer statRx(5002);
    // // idk::SharedRxer statRx("IDKGameEngineIPC-EngineStatus", sizeof(idk::EngineStatData));

    // int count = 0;
    // while (count < 10)
    // {
    //     if (statusTimer.expired())
    //     {
    //         statusTimer.reset();
    //         if (!statRx.recvMsg(statusData))
    //         {
    //             continue;
    //         }
    //         if (statusData.allocatorMemoryUsage != prevData.allocatorMemoryUsage) VLOG_INFO("AllocatorMemoryUsage == {}", statusData.allocatorMemoryUsage);
    //         if (statusData.x != prevData.x) VLOG_INFO("StatusData.x == {}", statusData.x);
    //         if (statusData.y != prevData.y) VLOG_INFO("StatusData.y == {}", statusData.y);
    //         if (statusData.z != prevData.z) VLOG_INFO("StatusData.z == {}", statusData.z);
    //         prevData = statusData;
    //     }
    // }

    return 0;
}
