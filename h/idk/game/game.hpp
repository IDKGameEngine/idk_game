#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/game.hpp"
#include "idk/core/service.hpp"
#include "idk/core/periodic_timer.hpp"
#include "idk/engine/controller.hpp"

#include "idk/gfxapi.hpp"


namespace idk
{
    class Game: public idk::core::Service
    {
    private:
        idk::GfxApi mGfx;
        idk::PeriodicTimer<60> mTimer;
        idk::TestCharacterController mInput;
        glm::vec3 dmove;
        glm::vec3 dlook;

    public:
        Game(const idk::GfxApi&);
        virtual void onUpdate(idk::IEngine*) final;
        virtual void onShutdown(idk::IEngine*) final;

    };
}
