#include "dispatcher.h"
#include "..\mm2_network.h"

using namespace MM2;

std::vector<void(*)(bool)> GameEventDispatcher::beginPhaseCallbacks = {};
std::vector<void(*)()> GameEventDispatcher::endPhaseCallbacks = {};
std::vector<void(*)()> GameEventDispatcher::stateEndCallbacks = {};
std::vector<void(*)()> GameEventDispatcher::stateBeginCallbacks = {};
std::vector<void(*)(const char*)> GameEventDispatcher::chatMessageCallbacks = {};
std::vector<void(*)()> GameEventDispatcher::resetCallbacks = {};

/*
    Registry
*/
void GameEventDispatcher::RegisterBeginPhaseCallback(void(*cb)(bool))
{
    beginPhaseCallbacks.push_back(cb);
}

void GameEventDispatcher::RegisterEndPhaseCallback(void(*cb)())
{
    endPhaseCallbacks.push_back(cb);
}

void GameEventDispatcher::RegisterStateEndCallback(void(*cb)())
{
    stateEndCallbacks.push_back(cb);
}

void GameEventDispatcher::RegisterStateBeginCallback(void(*cb)())
{
    stateBeginCallbacks.push_back(cb);
}

void GameEventDispatcher::RegisterChatMessageCallback(void(*cb)(const char*))
{
    chatMessageCallbacks.push_back(cb);
}

void GameEventDispatcher::RegisterOnResetCallback(void(*cb)())
{
    resetCallbacks.push_back(cb);
}

/*
    Dispatcher static functions
*/
void GameEventDispatcher::BeginPhase(bool a1)
{
    for (auto cb : beginPhaseCallbacks) cb(a1);
    MM2Lua::OnStartup();
}

void GameEventDispatcher::EndPhase()
{
    for (auto cb : endPhaseCallbacks) cb();
    MM2Lua::OnShutdown();
}

void GameEventDispatcher::onStateBegin()
{
    for (auto cb : stateBeginCallbacks) cb();
    MM2Lua::OnStateBegin();
}

void GameEventDispatcher::onStateEnd()
{
    for (auto cb : stateEndCallbacks) cb();
    MM2Lua::OnStateEnd();
}

void GameEventDispatcher::onChatMessage(const char * message)
{
    for (auto cb : chatMessageCallbacks) cb(message);
    MM2Lua::OnChatMessage(message);
}

void GameEventDispatcher::onSessionCreate() 
{
    MM2Lua::OnSessionCreate();
}

void GameEventDispatcher::onSessionJoin() 
{
    MM2Lua::OnSessionJoin();
}

void GameEventDispatcher::onDisconnect()
{
    MM2Lua::OnDisconnect();
}

void GameEventDispatcher::onReset() {
    //call original
    auto game = mmGameManager::Instance->getGame();
    hook::Thunk<0x413D40>::ThisCall<void>(game);

    for (auto cb : resetCallbacks) cb();
    MM2Lua::OnReset();
}

void GameEventDispatcher::Install() {
    InstallCallback("mmGame::Reset", "Register onGameReset with dispatcher.",
        &GameEventDispatcher::onReset, {
            cb::call(0x433B3C),      //mmGameSingle::Reset
            cb::call(0x43A6B7),     //mmGameMulti::Reset
        }
    );
}