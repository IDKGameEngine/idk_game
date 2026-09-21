#pragma once

#include "libidk/Service.hpp"


namespace mygame
{
    class GameImpl: public idk::Service
    {
    private:

    public:
        virtual void onInit(idk::EngineAPI&) final;
        virtual void onShutdown(idk::EngineAPI&) final;
        virtual void onUpdate(idk::EngineAPI&) final;
        virtual void onFixedUpdate(idk::EngineAPI&) final;

    };
}
