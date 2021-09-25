#pragma once
#include <mm2_common.h>
#include <modules/node.h>
#include <map>

//helper to get callbacks from lua
class luaCallback
{
private:
    LuaRef self;
    LuaRef function;
public:
    luaCallback(LuaRef self, LuaRef function) 
    {
        this->self = self;
        this->function = function;
    }

    void Call()
    {
        if (function.isValid()) 
        {
            function.call(self);
        }
    }

    void Release()
    {
        if (self.isValid())
            self.~LuaRef();
        if (function.isValid())
            function.~LuaRef();
    }

    //lua
    static void BindLua(LuaState L) {
        LuaBinding(L).beginClass<luaCallback>("luaCallback")
            .addFactory([](LuaRef self, LuaRef function = LuaRef()) {
                auto callback = new luaCallback(self, function);
            }, LUA_ARGS(LuaRef, _opt<LuaRef>))
            .endClass();
    }
};

class luaDrawableHandler {
private:
    static std::map<int, int> idToIndexMap;
    static std::map<int, int> idToListMap;
    static std::vector<luaCallback> callbackLists[2];
    static uint idProvider;
public:
    static int RegisterCallback(LuaRef self, LuaRef function, int phase);
    static void UnregisterCallback(int id);
    static void ResetLuaCallbacks();

    void CallCallbacks(int phase);
    void ResetCallbacks();

    static void Install();
};