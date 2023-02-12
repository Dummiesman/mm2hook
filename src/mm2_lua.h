#pragma once
#include "mm2_common.h"
#include "mm2_network.h"

static ConfigValue<bool> cfgEnableLua   ("EnableLua", true);

//helper to get callbacks from lua
class LuaCallback
{
private:
    LuaRef self;
    LuaRef function;
public:
    LuaCallback(LuaRef self, LuaRef function);
    void Call();
    void Release();
    static void BindLua(LuaState L);
};

namespace MM2Lua
{
    //
    LuaState * GetState();

    //
    bool IsInitialized();
    bool IsEnabled();

    void Initialize();
    void Reset();

    //helper functions
    template <class retType, typename... T>
    retType TryCallFunction(LuaRef func, T&&... args);
    void TryCallFunction(LuaRef func);

    //events
    void OnChatMessage(char* message);
    void OnGameEnd();
    void OnGamePreInit();
    void OnGamePostInit();
    void OnReset();
    void OnDisconnect();
    void OnSessionCreate(char *sessionName, char *sessionPassword, int sessionMaxPlayers, MM2::NETSESSION_DESC *sessionData);
    void OnSessionJoin(char *a2, GUID *a3, char *a4);
    
    void OnTick();
    void OnShutdown();
    void OnKeyPress(DWORD vKey);

    void OnInitializeUi();
    void OnRenderUi();

    void SendCommand(LPCSTR command);
}