#pragma once

#include "idk_core/Engine.hpp"
#include "idk_core/Service.hpp"
#include "idk_core/Timer.hpp"
#include "idk_gfx/controller.hpp"


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
