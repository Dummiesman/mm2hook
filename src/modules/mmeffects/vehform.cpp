#include "vehform.h"


namespace MM2
{
    AGE_API mmVehicleForm::mmVehicleForm(void) {
        scoped_vtable x(this);
        hook::Thunk<0x5339E0>::Call<void>(this);
    }

    AGE_API mmVehicleForm::~mmVehicleForm(void) {
        scoped_vtable x(this);
        hook::Thunk<0x533AA0>::Call<void>(this);
    }

    /*
        asNode virtuals
    */

    AGE_API void mmVehicleForm::Cull() { hook::Thunk<0x534070>::Call<void>(this); };
    AGE_API void mmVehicleForm::Update() { hook::Thunk<0x534040>::Call<void>(this); };

    AGE_API void mmVehicleForm::SetShape(LPCSTR basename, LPCSTR lodName, LPCSTR shadowModelName, Vector3* unknown)
    {
        hook::Thunk<0x533BF0>::Call<void>(this, basename, lodName, shadowModelName, unknown);
    }
    
    void mmVehicleForm::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<mmVehicleForm, asNode>("mmVehicleForm")
            .endClass();
    }
}