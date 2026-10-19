#pragma once

#include "libidk/Service.hpp"
#include "idk/gfx/RenderTypes.hpp"


namespace mygame
{
    class GameImpl: public idk::Service
    {
    private:
        idk::gfx::ModelHandle mModel;
        float mTime = 0.0f;

    public:
        virtual void onInit(idk::EngineAPI&) final;
        virtual void onShutdown(idk::EngineAPI&) final;
        virtual void onUpdate(idk::EngineAPI&) final;
        virtual void onFixedUpdate(idk::EngineAPI&) final;
        void onPreRender(idk::EngineAPI&) override;

    };
}
