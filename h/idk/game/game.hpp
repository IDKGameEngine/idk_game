#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/game.hpp"


namespace idk::game
{
    void main(idk::core::IEngine*);

    class Game;
}


class idk::game::Game: public idk::core::IGame
{
public:
    Game();
    virtual void update() final;

};
