#include "ImGuiFeatureHandler.h"
#include <events\dispatcher.h>
#include <imgui\renderer\imgui_age_rendernode.h>

using namespace MM2;

/*
    ImGuiFeatureHandler
*/

void ImGuiFeatureHandler::EndPhase()
{
    // shutdown imgui
    if (mmImGuiManager::Instance != nullptr) {
        delete mmImGuiManager::Instance;
    }
}

void ImGuiFeatureHandler::BeginPhase(bool a1)
{
    //create imgui
    auto imgui = new mmImGuiManager(!a1);
    MM2::ROOT->AddChild(imgui);
}

void ImGuiFeatureHandler::MenuUpdateHook() 
{
    reinterpret_cast<asNode*>(this)->asNode::Update();
    mmImGuiManager::Instance->Render();
}

void ImGuiFeatureHandler::Install()
{
    GameEventDispatcher::RegisterBeginPhaseCallback(BeginPhase);
    GameEventDispatcher::RegisterEndPhaseCallback(EndPhase);

    InstallCallback("MenuManager::Update", "Hook asNode update to add special imgui rendering order for main menu.",
        &MenuUpdateHook, {
            cb::call(0x4E541D),
        }
    );
}

