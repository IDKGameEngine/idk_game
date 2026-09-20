#pragma once

#include "idk/engine/IApplication.hpp"
// #include "libidk/dsa/PeriodicTimer.hpp"


namespace mygame
{
    class Game: public idk::IApplication
    {
    private:

    public:
        Game();
        virtual void onInit(idk::Engine&) final;
        virtual void onUpdate(idk::Engine&) final;
        virtual void onShutdown(idk::Engine&) final;

    };
}
