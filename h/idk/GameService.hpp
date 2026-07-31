#pragma once

#include "libidk/Engine.hpp"
#include "libidk/Service.hpp"
#include "libidk/Timer.hpp"

#include "idk/GfxService.hpp"
#include "idk/gfx/Controller.hpp"


namespace idk
{
    class GameService: public idk::core::Service
    {
    private:
        idk::TestCharacterController mCtl;
        idk::PeriodicTimer mTimer;;

    public:
        GameService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    };
}
