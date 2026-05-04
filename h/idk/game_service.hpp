#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/service.hpp"
#include "idk/core/periodic_timer.hpp"

#include "idk/gfx_api.hpp"
#include "idk/gfx/controller.hpp"


namespace idk
{
    class GameService: public idk::core::Service
    {
    private:
        idk::GfxApi mGfx;
        idk::TestCharacterController mCtl;

    public:
        GameService(const idk::GfxApi&);
        virtual void _startup(idk::IEngine*) final;
        virtual void _update(idk::IEngine*) final;
        virtual void _shutdown(idk::IEngine*) final;

    };
}
