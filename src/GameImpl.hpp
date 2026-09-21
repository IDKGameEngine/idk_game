#pragma once

#include "idk/engine/IApplication.hpp"


namespace mygame
{
    class GameImpl: public idk::IApplication
    {
    private:

    public:
        GameImpl();

        virtual void onInit(idk::EngineAPI&) final;
        virtual void onUpdate(idk::EngineAPI&) final;
        virtual void onShutdown(idk::EngineAPI&) final;
        virtual void onEvent(idk::EngineAPI&, const void*) final;

    };
}
