#pragma once

#include "libidk/Service.hpp"
// #include "libidk/dsa/PeriodicTimer.hpp"


namespace idk
{
    class GameService: public idk::Service
    {
    private:

    public:
        GameService();
        virtual void init(idk::ServiceManager*) final;
        virtual void update(idk::ServiceManager*) final;

    };
}
