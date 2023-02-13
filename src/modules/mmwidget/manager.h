#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class MenuManager;

    // External declarations

    // Class definitions
    class MenuManager : public asNode {
    private:
        char _buf[0x138];
    private:
        static hook::Type<MenuManager*> Instance;

        static hook::Field<0x138, int> _currentMenuId;
        static hook::Field<0x114, int> _lastMenuId;
    public:
        static MenuManager* GetInstance()
        {
            return Instance.get();
        }

        int GetCurrentMenuID() const {
            return _currentMenuId.get(this);
        }

       void Switch(int menuID)                                   { hook::Thunk<0x4E5A30>::Call<void>(this, menuID); }

       static void BindLua(LuaState L) {
           LuaBinding(L).beginExtendClass<MenuManager, asNode>("MenuManager")
               .addStaticProperty("Instance", &GetInstance)
               .addPropertyReadOnly("CurrentMenuID", &GetCurrentMenuID)
               .addFunction("Switch", &Switch)
               .endClass();
       }

    };
    ASSERT_SIZEOF(MenuManager, 0x150);
}