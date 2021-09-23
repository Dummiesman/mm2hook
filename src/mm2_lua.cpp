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

void mm2L_error(LPCSTR message)
{
    MM2::Errorf("[Lua] Error -- %s", message);
}

template <class retType, typename... T>
retType tryCallFunction(LuaRef func, T&&... args)
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

void tryCallFunction(LuaRef func)
{
    tryCallFunction<void>(func);
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
    auto pPlayer = (pGame != NULL) ? pGame->getPlayer() : NULL;
    auto pHUD = (pPlayer != NULL) ? pPlayer->getHUD() : NULL;

    Lua::setGlobal(L, "HUD", pHUD);
    Lua::setGlobal(L, "Game", pGame);
    Lua::setGlobal(L, "Player", pPlayer);
    Lua::setGlobal(L, "ROOT", &ROOT);
    Lua::setGlobal(L, "MMSTATE", &MMSTATE);

    LogFile::WriteLine("Done!");
}

LUAMOD_API int luaopen_MM2(lua_State *L)
{
    LogFile::WriteLine(" - Registering MM2 library...");

    LuaRef mod = LuaRef::createTable(L);
    auto modL = mod.state();

    luaAddModule_LogFile(modL);
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
    LogFile::WriteLine("      module_ai");
    luaAddModule<module_ai>(modL);
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
    LogFile::WriteLine("      module_particle");
    luaAddModule<module_particle>(modL);
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
    //char lua_file[MAX_PATH];
    //MM2::datAssetManager::FullPath(lua_file, sizeof(lua_file), "lua", "main", "lua");

    LPCSTR lua_file = ".\\lua\\main.lua";

    if (file_exists(lua_file))
    {
        int status = L.loadFile(lua_file) || L.pcall(0, 0, 0);

        if (isMainLuaLoaded = (status == LUA_OK))
        {
            LogFile::Format(" - Loaded script file: %s\n", lua_file);

            // this is kinda important, no?
            luaSetGlobals();

            //call init
            LuaRef func(L, "init");
            tryCallFunction(func);
        }

        //mm2L_error(L.toString(-1));
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

    mmGameManager *mgr = mmGameManager::Instance;
    auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;

    if (gamePtr != NULL && gamePtr->getPlayer() != NULL)
    {
        auto hud = gamePtr->getPlayer()->getHUD();
        if (hud != NULL)
            hud->SetMessage("Lua script reloaded.", 3.5, 0);
    }
    else
    {
        LogFile::WriteLine("Lua script reloaded.\n");
    }

    // garbage collect
    GC();
}

LuaState * MM2Lua::GetState() {
    return &L;
}

bool MM2Lua::IsEnabled()
{
    return cfgEnableLua;
}

bool MM2Lua::IsLoaded()
{
    return cfgEnableLua && isMainLuaLoaded;
}

void MM2Lua::Initialize() {
    if (cfgEnableLua) {
        if (isMainLuaLoaded)
            mm2L_error("Tried to initialize the Lua engine twice!");

        LogFile::WriteLine("Initializing Lua...");

        L = LuaState::newState();

        L.openLibs();
        L.require("MM2", luaopen_MM2);
        L.require("lfs", luaopen_lfs);
        L.pop();

        ImguiBindLua(L);

        //set LFS path
        LogFile::WriteLine("Setting lfs path...");
        char execPath[MAX_PATH];
        GetModuleFileNameA(NULL, execPath, sizeof(execPath));
        PathRemoveFileSpecA(execPath);

        LuaRef(L, "lfs").get("chdir").call(execPath);

        //override print function
        SendCommand("print = function(x) Displayf(tostring(x)) end");

        //
        LogFile::WriteLine("Loading main script...");
        ReloadScript();
    }
}

void MM2Lua::Reset()
{
    luaSetGlobals();
}

void MM2Lua::OnChatMessage(char* message) {
    if (IsLoaded()) {
        LuaRef func(L, "onChatMessage");
        tryCallFunction<void>(func, message);
    }
}

void MM2Lua::OnGameEnd() {
    if (IsLoaded()) {
        LuaRef func(L, "onGameEnd");
        tryCallFunction(func);
    }
}

void MM2Lua::OnGamePreInit() {
    if (IsLoaded()) {
        LuaRef func(L, "onGamePreInit");
        tryCallFunction(func);
    }
}

void MM2Lua::OnGamePostInit() {
    if (IsLoaded()) {
        luaSetGlobals(); //set globals so post init has access to things like Game
        LuaRef func(L, "onGamePostInit");
        tryCallFunction(func);
    }
}

void MM2Lua::OnSessionCreate(char * sessionName, char * sessionPassword, int sessionMaxPlayers, NETSESSION_DESC * sessionData)
{
    if (IsLoaded()) {
        LuaRef func(L, "onSessionCreate");
        tryCallFunction<void>(func, sessionName, sessionPassword, sessionMaxPlayers, sessionData);
    }
}

void MM2Lua::OnSessionJoin(char * a2, GUID * a3, char * a4)
{
    if (IsLoaded()) {
        LuaRef func(L, "onSessionJoin");
        tryCallFunction<void>(func, a2, a3, a4);
    }
}

void MM2Lua::OnDisconnect() {
    if (IsLoaded()) {
        LuaRef func(L, "onDisconnect");
        tryCallFunction(func);
    }
}

void MM2Lua::OnReset() {
    if (IsLoaded()) {
        LuaRef func(L, "onReset");
        tryCallFunction(func);
    }
}

void MM2Lua::OnTick()
{
    if (IsLoaded()) {
        LuaRef tickFunction(L, "tick");
        tryCallFunction(tickFunction);
    }

    // reset lastKey
    Lua::setGlobal(L, "lastKey", -1);
}

void MM2Lua::OnShutdown()
{
    if(IsLoaded()) {
        LuaRef func(L, "shutdown");
        tryCallFunction(func);
    }
    GC();
    L.close();
    isMainLuaLoaded = false;
}

void MM2Lua::OnRenderUi()
{
    if (IsLoaded()) {
        LuaRef func(L, "onRenderUi");
        tryCallFunction(func);
    }
}

void MM2Lua::OnKeyPress(DWORD vKey)
{
    Lua::setGlobal(L, "lastKey", vKey);

    switch (vKey)
    {
        case VK_F5:
        {
            ReloadScript();
        } break;
    }
}

void MM2Lua::SendCommand(LPCSTR command)
{
    LogFile::Format("> [Lua]: %s\n", command);
    try 
    {
        Lua::exec(L, command);
    }
    catch (LuaException le) 
    {
        mm2L_error(le.what());
    }
}