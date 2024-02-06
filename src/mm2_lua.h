#pragma once
#include "mm2_common.h"
#include "mm2_network.h"
#include <map>
#include <string>

static ConfigValue<bool> cfgEnableLua   ("EnableLua", true);

//helper to get callbacks from lua
class LuaCallback
{
private:
    LuaIntf::LuaRef self;
    LuaIntf::LuaRef function;
public:
    LuaCallback(LuaIntf::LuaRef self, LuaIntf::LuaRef function);
    void Call();
    void Release();
    static void BindLua(LuaIntf::LuaState L);
};

class PersistentDataStore
{
private:
    static std::map<std::string, std::string> m_Data;
public:
    static std::string Retrieve(std::string key);
    static bool Contains(std::string key);
    static void Store(std::string key, std::string value);
    static void Delete(std::string key);
};

class MM2Lua
{
private:
    //cleanup list
    static std::vector<MM2::Base*> dirtyLaundry;
private:
    static void mm2L_error(LPCSTR message);
public:
    //helper functions
    template <class retType = void, typename... T>
    static retType TryCallFunction(LuaIntf::LuaRef func, T&&... args)
    {
        if (func.isValid() && func.isFunction())
        {
            try
            {
                return func.call<retType>(std::forward<T>(args)...);
            }
            catch (LuaException le)
            {
                mm2L_error(le.what());
            }
        }
    }

    static void TryCallFunction(LuaIntf::LuaRef func);
public:
    static LuaState & GetState();

    //
    static bool IsInitialized();
    static bool IsEnabled();

    static void Initialize();
    static void Reset();

    //cleanup
    static void MarkForCleanupOnShutdown(MM2::Base* object);

    //events
    static void OnRenderHudmap();
    static void OnChatMessage(const char* message);
    static void OnReset();
    static void OnDisconnect();
    static void OnSessionCreate();
    static void OnSessionJoin();
    static void OnDebugMessage(int level, const char* text);

    static void OnStateBegin();
    static void OnStateEnd();

    static void OnTick();
    static void OnStartup();
    static void OnShutdown();

    static void OnInitializeUi();
    static void OnRenderUi();

    static void OnKeyPress(DWORD vKey);

    static void SendCommand(LPCSTR command);
};