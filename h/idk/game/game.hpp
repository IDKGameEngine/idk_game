#pragma once

#include "idk/core/engine.hpp"
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
        idk::TestCharacterController mInput;
        glm::vec3 dmove;
        glm::vec3 dlook;

    public:
        Game(const idk::GfxApi&);
        virtual void _startup(idk::IEngine*) final;
        virtual void _update(idk::IEngine*) final;
        virtual void _shutdown(idk::IEngine*) final;

    };
}
