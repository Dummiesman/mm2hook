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
    void OnChatMessage(const char* message);
    void OnReset();
    void OnDisconnect();
    void OnSessionCreate();
    void OnSessionJoin();
    
    void OnStateBegin();
    void OnStateEnd();

    void OnTick();
    void OnStartup();
    void OnShutdown();

    void OnInitializeUi();
    void OnRenderUi();

    void OnKeyPress(DWORD vKey);

    void SendCommand(LPCSTR command);
}