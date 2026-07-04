#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/periodic_timer.hpp"

#include "idk/GfxService.hpp"
#include "idk/gfx/controller.hpp"


namespace idk
{
    class GameService: public idk::core::Service
    {
    private:
        idk::TestCharacterController mCtl;
        idk::PeriodicTimer timer_;

    public:
        GameService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    };
}
