#pragma once

#include "idk/engine/Engine.hpp"
#include "libidk/dsa/PeriodicTimer.hpp"


namespace idk
{
    class GameService: public idk::EngineService
    {
    private:

    public:
        GameService(EngineContext&);
        virtual void init(idk::Engine&) final;
        virtual void update(idk::Engine&) final;

    };
}
