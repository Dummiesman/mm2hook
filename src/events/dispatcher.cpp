#include "dispatcher.h"
#include "..\..\discord-rpc\discord-presence.h"
#include "..\mm2_network.h"
#include "..\handlers\feature_handlers.h"

using namespace MM2;

/*
    thiscall hooked functions
*/
void GameEventDispatcher::onGameInitHook()
{
    GameEventDispatcher::onGamePreInit();
    get<mmGame>()->mmGame::Init();
    GameEventDispatcher::onGamePostInit();
}

void GameEventDispatcher::onGameEndHook(int a1)
{
    GameEventDispatcher::onGameEnd(a1);
}

/*
    Dispatcher static functions
*/

void GameEventDispatcher::onGameEnd(int a1)
{
    if (MM2Lua::IsEnabled())
        MM2Lua::OnGameEnd();
    TextureVariantHandler::Reset();
    cityTimeWeatherLightingHandler::Reset();
}

void GameEventDispatcher::onGamePreInit()
{
    if (MM2Lua::IsEnabled())
        MM2Lua::OnGamePreInit();
}

void GameEventDispatcher::onGamePostInit()
{
    if (MM2Lua::IsEnabled()) 
        MM2Lua::OnGamePostInit();
    dgBangerInstanceHandler::Reset();
}

void GameEventDispatcher::onChatMessage(char * message)
{
    if (MM2Lua::IsEnabled())
        MM2Lua::OnChatMessage(message);
}

void GameEventDispatcher::onTick()
{
    if(MM2Lua::IsEnabled())
        MM2Lua::OnTick();
}

void GameEventDispatcher::onSessionCreate(char *sessionName, char *sessionPassword, int sessionMaxPlayers, NETSESSION_DESC *sessionData) 
{
    if (MM2Lua::IsEnabled())
        MM2Lua::OnSessionCreate(sessionName, sessionPassword, sessionMaxPlayers, sessionData);
}

void GameEventDispatcher::onSessionJoin(char *a2, GUID *a3, char *a4) 
{
    if (MM2Lua::IsEnabled())
     MM2Lua::OnSessionJoin(a2, a3, a4);
}

void GameEventDispatcher::onDisconnect()
{
    if (MM2Lua::IsEnabled())
        MM2Lua::OnDisconnect();
}

void GameEventDispatcher::onReset() {
    if (MM2Lua::IsEnabled())
        MM2Lua::OnReset();

    //do fade (maybe move this eventually)
    gfxPipeline::SetFade(0xFF000000);
    gfxPipeline::StartFade(0x00000000, 1.f);

    //call original
    auto game = mmGameManager::Instance->getGame();
    hook::Thunk<0x413D40>::ThisCall<void>(game);
}

void GameEventDispatcher::Install() {
    InstallCallback("mmGame::Init", "Register onGameInitHook with dispatcher.",
        &onGameInitHook, {
            cb::jmp(0x433AA0),      //mmGameSingle::Init
            cb::call(0x438F81),     //mmGameMulti::Init
        }
    );
    InstallCallback("mmGame::BeDone", "Register onGameEndHook with dispatcher.",
        &onGameEndHook, {
            cb::jmp(0x414DF1),      //end of mmGame::BeDone
        }
    );

    InstallCallback("mmGame::Reset", "Register onGameReset with dispatcher.",
        &GameEventDispatcher::onReset, {
            cb::call(0x433B3C),      //mmGameSingle::Reset
            cb::call(0x43A6B7),     //mmGameMulti::Reset
        }
    );
}