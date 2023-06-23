#pragma once
#include <modules\mmwidget\menu.h>

namespace MM2
{
    // Forward declarations

    // External declarations


    // Class definitions
    class VehicleSelectBase : public UIMenu
    {
    private:
        char _buffer[0xE4];
    private:
        bool currentVehicleIsLockedLua() 
        {
            return this->CurrentVehicleIsLocked() == TRUE;
        }
    public:

        /*
            asNode virtuals
        */

        AGE_API void Update() override                      { hook::Thunk<0x4F6B60>::Call<void>(this); }
        AGE_API void Reset() override                       { hook::Thunk<0x4F6B50>::Call<void>(this); }

        /*
            uiMenu virtuals
        */

        AGE_API virtual void PreSetup() override            { hook::Thunk<0x4F6A00>::Call<void>(this); }
        AGE_API virtual void PostSetup() override           { hook::Thunk<0x4F6AD0>::Call<void>(this); }

        /*
            VehicleSelectBase members
        */
        AGE_API void AllSetCar(LPCSTR basename, int color) { hook::Thunk<0x4F6AF0>::Call<void>(this, basename, color); }
        AGE_API void SetLastUnlockedVehicle()              { hook::Thunk<0x4F6D90>::Call<void>(this); }
        AGE_API BOOL CurrentVehicleIsLocked()              { return hook::Thunk<0x4F6D30>::Call<BOOL>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<VehicleSelectBase, UIMenu>("VehicleSelectBase")
                .addPropertyReadOnly("CurrentVehicleIsLocked", &currentVehicleIsLockedLua)
                .addFunction("AllSetCar", &AllSetCar)
                .addFunction("SetLastUnlockedVehicle", &SetLastUnlockedVehicle)
                .endClass();
        }
    };
}