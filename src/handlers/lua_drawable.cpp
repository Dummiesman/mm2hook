#include "lua_drawable.h"

using namespace MM2;

/*
    luaDrawableHandler
*/
const int LIST_COUNT = 2;

std::map<int, int> luaDrawableHandler::idToIndexMap = std::map<int, int>();
std::map<int, int> luaDrawableHandler::idToListMap = std::map<int, int>();

uint luaDrawableHandler::idProvider = 0;
std::vector<luaCallback> luaDrawableHandler::callbackLists[2];

void luaDrawableHandler::UnregisterCallback(int id)
{
    if (idToIndexMap.count(id) == 0) {
        Warningf("Tried to unregister Lua callback with nonexistent id %i", id);
        return;
    }

    //remove  it
    int index = idToIndexMap.at(id);
    int listnum = idToListMap.at(id);
    
    auto &list = callbackLists[listnum];
    auto &callback = list.at(index);
    callback.Release();

    list.erase(list.begin() + index);
    idToIndexMap.erase(id);
    idToListMap.erase(id);

    //re-index the rest
    for (auto it = idToIndexMap.begin(); it != idToIndexMap.end(); ++it)
    {
        if (it->second >= index)
            it->second -= 1;
    }
}

int luaDrawableHandler::RegisterCallback(LuaRef self, LuaRef function, int phase)
{
    //verify
    if (!function.isValid() || !function.isFunction()) {
        Errorf("RegisterLuaDrawable: function input wasn't valid");
        return -1;
    }
    if (phase >= LIST_COUNT)
    {
        Errorf("RegisterLuaDrawable: invalid callback phase %i", phase);
        return -1;
    }

    auto &list = callbackLists[phase];
    int id = idProvider++;

    idToIndexMap.insert({ id, list.size() });
    idToListMap.insert({ id, phase });
    list.push_back(luaCallback(self, function));

    return id;
}

void luaDrawableHandler::CallCallbacks(int phase)
{
    //call original
    hook::Thunk<0x465630>::Call<void>(this, phase);

    //call lua callbacks
    auto& list = callbackLists[phase];
    for (auto &callback : list)
    {
        callback.Call();
    }
}

void luaDrawableHandler::ResetLuaCallbacks()
{
    //reset our callbacks
    for (int i = 0; i < LIST_COUNT; i++) {
        auto& list = callbackLists[i];
        for (auto &callback : list)
        {
            callback.Release();
        }
        list.clear();
    }
}

void luaDrawableHandler::ResetCallbacks()
{
    if (ROOT->IsPaused())
        return;

    //call original resetcallbacks
    hook::Thunk<0x465680>::Call<void>(this);
}

void luaDrawableHandler::Install()
{
    InstallCallback("lvlLevel::CallCallbacks", "Hook for lua callbacks",
        &CallCallbacks, {
            cb::call(0x446217),
            cb::call(0x445D6F),
        }
    );

    InstallCallback("lvlLevel::ResetCallbacks", "Allows for control over when to clear callbacks.",
        &ResetCallbacks, {
            cb::jmp(0x465460),
        }
    );
}