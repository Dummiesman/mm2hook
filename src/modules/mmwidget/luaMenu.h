#pragma once
#include "menu.h"

namespace MM2
{
    // Forward declarations
    class luaMenu;

    // External declarations

    // Class definitions
    class luaMenu : public UIMenu {
    private:
        bool isAnOptionMenu;
        LuaRef m_PreSetupFunction;
        LuaRef m_PostSetupFunction;
        LuaRef m_BackUpFunction;
        LuaRef m_CheckInputFunction;
        LuaRef m_UpdateFunction;
        LuaRef m_CullFunction;
    private:
        LuaRef getNil() {
            return LuaRef();
        }
    public:
        luaMenu(int id) : UIMenu(id) {
            m_PreSetupFunction = getNil();
            m_PostSetupFunction = getNil();
            m_BackUpFunction = getNil();
            m_CheckInputFunction = getNil();
            m_UpdateFunction = getNil();
            m_CullFunction = getNil();
            isAnOptionMenu = false;
        }

        /*
            asNode virtuals
        */

        AGE_API void Update() override { 
            MM2Lua::TryCallFunction(m_UpdateFunction);
            UIMenu::Update();
        }


        AGE_API void Cull() override {
            MM2Lua::TryCallFunction(m_CullFunction);
            UIMenu::Cull();
        }

        /*
            uiMenu virtuals
        */

        AGE_API virtual void PreSetup() override {
            MM2Lua::TryCallFunction(m_PreSetupFunction);
            UIMenu::PreSetup();
        }

        AGE_API virtual void PostSetup() override {
            MM2Lua::TryCallFunction(m_PostSetupFunction); 
            UIMenu::PostSetup();
        }

        AGE_API virtual void BackUp() override { 
            MM2Lua::TryCallFunction(m_BackUpFunction); 
            UIMenu::BackUp();
        }

        AGE_API virtual void CheckInput() override {
            MM2Lua::TryCallFunction(m_CheckInputFunction); 
            UIMenu::CheckInput();
        }
        
        AGE_API virtual BOOL IsAnOptionMenu() override      { return this->isAnOptionMenu ? TRUE : FALSE; }

        /*
            luaMenu members
        */
        void SetUpdateFunction(LuaRef function) {
            if (function.isFunction() && function.isValid())
                m_UpdateFunction = function;
        }

        void SetCullFunction(LuaRef function) {
            if (function.isFunction() && function.isValid())
                m_CullFunction = function;
        }

        void SetPreSetupFunction(LuaRef function) {
            if (function.isFunction() && function.isValid())
                m_PreSetupFunction = function;
        }

        void SetPostSetupFunction(LuaRef function) {
            if (function.isFunction() && function.isValid())
                m_PostSetupFunction = function;
        }

        void SetBackUpFunction(LuaRef function) {
            if (function.isFunction() && function.isValid())
                m_BackUpFunction = function;
        }

        void SetCheckInputFunction(LuaRef function) {
            if (function.isFunction() && function.isValid())
                m_CheckInputFunction = function;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<luaMenu, UIMenu>("luaMenu")
                .addFactory([](int id) {
                    auto object = new luaMenu(id);
                    return object;
                })
                .addProperty("PreSetup", &getNil, &SetPreSetupFunction)
                .addProperty("PostSetup", &getNil, &SetPostSetupFunction)
                .addProperty("BackUp", &getNil, &SetBackUpFunction)
                .addProperty("CheckInput", &getNil, &SetCheckInputFunction)
                .addProperty("Update", &getNil, &SetUpdateFunction)
                .addProperty("Cull", &getNil, &SetCullFunction)
                .endClass();
        }
    };
}