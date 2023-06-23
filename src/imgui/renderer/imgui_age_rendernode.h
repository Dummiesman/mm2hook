#pragma once
#include <mm2_common.h>
#include <modules/node.h>
#include <modules/input.h>
#include <mm2_gfx.h>
#include <mm2_lua.h>

#include <implot/implot.h>
#include <imgui/imgui.h>
//#include <imgui/impl/imgui_impl_win32.h>
#include <imgui/impl/imgui_impl_age.h>

using namespace MM2;
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class mmImGuiManager : public asNode {
public:
    static mmImGuiManager* Instance;
private:
    gfxViewport* viewport;
    bool gameMode;
private:
    //mmInput::PutEventInQueue called from mmInput::ProcessMouseEvents
    void PutMouseEventInQueue(long long a1)
    {
        auto io = ImGui::GetIO();

        if (!io.WantCaptureMouse)
            hook::Thunk<0x52D5E0>::Call<void>(this, a1);
    }

    //mmInput::PutEventInQueue called from mmInput::ProcessKeyboardEvents
    void PutKeyboardEventInQueue(long long a1)
    {
        auto io = ImGui::GetIO();

        if (!io.WantCaptureKeyboard)
            hook::Thunk<0x52D5E0>::Call<void>(this, a1);
    }

    //ioEventQueue::Queue hook
    static void EventQueueQueue(int a1, int a2, int a3, int a4)
    {
        auto io = ImGui::GetIO();

        if (io.WantCaptureKeyboard && a1 >= 7)
            return;

        if (io.WantCaptureMouse && a1 <= 6) 
        {
            ioMouse::ClearStates();
            return;
        }

        hook::StaticThunk<0x4BA9D0>::Call<void>(a1, a2, a3, a4);
    }

    //ioInput::Update hook (Currently doesn't respect useJoystick but that always seems to be 0 anyways?)
    static void IOInputHook()
    {
        auto io = ImGui::GetIO();

        //update mouse
        //if (!io.WantCaptureMouse)
            ioMouse::Update();
        //else
            //ioMouse::ClearStates();

        //update keyboard
        //if (!io.WantCaptureKeyboard)
            ioKeyboard::Update();
        //else
            //ioKeyboard::ClearStates();

        //update pad
        hook::StaticThunk<0x4BB7A0>::Call<void>(); //ioPad::UpdateAll
    }
public:
    ANGEL_ALLOCATOR 

    mmImGuiManager::mmImGuiManager(bool gameMode) 
    {
        // delete instance if we already have one
        if (mmImGuiManager::Instance != nullptr) {
            Quitf("mmImGuiManager already created!");
        }

        Displayf("Initializing ImGui");

        // init the AGE part
        viewport = gfxPipeline::CreateViewport();

        // init the imgui part
        ImGui::CreateContext();
        ImPlot::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        // Initialize helper Platform and Renderer bindings
        //ImGui_ImplWin32_Init(hWndMain.get());

        if (gameMode)
            ImGui_ImplAGE_Init(viewport);
        else
            ImGui_ImplAGE_Init(viewport, 640, 480);

        // Steal input from game engine
        //mmInput:: ProcessXEvents hooks hook things like map key, toggle HUD key, etc.
        //ioInput:: hooks mainly the driving axes.  Accelerate, brake, etc.
        //ioEventQueue:: hooks the rest. Ex. mouse clicking used in menus
       //InstallCallback("mmInput::ProcessMouseEvents", "Rewire input through the imGuiManager",
       //    &mmImGuiManager::PutMouseEventInQueue, {
       //        cb::call(0x52CC4A),
       //    }
       //);
       //
       //InstallCallback("mmInput::ProcessKeyboardEvents", "Rewire input through the imGuiManager",
       //    &mmImGuiManager::PutKeyboardEventInQueue, {
       //        cb::call(0x52CC9F),
       //    }
       //);
       //
       //InstallCallback("ioInput::Update", "Rewire input through the imGuiManager",
       //    &mmImGuiManager::IOInputHook, {
       //        cb::call(0x4A916A),
       //    }
       //);
       //
       //InstallCallback("ioEventQueue::Queue", "Rewire input through the imGuiManager",
       //    &mmImGuiManager::EventQueueQueue, {
       //        cb::call(0x4BAD72),
       //        cb::call(0x4BAD93),
       //        cb::call(0x4BADDB),
       //        cb::call(0x4BAE3D),
       //        cb::call(0x4BAE89),
       //        cb::call(0x4BAEDB),
       //        cb::call(0x4BAF4E),
       //        cb::call(0x4BAFA7),
       //        cb::call(0x4BB044),
       //    }
       //);

        this->gameMode = gameMode;
        mmImGuiManager::Instance = this;
        asNode::asNode();
    }

    mmImGuiManager::~mmImGuiManager()
    {
        Displayf("Shutting down IMGUI");
        ImGui_ImplAGE_Shutdown();
        //ImGui_ImplWin32_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
        mmImGuiManager::Instance = nullptr;

        asNode::~asNode();
    }

	virtual void Cull() override {
        // Feed inputs to dear imgui, start new frame
        ImGui_ImplAGE_NewFrame();
        //ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Lua
        MM2Lua::OnRenderUi();

        ImGui::Render();
        ImGui_ImplAGE_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
	}
    
	virtual void Update() override {
        ImGui_ImplAGE_UpdateInput();
        if (gameMode)
            Render();
	}

    void Render() 
    {
        asCullManager::Instance.get()->DeclareCullable2DFG(this);
    }
};