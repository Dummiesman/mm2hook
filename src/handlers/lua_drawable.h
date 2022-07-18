#pragma once
#include <mm2_common.h>
#include <mm2_lua.h>
#include <mm2_lua.h>
#include <modules/node.h>
#include <map>

class luaDrawableHandler {
private:
    static std::map<int, int> idToIndexMap;
    static std::map<int, int> idToListMap;
    static std::vector<LuaCallback> callbackLists[2];
    static uint idProvider;
public:
    static int RegisterCallback(LuaRef self, LuaRef function, int phase);
    static void UnregisterCallback(int id);
    static void ResetLuaCallbacks();

    void CallCallbacks(int phase);
    void ResetCallbacks();

    static void Install();
};