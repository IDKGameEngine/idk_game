#pragma once

#include "idk/engine/Engine.hpp"
#include "libidk/dsa/PeriodicTimer.hpp"


namespace idk
{
    class GameService: public idk::EngineComponent
    {
    private:

    public:
        GameService(EngineContext&);
        virtual void update() final;

    };
}
