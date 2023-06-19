#pragma once
#include "pu_menu.h"

namespace MM2
{
    // Forward declarations
    class PUMenuBase;

    // External declarations


    // Class definitions
    class LuaPUMenu : public PUMenuBase
    {
    private:
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
        LuaPUMenu(int id, float x, float y, float w, float h, LPCSTR backgroundImageName, bool scaleBackground)
            : PUMenuBase(id, x, y, w, h, backgroundImageName, scaleBackground)
        {
            m_PreSetupFunction = getNil();
            m_PostSetupFunction = getNil();
            m_BackUpFunction = getNil();
            m_CheckInputFunction = getNil();
            m_UpdateFunction = getNil();
            m_CullFunction = getNil();
        }

        /*
            asNode virtuals
        */

        AGE_API void Update() override {
            MM2Lua::TryCallFunction(m_UpdateFunction);
            PUMenuBase::Update();
        }


        AGE_API void Cull() override {
            MM2Lua::TryCallFunction(m_CullFunction);
            PUMenuBase::Cull();
        }

        /*
            uiMenu virtuals
        */

        AGE_API virtual void PreSetup() override {
            MM2Lua::TryCallFunction(m_PreSetupFunction);
            PUMenuBase::PreSetup();
        }

        AGE_API virtual void PostSetup() override {
            MM2Lua::TryCallFunction(m_PostSetupFunction);
            PUMenuBase::PostSetup();
        }

        AGE_API virtual void BackUp() override {
            MM2Lua::TryCallFunction(m_BackUpFunction);
            PUMenuBase::BackUp();
        }

        AGE_API virtual void CheckInput() override {
            MM2Lua::TryCallFunction(m_CheckInputFunction);
            PUMenuBase::CheckInput();
        }

        /*
            luaPopup members
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
            LuaBinding(L).beginExtendClass<LuaPUMenu, PUMenuBase>("luaPopupMenu")
                .addFactory([](int id, float x, float y, float w, float h, LuaRef backgroundImageName, bool scaleBackground)
                {
                    LPCSTR bgImageName = nullptr;
                    if (backgroundImageName.type() != LuaTypeID::NIL)
                    {
                        bgImageName = backgroundImageName.toValue<LPCSTR>();
                    }

                    auto object = new LuaPUMenu(id, x, y, w, h, bgImageName, scaleBackground);
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