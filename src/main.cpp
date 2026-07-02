#include "idk/engine.hpp"

#include "idk/core/cfgparser.hpp"
#include "idk/core/file.hpp"
#include "idk/core/threadpool.hpp"
#include "idk/engine.hpp"
#include "idk/game_service.hpp"
#include "idk/gfx_service.hpp"
#include "idk/io_service.hpp"

alignas(max_align_t) static uint8_t gfxSrvBuf[sizeof(idk::GfxService)];
alignas(max_align_t) static uint8_t ioSrvBuf[sizeof(idk::IoService)];
alignas(max_align_t) static uint8_t gameSrvBuf[sizeof(idk::GameService)];


// struct testArgType
// {
//     size_t offset;
//     size_t length;
//     uint8_t *data;
// };

// static void testFunc(void *ptr)
// {
//     auto &arg = *(testArgType*)ptr;
//     VLOG_INFO("[testFunc] offset={}", arg.offset);
//     for (size_t i=0; i<arg.length; i++)
//     {
//         arg.data[arg.offset + i] = 0xA0;
//     }
// }


int idk::platform::AppEntry(int argc, char **argv)
{
    VLOG_INFO("[idk::platform::AppEntry]");

    (void)argc;
    (void)argv;

    // static idk::core::ThreadPool threadpool;
    // static uint8_t buf[2048];
    // static testArgType argbuf[8];
    // for (int i=0; i<8; i++)
    // {
    //     argbuf[i].offset = i*256;
    //     argbuf[i].length = 256;
    //     argbuf[i].data = buf;
    //     threadpool.createTask(testFunc, &argbuf[i]);
    // }

    auto *gfxSrv = new (gfxSrvBuf) idk::GfxService();
    auto *ioSrv = new (ioSrvBuf) idk::IoService();
    auto *gameSrv = new (gameSrvBuf) idk::GameService(gfxSrv);

    idk::Engine engine({ioSrv, gameSrv, gfxSrv});

    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
