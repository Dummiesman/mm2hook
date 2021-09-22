#pragma once
#include <modules\phys\phboundgeom.h>

namespace MM2
{
    // Forward declarations
    class vehBound;

    // External declarations
    extern class lvlMaterial;

    // Class definitions
    class vehBound : public phBoundGeometry
    {
    private:
        lvlMaterial* Material;
    public:

        AGE_API void Init()                                    { hook::Thunk<0x4DA6E0>::Call<void>(this); }

        /*
          phBound Overrides
        */
        AGE_API virtual void SetFriction(float a1)  override    { hook::Thunk<0x4DA700>::Call<void>(this, a1); }
        AGE_API virtual void SetElasticity(float a1)  override  { hook::Thunk<0x4DA710>::Call<void>(this, a1); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehBound, phBoundGeometry>("vehBound")
                .endClass();
        }
    };



    // Lua initialization

}