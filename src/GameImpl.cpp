#include "GameImpl.hpp"
#include "idk/EngineAPI.hpp"
#include "idk/gfx/RenderEngine.hpp"
#include "idk/asset/ModelImporter.hpp"
#include "idk/asset/PrimitiveShape.hpp"
#include <SDL3/SDL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>

using namespace mygame;

namespace
{
    glm::vec3 position(const idk::net::Transform &t) { return {t.position[0], t.position[1], t.position[2]}; }
    glm::quat rotation(const idk::net::Transform &t) { return {t.rotation[3], t.rotation[0], t.rotation[1], t.rotation[2]}; }
    glm::mat4 matrix(const idk::net::Transform &t)
    {
        return glm::translate(glm::mat4(1), position(t)) * glm::mat4_cast(rotation(t));
    }
}

void GameImpl::fail(idk::EngineAPI &api, const char *message)
{
    std::fprintf(stderr, "Game client: %s\n", message);
    mFailed = true;
    api.quit();
}

void GameImpl::onInit(idk::EngineAPI &api)
{
    auto *renderer = api.getService<idk::gfx::RenderEngine>();
    if (!renderer || !renderer->IsInitialized()) { fail(api, "Renderer initialization failed"); return; }
    mFixedRate = api.getTimingConfig().fixedRateHz;
    mPhysicsRate = api.getTimingConfig().physicsRateHz;
    idk::Arena output(mStorage.levels[0]), scratch(mStorage.scratch);
    if (!mContent.isLevel())
    {
        idk::asset::Model model;
        idk::asset::ImportError error;
        if (!idk::asset::ImportModel(mContent.demoPath(), output, scratch, model, error)) { fail(api, error.message); return; }
        mModel = renderer->UploadModel(model);
        if (!mModel.isValid() || !mModels.bind(DemoModel, mModel)) { fail(api, "Could not upload demo model"); return; }
    }
    else
    {
        idk::asset::Primitive box;
        if (!idk::asset::CreateShape(idk::asset::ShapeKind::Box, scratch, box)) { fail(api, "Could not create player geometry"); return; }
        const idk::gfx::VertexAttribute attributes[] = {
            {0, idk::gfx::VertexAttribType::Float3, offsetof(idk::asset::Vertex, position)},
            {1, idk::gfx::VertexAttribType::Float2, offsetof(idk::asset::Vertex, uv)},
            {4, idk::gfx::VertexAttribType::Float4, offsetof(idk::asset::Vertex, color)}};
        mBox = renderer->CreateMesh({box.vertices.data(), box.vertices.size(), sizeof(idk::asset::Vertex),
                                    box.indices.data(), box.indices.size(), attributes});
        idk::gfx::MaterialDesc material;
        material.shader = renderer->GetDefaultShader();
        material.baseColor = {.2f, .65f, 1, 1};
        mPlayerMaterial = renderer->CreateMaterial(material);
        material.baseColor = {1, .45f, .1f, .35f};
        material.blend = idk::gfx::BlendMode::Alpha;
        material.doubleSided = true;
        material.depthWrite = false;
        mExitMaterial = renderer->CreateMaterial(material);
        if (!mBox.isValid() || !mPlayerMaterial.isValid() || !mExitMaterial.isValid())
        { fail(api, "Could not upload player/exit resources"); return; }
    }
    renderer->SetClearColor({.075f, .085f, .105f, 1});
    SDL_SetWindowTitle(static_cast<SDL_Window *>(renderer->GetWindowHandle()), "IDK Game - connecting to server");
    if (!mClient.connect(mOptions.mode == Mode::Connect ? mOptions.host : "127.0.0.1", mPort,
                         mContent.manifest(), idk::net::nowSeconds())) fail(api, mClient.error());
}

bool GameImpl::loadScene(idk::gfx::RenderEngine &renderer, size_t index)
{
    unsigned bank = mScene ? 1 - mBank : 0;
    idk::Arena output(mStorage.levels[bank]), scratch(mStorage.scratch);
    idk::level::Level candidate;
    idk::level::Error error;
    idk::asset::Model model;
    if (!mContent.load(index, output, candidate, error) || !candidate.model(scratch, model, error))
    {
        std::fprintf(stderr, "Client scene load: %s\n", error.message); return false;
    }
    auto uploaded = renderer.UploadModel(model);
    if (!uploaded.isValid()) return false;
    renderer.DestroyModel(mModel);
    mModel = uploaded;
    mLevel = candidate;
    mBank = bank;
    return true;
}

void GameImpl::onShutdown(idk::EngineAPI &api)
{
    mClient.disconnect();
    mModels.clear();
    if (auto *renderer = api.getService<idk::gfx::RenderEngine>())
    {
        if (renderer->GetWindowHandle()) SDL_SetWindowRelativeMouseMode(static_cast<SDL_Window *>(renderer->GetWindowHandle()), false);
        renderer->DestroyModel(mModel);
        renderer->DestroyMesh(mBox);
        renderer->DestroyMaterial(mPlayerMaterial);
        renderer->DestroyMaterial(mExitMaterial);
    }
}

void GameImpl::onUpdate(idk::EngineAPI &api)
{
    if (mFailed) return;
    if (mLocal && !mLocal->alive()) { fail(api, "Local server exited"); return; }
    const auto &timing = api.getTimingConfig();
    if (mLocal && (timing.fixedRateHz != mFixedRate || timing.physicsRateHz != mPhysicsRate))
    {
        mLocal->reload();
        mFixedRate = timing.fixedRateHz; mPhysicsRate = timing.physicsRateHz;
    }
    double now = idk::net::nowSeconds();
    if (!mClient.poll(now))
    {
        if (mClient.rejection() == idk::net::RejectReason::ManifestMismatch) fail(api, "Server has different game content");
        else if (mClient.rejection() == idk::net::RejectReason::ServerFull) fail(api, "Server is full");
        else fail(api, "Server disconnected or timed out");
        return;
    }
    auto &renderer = *api.getService<idk::gfx::RenderEngine>();
    auto *window = static_cast<SDL_Window *>(renderer.GetWindowHandle());
    const auto &snapshot = mClient.snapshot();
    const idk::net::EntityState *own = nullptr, *scene = nullptr;
    bool completed = false;
    for (const auto &entity: std::span(snapshot.entities, snapshot.count))
    {
        if (entity.owner == mClient.id() && entity.owner) own = &entity;
        if (entity.resource >= SceneBase && entity.resource < SceneBase + Content::MaxLevels) scene = &entity;
        if (entity.resource == Completed) completed = true;
    }
    if (scene && scene->id != mScene)
    {
        if (!mContent.isLevel() || !loadScene(renderer, scene->resource - SceneBase))
        { fail(api, "Could not load the server's scene from local content"); return; }
        mScene = scene->id;
    }
    if (own && own->id != mOwnEntity)
    {
        mOwnEntity = own->id;
        mYaw = std::remainder(glm::degrees(2 * std::atan2(own->transform.rotation[1], own->transform.rotation[3])), 360.0f);
        mPitch = mContent.isLevel() ? std::clamp(mLevel.entry.rotation.x, -85.0f, 85.0f) : 0;
        SDL_SetWindowTitle(window, "IDK Game - WASD move | Shift run | Space jump | R restart | Tab cursor | Esc close");
        std::printf("Client %u joined; authoritative entity %llu\n", mClient.id(), static_cast<unsigned long long>(own->id));
    }
    if (completed != mComplete)
    {
        mComplete = completed;
        SDL_SetWindowTitle(window, completed ? "Exit reached! - R restart | Esc close" : "IDK Game - WASD move | R restart | Esc close");
    }
    if (snapshot.revision != mRevision)
    {
        if (mSnapshotAt) mSnapshotInterval = std::clamp(now - mSnapshotAt, .001, .25);
        mSnapshotAt = now; mRevision = snapshot.revision;
    }
    const bool *keys = SDL_GetKeyboardState(nullptr);
    bool focused = (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS) != 0;
    if (focused && keys[SDL_SCANCODE_ESCAPE]) api.quit();
    bool tab = focused && keys[SDL_SCANCODE_TAB];
    if (tab && !mTabHeld) mCaptured = !mCaptured;
    mTabHeld = tab;
    if (!focused) mCaptured = false;
    if (focused && !mCaptured && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_LMASK)) mCaptured = true;
    SDL_SetWindowRelativeMouseMode(window, mContent.isLevel() && mCaptured);
    float mouseX = 0, mouseY = 0;
    SDL_GetRelativeMouseState(&mouseX, &mouseY);
    idk::net::InputState input;
    if (focused && (!mContent.isLevel() || mCaptured))
    {
        if (mContent.isLevel())
        {
            mYaw = std::remainder(mYaw - mouseX * .12f, 360.0f);
            mPitch = std::clamp(mPitch - mouseY * .12f, -85.0f, 85.0f);
        }
        input.axes[0] = float(int(keys[SDL_SCANCODE_D]) - int(keys[SDL_SCANCODE_A]));
        input.axes[1] = float(int(keys[SDL_SCANCODE_W]) - int(keys[SDL_SCANCODE_S]));
        if (keys[SDL_SCANCODE_SPACE]) input.buttons |= Jump;
        if (keys[SDL_SCANCODE_LSHIFT]) input.buttons |= Sprint;
    }
    if (focused && keys[SDL_SCANCODE_R]) input.buttons |= Restart;
    mPendingButtons |= input.buttons & (Jump | Restart);
    if (!focused) mPendingButtons = 0;
    input.axes[2] = mYaw / 180.0f;
    if (own && now >= mNextInput)
    {
        input.buttons |= mPendingButtons;
        if (!mClient.sendInput(own->id, input)) { fail(api, "Could not send input to server"); return; }
        mPendingButtons = 0;
        mNextInput = now + 1.0 / 60;
    }
}

idk::net::Transform GameImpl::interpolated(const idk::net::EntityState &entity, double now) const
{
    const auto *previous = mClient.previousSnapshot().find(entity.id);
    if (!previous) return entity.transform;
    float alpha = float(std::clamp((now - mSnapshotAt) / mSnapshotInterval, 0.0, 1.0));
    auto p = glm::mix(position(previous->transform), position(entity.transform), alpha);
    auto q = glm::slerp(rotation(previous->transform), rotation(entity.transform), alpha);
    return {{p.x, p.y, p.z}, {q.x, q.y, q.z, q.w}};
}

void GameImpl::onPreRender(idk::EngineAPI &api)
{
    if (mFailed || !mOwnEntity || (mContent.isLevel() && !mScene)) return;
    const auto *own = mClient.snapshot().find(mOwnEntity);
    if (!own) return;
    auto &renderer = *api.getService<idk::gfx::RenderEngine>();
    int width = 1, height = 1;
    SDL_GetWindowSizeInPixels(static_cast<SDL_Window *>(renderer.GetWindowHandle()), &width, &height);
    if (width <= 0 || height <= 0) return;
    double now = idk::net::nowSeconds();
    auto feet = position(interpolated(*own, now));
    idk::gfx::Camera camera;
    if (mContent.isLevel())
    {
        float yaw = glm::radians(mYaw), pitch = glm::radians(mPitch);
        auto eye = feet + glm::vec3(0, 1.6f, 0);
        glm::vec3 forward(-std::sin(yaw) * std::cos(pitch), std::sin(pitch), -std::cos(yaw) * std::cos(pitch));
        camera.view = glm::lookAt(eye, eye + forward, glm::vec3(0, 1, 0));
        camera.proj = glm::perspectiveRH_NO(glm::radians(70.0f), float(width) / height, .05f, 20000.0f);
        if (!renderer.SubmitModel(mModel) || !renderer.Submit({mBox, mExitMaterial, mLevel.exit.transform()}))
        { fail(api, "Could not render scene"); return; }
    }
    else
    {
        camera.view = glm::lookAt(feet + glm::vec3(0, 5, 9), feet, glm::vec3(0, 1, 0));
        camera.proj = glm::perspectiveRH_NO(glm::radians(50.0f), float(width) / height, .1f, 100.0f);
    }
    renderer.SetCamera(camera);
    for (const auto &entity: std::span(mClient.snapshot().entities, mClient.snapshot().count))
    {
        auto transform = matrix(interpolated(entity, now));
        if (const auto *model = mModels.find(entity.resource))
        {
            if (!renderer.SubmitModel(*model, transform)) { fail(api, "Could not render replicated model"); return; }
        }
        else if (entity.resource == PlayerModel && entity.id != mOwnEntity)
        {
            transform = glm::translate(transform, glm::vec3(0, .9f, 0));
            transform = glm::scale(transform, glm::vec3(.6f, 1.8f, .6f));
            if (!renderer.Submit({mBox, mPlayerMaterial, transform})) { fail(api, "Could not render remote player"); return; }
        }
    }
    if (mOptions.frames && ++mFrames >= mOptions.frames) api.quit();
}
