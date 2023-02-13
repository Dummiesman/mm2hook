#include "mm2.h"
#include "mm2_lua.h"
#include "mm2_network.h"
#include "luafilesystem/lfs.h"
#include <discord-presence.h>
#include <imgui\luabindings\imgui_lua_bindings.cpp>

using namespace LuaIntf;
using namespace MM2;

LuaState L;
bool isMainLuaLoaded = false;
bool isStateInitialized;

/*
    luaCallback
*/
LuaCallback::LuaCallback(LuaRef self, LuaRef function)
{
    this->self = self;
    this->function = function;
}

void LuaCallback::Call()
{
    MM2Lua::TryCallFunction<void>(function, self);
}

void LuaCallback::Release()
{
    if (self.isValid())
        self.~LuaRef();
    if (function.isValid())
        function.~LuaRef();
}

void LuaCallback::BindLua(LuaState L) {
    LuaBinding(L).beginClass<LuaCallback>("LuaCallback")
        .addFactory([](LuaRef self, LuaRef function = LuaRef()) {
        auto callback = LuaCallback(self, function);
    }, LUA_ARGS(LuaRef, _opt<LuaRef>))
        .endClass();
}

/*
    mm2_lua
*/
void mm2L_error(LPCSTR message)
{
    MM2::Errorf("[Lua] Error -- %s", message);
}

template <class retType, typename... T>
retType MM2Lua::TryCallFunction(LuaRef func, T&&... args)
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

void MM2Lua::TryCallFunction(LuaRef func)
{
    TryCallFunction<void>(func);
}

void luaAddModule_LogFile(lua_State * L)
{
    LuaBinding(L)
        .beginModule("LogFile")
            .addFunction("AppendLine", &LogFile::AppendLine)
            .addFunction("Write", &LogFile::Write)
            .addFunction("WriteLine", &LogFile::WriteLine)
        .endModule();
}

void luaAddModule_HookConfig(lua_State* L)
{
    LuaBinding(L).beginClass<HookConfig>("HookConfig")
        //functions
        .addStaticFunction("Get", [](LPCSTR key) -> bool                  { bool value = false;
                                                                            bool res = HookConfig::GetProperty(key, value);
                                                                            return value; })
        .addStaticFunction("GetInt", [](LPCSTR key, int def) -> int       { int value;
                                                                            bool res = HookConfig::GetProperty(key, value);
                                                                            return res ? value : def; }, LUA_ARGS(LPCSTR,_def<int, 0>))
        .addStaticFunction("GetFloat", [](LPCSTR key, float def) -> float { float value;
                                                                            bool res = HookConfig::GetProperty(key, value);
                                                                            return res ? value : def; }, LUA_ARGS(LPCSTR, _def<float, 0>))
        .addStaticFunction("GetString", [](LPCSTR key, LPCSTR def) -> LPCSTR          { char value[2048];
                                                                            bool res = HookConfig::GetProperty(key, value, sizeof(value));
                                                                            return res ? (LPCSTR)&value : def; })
        .addStaticFunction("HasProperty", &HookConfig::HasProperty)
        .endClass();
}
    
// these need to be here due to header file placement
// maybe this should be fixed...
void luaAddModule_Vector(lua_State *L)
{
    luaBind<Vector2>(L);
    luaBind<Vector3>(L);
    luaBind<Vector4>(L);
    luaBind<Matrix34>(L);
    luaBind<Matrix44>(L);
}

void luaSetGlobals()
{
    LogFile::Write("Updating Lua globals...");

    mmGameManager *gameMgr = mmGameManager::Instance;

    auto pGame = (gameMgr != NULL) ? gameMgr->getGame() : NULL;
    auto pPlayer = (pGame != NULL) ? pGame->GetPlayer() : NULL;
    auto pHUD = (pPlayer != NULL) ? pPlayer->GetHUD() : NULL;

    Lua::setGlobal(L, "HUD", pHUD);
    Lua::setGlobal(L, "Game", pGame);
    Lua::setGlobal(L, "Player", pPlayer);
    Lua::setGlobal(L, "ROOT", &ROOT);
    Lua::setGlobal(L, "MMSTATE", &MMSTATE);
    Lua::setGlobal(L, "MMCURRPLAYER", &MMCURRPLAYER);
    Lua::setGlobal(L, "NETMGR", &NETMGR);
    Lua::setGlobal(L, "AIMAP", aiMap::GetInstance());
    Lua::setGlobal(L, "VehicleList", VehicleListPtr.get());
    Lua::setGlobal(L, "CityList", CityListPtr.get());

    LogFile::WriteLine("Done!");
}

LUAMOD_API int luaopen_MM2(lua_State *L)
{
    LogFile::WriteLine(" - Registering MM2 library...");

    LuaRef mod = LuaRef::createTable(L);
    auto modL = mod.state();

    luaAddModule_LogFile(modL);
    luaAddModule_HookConfig(modL);
    luaAddModule_Vector(modL);
    
    // register all Lua modules
    // empty modules will be safely ignored
    // note that order matters. any beginExtendClass<>
    // which is missing a parent class will CTD/softlock before the game even starts!
    LogFile::WriteLine("      module_base");
    luaAddModule<module_base>(modL);
    LogFile::WriteLine("      module_audio");
    luaAddModule<module_audio>(modL);
    LogFile::WriteLine("      module_bound");
    luaAddModule<module_bound>(modL);
    LogFile::WriteLine("      module_inst");
    luaAddModule<module_inst>(modL);
    LogFile::WriteLine("      module_phys");
    luaAddModule<module_phys>(modL);
    LogFile::WriteLine("      module_breakable");
    luaAddModule<module_breakable>(modL);
    LogFile::WriteLine("      module_camera");
    luaAddModule<module_camera>(modL);
    LogFile::WriteLine("      module_common");
    luaAddModule<module_common>(modL);
    LogFile::WriteLine("      module_creature");
    luaAddModule<module_creature>(modL);
    LogFile::WriteLine("      module_stream");
    luaAddModule<module_stream>(modL);
    LogFile::WriteLine("      module_data");
    luaAddModule<module_data>(modL);
    LogFile::WriteLine("      module_game");
    luaAddModule<module_game>(modL);
    LogFile::WriteLine("      module_gfx");
    luaAddModule<module_gfx>(modL);
    LogFile::WriteLine("      module_input");
    luaAddModule<module_input>(modL);
    LogFile::WriteLine("      module_effects");
    luaAddModule<module_effects>(modL);
    LogFile::WriteLine("      module_level");
    luaAddModule<module_level>(modL);
    LogFile::WriteLine("      module_city");
    luaAddModule<module_city>(modL);
    LogFile::WriteLine("      module_model");
    luaAddModule<module_model>(modL);
    LogFile::WriteLine("      module_network");
    luaAddModule<module_network>(modL);
    LogFile::WriteLine("      module_rgl");
    luaAddModule<module_rgl>(modL);
    LogFile::WriteLine("      module_ui");
    luaAddModule<module_ui>(modL);
    LogFile::WriteLine("      module_vehicle");
    luaAddModule<module_vehicle>(modL);
    LogFile::WriteLine("      module_ai");
    luaAddModule<module_ai>(modL);

    LogFile::WriteLine("      discordHandler::BindLua");
    discordHandler::BindLua(modL);

    // Kind of hacky
    // This references derived classes that aren't registered
    // during the time of the initial lua registration
    LogFile::WriteLine("      lvlInstance::BindLuaLate");
    MM2::lvlInstance::BindLuaLate(modL);

    mod.pushToStack();

    LogFile::WriteLine("Done!");

    return 1;
}

void LoadMainScript() {
    LPCSTR lua_file = ".\\lua\\main.lua";

    if (file_exists(lua_file))
    {
        int status = L.loadFile(lua_file) || L.pcall(0, 0, 0);

        if (isMainLuaLoaded = (status == LUA_OK))
        {
            LogFile::Format(" - Loaded script file: %s\n", lua_file);

            //call init
            LuaRef func(L, "init");
            MM2Lua::TryCallFunction(func);
        }
    }
    else
    {
        LogFile::Format(" - COULD NOT FIND FILE '%s'!!!\n", lua_file);
    }
}

void GC()
{
    L.gc();
}

void ReloadScript()
{
    // try reloading Lua
    LogFile::WriteLine("Reloading main script...");
    LoadMainScript();
    
    // garbage collect
    GC();

    mmGameManager *mgr = mmGameManager::Instance;
    auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;

    if (gamePtr != NULL && gamePtr->GetPlayer() != NULL)
    {
        auto hud = gamePtr->GetPlayer()->GetHUD();
        if (hud != NULL)
            hud->SetMessage("Lua script reloaded.", 3.5, 0);
    }
    else
    {
        LogFile::WriteLine("Lua script reloaded.\n");
    }
}

LuaState * MM2Lua::GetState() {
    return &L;
}

bool MM2Lua::IsEnabled()
{
    return cfgEnableLua;
}

bool MM2Lua::IsInitialized()
{
    return isStateInitialized;
}

void MM2Lua::Initialize() {
    if (IsInitialized())
    {
        mm2L_error("Tried to initialize the Lua engine twice!");
        return;
    }

    if (IsEnabled()) {
        LogFile::WriteLine("Initializing Lua...");

        L = LuaState::newState();
        isStateInitialized = true;

        L.openLibs();
        L.require("MM2", luaopen_MM2);
        L.require("lfs", luaopen_lfs);
        L.pop();

        ImguiBindLua(L);
        luaSetGlobals();

        //set LFS path
        LogFile::WriteLine("Setting lfs path...");
        char execPath[MAX_PATH];
        GetModuleFileNameA(NULL, execPath, sizeof(execPath));
        PathRemoveFileSpecA(execPath);

        LuaRef(L, "lfs").get("chdir").call(execPath);

        //
        LogFile::WriteLine("Loading main script...");
        ReloadScript();
    }
}

void MM2Lua::Reset()
{
    if(IsInitialized())
        luaSetGlobals();
}

void MM2Lua::OnChatMessage(char* message) {
    if (IsInitialized()) {
        LuaRef func(L, "onChatMessage");
        TryCallFunction<void>(func, message);
    }
}

void MM2Lua::OnGameEnd() {
    if (IsInitialized()) {
        LuaRef func(L, "onGameEnd");
        TryCallFunction(func);
    }
}

void MM2Lua::OnGamePreInit() {
    if (IsInitialized()) {
        luaSetGlobals();
        LuaRef func(L, "onGamePreInit");
        TryCallFunction(func);
    }
}

void MM2Lua::OnGamePostInit() {
    if (IsInitialized()) {
        luaSetGlobals();
        LuaRef func(L, "onGamePostInit");
        TryCallFunction(func);
    }
}

void MM2Lua::OnSessionCreate(char * sessionName, char * sessionPassword, int sessionMaxPlayers, NETSESSION_DESC * sessionData)
{
    if (IsInitialized()) {
        LuaRef func(L, "onSessionCreate");
        TryCallFunction<void>(func, sessionName, sessionPassword, sessionMaxPlayers, sessionData);
    }
}

void MM2Lua::OnSessionJoin(char * a2, GUID * a3, char * a4)
{
    if (IsInitialized()) {
        LuaRef func(L, "onSessionJoin");
        TryCallFunction<void>(func, a2, a3, a4);
    }
}

void MM2Lua::OnDisconnect() 
{
    if (IsInitialized()) {
        LuaRef func(L, "onDisconnect");
        TryCallFunction(func);
    }
}

void MM2Lua::OnReset() 
{
    if (IsInitialized()) {
        LuaRef func(L, "onReset");
        TryCallFunction(func);
    }
}

void MM2Lua::OnTick()
{
    if (IsInitialized()) {
        LuaRef tickFunction(L, "tick");
        TryCallFunction(tickFunction);
    }

    // reset lastKey
    if (IsInitialized())
        Lua::setGlobal(L, "lastKey", -1);
}

void MM2Lua::OnShutdown()
{
    if(IsInitialized()) {
        {
            LuaRef func(L, "shutdown");
            TryCallFunction(func);
        }
        L.close();
        isStateInitialized = false;
        isMainLuaLoaded = false;
    }
}

void MM2Lua::OnInitializeUi()
{
    if (IsInitialized()) {
        LuaRef func(L, "onInitUi");
        TryCallFunction(func);
    }
}

void MM2Lua::OnRenderUi()
{
    if (IsInitialized()) {
        LuaRef func(L, "onRenderUi");
        TryCallFunction(func);
    }
}

void MM2Lua::OnKeyPress(DWORD vKey)
{
    if (IsInitialized()) {
        Lua::setGlobal(L, "lastKey", vKey);

        if (vKey == VK_F5) {
            ReloadScript();
        }
    }
}

void MM2Lua::SendCommand(LPCSTR command)
{
    if (IsInitialized()) {
        LogFile::Format("> [Lua]: %s\n", command);
        try
        {
            Lua::exec(L, command);
        }
        catch (LuaException le)
        {
            mm2L_error(le.what());
        }
        catch (const std::exception& ex)
        {
            std::string errMsg = "Non LuaException error:" + std::string(ex.what());
            mm2L_error(errMsg.c_str());
        }
    }
}