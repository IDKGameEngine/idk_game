#pragma once

#include "idk/engine/IApplication.hpp"


namespace mygame
{
    class GameImpl: public idk::IApplication
    {
    private:

    public:
        virtual void onInit(idk::EngineAPI&) final;
        virtual void onShutdown(idk::EngineAPI&) final;
        virtual void onPreFrame(idk::EngineAPI&) final;
        virtual void onPostFrame(idk::EngineAPI&) final;
        virtual void onEvent(idk::EngineAPI&, const void*) final;

    };
}
