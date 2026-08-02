#include "libidk/platform/Platform.hpp"
#include "idk_engine/Engine.hpp"

#include "libidk/message/RemoteRxTx.hpp"
#include "libidk/gpu/gl_bindings.hpp"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <SDL3/SDL.h>

static void ImGuiSDL3EventFunc(idk::platform::Platform &plat, void *event, void *arg)
{
    (void)plat;
    (void)arg;
    ImGui_ImplSDL3_ProcessEvent((SDL_Event*)event);
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (argc != 3)
    {
        VLOG_FATAL("Usage: gamectl hostname port");
    }

    idk::platform::Platform plat;
    idk::platform::Window &win = plat.getWindow();

    idk::EngineCtrlData ctrl, ctrlBuf;
    idk::EngineStatData stat;
    idk::PeriodicTimer ctrlTimer(4);
    idk::PeriodicTimer statTimer(4);
    idk::RemoteRxTxer port(argv[1], atol(argv[2]));

    plat.addEventCallback(ImGuiSDL3EventFunc, nullptr);


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(win.getWinCtx(), win.getGpuCtx());
    ImGui_ImplOpenGL3_Init(nullptr);

    bool show_demo_window = true;
    // bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (plat.running())
    {
        plat.update();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        {
            ImGui::Begin("EngineCtrlData");
            ImGui::InputInt("X", &ctrlBuf.x);
            ImGui::InputInt("Y", &ctrlBuf.y);
            ImGui::InputInt("Z", &ctrlBuf.z);
            if (ImGui::Button("Send"))
            {
                ctrl = ctrlBuf;
            }
            ImGui::SameLine();
            if (ImGui::Button("Kill"))
            {
                ctrl.kill = true;
            }
            ImGui::End();
        }

        {
            ImGui::Begin("EngineStatData");
            ImGui::Text("StaticAllocatorMemoryUsage: %f", stat.allocatorMemoryUsage);
            ImGui::Text("x: %d", stat.x);
            ImGui::Text("y: %d", stat.y);
            ImGui::Text("z: %d", stat.z);

            ImGui::End();
        }

        if (ctrlTimer.expired())
        {
            ctrlTimer.reset();
            port.sendMsg(&ctrl, sizeof(ctrl), "CTRL");
        }

        if (statTimer.expired())
        {
            statTimer.reset();
            while (idk::MessageRecvInfo *msg = port.recvMsg())
            {
                auto &h = msg->header;
                if (!msg->isType("STAT"))
                {
                    VLOG_WARN("[Engine::handleCtrlMessage] Recieved \"{}\"", msg->header.payloadType.ascii);
                    continue;
                }
                if (h.payloadSize != sizeof(idk::EngineStatData))
                {
                    VLOG_WARN("h.payloadSize != sizeof(idk::EngineStatData)");
                    continue;
                }
                idk_memcpy(&stat, msg->payload, h.payloadSize);
            }
        }

        ImGui::Render();
        idk::gl::Viewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        idk::gl::ClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        idk::gl::Clear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(win.getWinCtx());
    }

    return 0;
}
