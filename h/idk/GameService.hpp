#pragma once

#include "idk/core/Engine.hpp"
#include "idk/core/Service.hpp"
#include "idk/core/Timer.hpp"

#include "idk/GfxService.hpp"
#include "idk/gfx/controller.hpp"


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
