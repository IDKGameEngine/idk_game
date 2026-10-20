#pragma once

#include "Options.hpp"
#include "libidk/Types.hpp"
#include <sys/types.h>

namespace mygame
{
    class LocalServer: public idk::Immobile
    {
    public:
        ~LocalServer();
        bool start(const Options &, uint16_t &port);
        bool alive();
        void reload();
        void stop();
    private:
        pid_t mPid = -1;
    };
}
