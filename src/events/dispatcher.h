#pragma once
#include "mm2.h"
#include <vector>

class GameEventDispatcher : handler_t {
private:
    static std::vector<void(*)(bool)> beginPhaseCallbacks;
    static std::vector<void(*)()> endPhaseCallbacks;
    static std::vector<void(*)()> stateEndCallbacks;
    static std::vector<void(*)()> stateBeginCallbacks;
    static std::vector<void(*)(const char*)> chatMessageCallbacks;
    static std::vector<void(*)()> resetCallbacks;
public:
    static void RegisterBeginPhaseCallback(void(*cb)(bool));
    static void RegisterEndPhaseCallback(void(*cb)());
    static void RegisterStateEndCallback(void(*cb)());
    static void RegisterStateBeginCallback(void(*cb)());
    static void RegisterChatMessageCallback(void(*cb)(const char*));
    static void RegisterOnResetCallback(void(*cb)());
    
    static void BeginPhase(bool a1);
    static void EndPhase();

    static void onStateBegin();
    static void onStateEnd();
    static void onChatMessage(const char* message);
    static void onSessionCreate();
    static void onSessionJoin();
    static void onDisconnect();
    static void onReset();

    static void Install();
};