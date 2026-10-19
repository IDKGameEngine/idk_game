#include "GameImpl.hpp"
#include "idk/engine/Engine.hpp"
#include "idk/platform/AudioManager.hpp"
#include "idk/gfx/RenderEngine.hpp"
#include "idk/asset/ModelImporter.hpp"
#include <SDL3/SDL.h>

#include <glm/gtc/matrix_transform.hpp>

void mygame::GameImpl::onInit(idk::EngineAPI &api)
{
    auto *audio = api.mAudio;
    const auto sound = audio->createSound("audio/hurt3.wav");
    audio->startSound(sound);

    auto &renderer = *api.getService<idk::gfx::RenderEngine>();
    if (!renderer.IsInitialized())
    {
        api.quit();
        return;
    }
    // Explicit loading budgets. These buffers can be shared with other load jobs;
    // they are not part of the per-frame path or the engine's service allocator.
    alignas(std::max_align_t) static std::byte modelBytes[4 * 1024 * 1024];
    alignas(std::max_align_t) static std::byte scratchBytes[8 * 1024 * 1024];
    idk::Arena output(modelBytes), scratch(scratchBytes);
    idk::asset::Model model;
    idk::asset::ImportError error;
    if (!idk::asset::ImportModel("model/sample/instanced-cubes.glb", output, scratch, model, error))
    {
        idk::vlog(idk::LogType::ERROR, "Model", "%s", error.message);
        api.quit();
        return;
    }
    mModel = renderer.UploadModel(model);
    output.reset(); // GPU upload owns its resources; no CPU references are retained.
    if (!mModel.isValid())
        api.quit();
}

void mygame::GameImpl::onShutdown(idk::EngineAPI &api)
{
    auto &renderer = *api.getService<idk::gfx::RenderEngine>();
    renderer.DestroyModel(mModel);
}

void mygame::GameImpl::onPreRender(idk::EngineAPI &api)
{
    auto &renderer = *api.getService<idk::gfx::RenderEngine>();
    mTime += float(api.getDeltaTimeSec());
    int width = 1, height = 1;
    SDL_GetWindowSizeInPixels(static_cast<SDL_Window *>(renderer.GetWindowHandle()), &width, &height);
    if (width <= 0 || height <= 0)
        return;
    idk::gfx::Camera camera;
    camera.view = glm::lookAt(glm::vec3(0, 1.4f, 4.5f), glm::vec3(0), glm::vec3(0, 1, 0));
    camera.proj = glm::perspective(glm::radians(50.0f), float(width) / float(height), 0.1f, 100.0f);
    renderer.SetCamera(camera);
    renderer.SubmitModel(mModel, glm::rotate(glm::mat4(1), mTime * .3f, glm::vec3(0, 1, 0)));
}
