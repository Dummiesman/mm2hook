#pragma once
#include <modules\mmui\vselect.h>

namespace MM2
{
    // Forward declarations

    // External declarations


    // Class definitions
    class Vehicle : public VehicleSelectBase
    {
    private:
        BOOL SubMenu;
        UIBMButton* GoButton;
    private:
        void setSubMenuLua(bool subMenu) 
        {
            this->SetSubMenu(subMenu ? TRUE : FALSE);
        }
    public:

        /*
            uiMenu virtuals
        */

        AGE_API virtual void PreSetup() override            { hook::Thunk<0x505800>::Call<void>(this); }
        AGE_API virtual void PostSetup() override           { hook::Thunk<0x505810>::Call<void>(this); }

        /*
            Vehicle members
        */
        AGE_API void SetSubMenu(BOOL subMenu)               { hook::Thunk<0x5057C0>::Call<void>(this, subMenu); }


        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<Vehicle, VehicleSelectBase>("Vehicle")
                .addFunction("SetSubMenu", &setSubMenuLua)
                .endClass();
        }
    };
}