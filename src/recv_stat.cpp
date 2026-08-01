#include "idk_engine/Engine.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    idk::EngineStatusData prevData;
    idk::EngineStatusData statusData;
    idk::PeriodicTimer statusTimer(8);
    idk::PortRxer<idk::RemoteRxer, idk::EngineStatusData> statusPort(5002);

    int count = 0;
    while (count < 10)
    {
        if (statusTimer.expired())
        {
            statusTimer.reset();
            if (!statusPort.readData(statusData))
            {
                continue;
            }
            if (statusData.x != prevData.x) VLOG_INFO("StatusData.x == {}", statusData.x);
            if (statusData.y != prevData.y) VLOG_INFO("StatusData.y == {}", statusData.y);
            if (statusData.z != prevData.z) VLOG_INFO("StatusData.z == {}", statusData.z);
            prevData = statusData;
        }
    }

    return 0;
}
