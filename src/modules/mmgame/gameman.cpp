#pragma once
#include "gameman.h"

using namespace MM2;

mmGame* mmGameManager::sm_CustomGame = nullptr;

bool mmGameManager::isCurrentGameSingleGame() const {
    auto game = getGame();
    return (game != nullptr && (game == m_SingleRoamGame || game == m_SingleRaceGame || game == m_SingleStuntGame
                                || game == m_SingleCircuitGame || game == m_SingleBlitzGame));
}

bool mmGameManager::isCurrentGameMultiGame() const {
    auto game = getGame();
    return (game != nullptr && (game == m_MultiRaceGame || game == m_MultiRoamGame || game == m_MultiCRGame
                                || game == m_MultiCircuitGame || game == m_MultiBlitzGame));
}

mmGame* mmGameManager::getGame() const {
    return this->m_CurrentGame;
};

mmPlayer* mmGameManager::getPlayerSafe() const {
    auto game = this->getGame();
    if (game == nullptr)
        return nullptr;
    auto player = game->GetPlayer();
    return player;
}


AGE_API mmGameManager::mmGameManager(void)  {
    scoped_vtable x(this);
    hook::Thunk<0x4029E0>::Call<void>(this);
}

AGE_API mmGameManager::~mmGameManager(void) {
    scoped_vtable x(this);
    hook::Thunk<0x402E40>::Call<void>(this);
}

/*
    asNode virtuals
*/
AGE_API void mmGameManager::Cull()                       { hook::Thunk<0x4031D0>::Call<void>(this); }
AGE_API void mmGameManager::Update()                     { hook::Thunk<0x403000>::Call<void>(this); }
AGE_API void mmGameManager::Reset()                      { hook::Thunk<0x402E30>::Call<void>(this); }

/* TODO?
mmGameManager::mmGameManager(void)
mmGameManager::~mmGameManager(void)

void mmGameManager::BeDone(void)
void mmGameManager::ForcePopupUI(void)
void mmGameManager::ForceReplayUI(void)
*/

void mmGameManager::BindLua(LuaState L) {
    LuaBinding(L).beginExtendClass<mmGameManager, asNode>("mmGameManager")
        //properties
        .addPropertyReadOnly("Game", &getGame)
                
        //statics
        .addStaticFunction("Instance", [] {return (mmGameManager *)Instance; })
    .endClass();
}