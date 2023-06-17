#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class UIMenu;

    // External declarations

    // Class definitions
    class UIMenu : public asNode {
    public:
        uint32_t ActionSource;
        uint32_t ActionState;
        uint32_t dword20;
        uint32_t MenuID;
        uint32_t dword28;
        uint32_t WidgetCount;
        uint32_t dword30;
        uiWidget **ppWidgets;
        const char *Name;
        uint32_t dword3C;
        uint32_t dword40;
        uint32_t dword44;
        Vector2 Position;
        Vector2 Scale;
        uint32_t dword58;
        float ScaleX;
        float ScaleY;
        uint32_t dword64;
        uint32_t *pCurrentWidgetID;
        uint32_t ActiveWidgetID;
        uint32_t dword70;
        uint32_t WidgetID;
        uint32_t dword78;
        uint32_t dword7C;
        float dword80;
        uint32_t dword84;
        const char *Background;
    public:
        AGE_API UIMenu(int id) {
            scoped_vtable x(this);
            hook::Thunk<0x4E0770>::Call<void>(this, id);
        }

        /*
            asNode virtuals
        */

        AGE_API void Update() override                      { hook::Thunk<0x4E0A80>::Call<void>(this); }

        /*
            uiMenu virtuals
        */

        AGE_API virtual void PreSetup()                     { hook::Thunk<0x4E0A60>::Call<void>(this); }
        AGE_API virtual void PostSetup()                    { hook::Thunk<0x4E0A70>::Call<void>(this); }
        AGE_API virtual void BackUp()                       { hook::Thunk<0x4E0B10>::Call<void>(this); }
        AGE_API virtual void CheckInput()                   { hook::Thunk<0x4E0DE0>::Call<void>(this); }
        AGE_API virtual BOOL IsAnOptionMenu()               { return hook::Thunk<0x4E2D70>::Call<BOOL>(this); }

        /*
            uiMenu members
        */
        AGE_API UIButton * AddButton(int id, LocString *text, float x, float y, float w, float h, int fontNum, int type, MM2::datCallback callback, int i3) {
            return hook::Thunk<0x4E1A90>::Call<UIButton*>(this, id, text, x, y, w, h, fontNum, type, callback, i3);
        };

        AGE_API void AssignBackground(LPCSTR imageName)
        {
            hook::Thunk<0x4E0980>::Call<void>(this, imageName);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UIMenu, asNode>("UIMenu")
                .addFactory([](int id) {
                    auto object = new UIMenu(id);
                    MM2Lua::MarkForCleanupOnShutdown(object);
                    return object;
                })
                .addFunction("AssignBackground", &AssignBackground)
                .endClass();
        }
    };
}