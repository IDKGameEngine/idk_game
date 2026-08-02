#include "libidk/platform/Platform.hpp"
#include "idk_engine/Engine.hpp"

#include "libidk/message/RemoteRxTx.hpp"
#include "libidk/message/SharedRxTx.hpp"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <SDL3/SDL.h>

// static void SendCtrlEventFunc(idk::platform::Platform &plat, void *event, void *arg)
// {
//     (void)plat;
//     (void)arg;
//     auto &e = *((SDL_Event*)event);
//     bool dirty = false;

//     if (e.type == SDL_EVENT_KEY_UP)
//     {
//         if (e.key.scancode == SDL_SCANCODE_X)
//         {
//             VLOG_INFO("SEND X"); ctrl.x = (ctrl.x+1) % 2;
//             dirty = true;
//         }

//         if (e.key.scancode == SDL_SCANCODE_Y)
//         {
//             VLOG_INFO("SEND Y"); ctrl.y = (ctrl.y+1) % 2;
//             dirty = true;
//         }

//         if (e.key.scancode == SDL_SCANCODE_Z)
//         {
//             VLOG_INFO("SEND Z"); ctrl.z = (ctrl.z+1) % 2;
//             dirty = true;
//         }
//     }

//     if (dirty)
//     {
//         txer.sendMsg(ctrl);
//     }
// }

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

    idk::EngineControlData ctrl, ctrlBuf;
    idk::EngineStatusData  stat;
    idk::PeriodicTimer ctrlTimer(4);
    idk::PeriodicTimer statTimer(4);
    idk::RemoteRxer statRx(5002);
    idk::RemoteTxer ctrlTx(argv[1], atol(argv[2]));
    // idk::SharedTxer txer("IDKGameEngineIPC-EngineControl", sizeof(idk::EngineControlData));

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
    // ImGuiStyle& style = ImGui::GetStyle();
    // style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    // style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(win.getWinCtx(), win.getGpuCtx());
    ImGui_ImplOpenGL3_Init(nullptr);

    bool show_demo_window = true;
    // bool show_another_window = false;
    // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (plat.running())
    {
        plat.update();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window) { ImGui::ShowDemoWindow(&show_demo_window); }

        {
            ImGui::Begin("Woop");
            ImGui::InputInt("X", &ctrlBuf.x);
            ImGui::InputInt("Y", &ctrlBuf.y);
            ImGui::InputInt("Z", &ctrlBuf.z);
            if (ImGui::Button("Send"))
            {
                ctrl = ctrlBuf;
            }
            ImGui::End();
        }

        {
            ImGui::Begin("EngineStatusData");
            ImGui::LabelText("EngineStatus", "x=%d y=%d z=%d", stat.x, stat.y, stat.z);
            ImGui::End();
        }

        if (ctrlTimer.expired())
        {
            ctrlTimer.reset();
            ctrlTx.sendMsg(ctrl);
        }

        if (statTimer.expired())
        {
            statTimer.reset();
            while (statRx.recvMsg(stat))
            {
                // 
            }
        }


        ImGui::Render();
        // glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(win.getWinCtx());
    }

    return 0;
}
