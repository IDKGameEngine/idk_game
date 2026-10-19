#pragma once

#include "libidk/Service.hpp"
#include "idk/gfx/RenderTypes.hpp"

namespace mygame
{
    class GameImpl final: public idk::Service
    {
    private:
        idk::gfx::ModelHandle mModel;
        float mTime = 0.0f;

    public:
        void onInit(idk::EngineAPI &) override;
        void onShutdown(idk::EngineAPI &) override;
        void onPreRender(idk::EngineAPI &) override;
    };
}
