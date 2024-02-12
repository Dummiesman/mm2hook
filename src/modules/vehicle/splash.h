#pragma once
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class vehSplash;

    // External declarations


    // Class definitions

    class vehSplash : public asNode {
    private:
        phInertialCS* ICS;
        float WaterLevel;
        float Buoyancy;
        float Dampening;
        Vector3 WaterLUT[64];
    protected:
        //Lua helpers
        void Deactivate() {
            this->setActive(false);
        }
    public:
        AGE_API vehSplash() {
            scoped_vtable x(this);
            hook::Thunk<0x4D6A00>::Call<void>(this);
        }

        AGE_API ~vehSplash() {
            scoped_vtable x(this);
            hook::Thunk<0x4D6F30>::Call<void>(this);
        }

        /*
            asNode virtuals
        */

        AGE_API void Update() override                      { hook::Thunk<0x4D6BF0>::Call<void>(this); }
        AGE_API void Reset() override                       { hook::Thunk<0x4D6A70>::Call<void>(this); }

        /*
            vehSplash members
        */

        AGE_API void Init(phInertialCS* ICS, Vector3 const & min, Vector3 const & max)
                                                            { hook::Thunk<0x4D6A80>::Call<void>(this, ICS, &min, &max); }
        AGE_API void Activate(float waterHeight)            { hook::Thunk<0x4D6BD0>::Call<void>(this, waterHeight); }

        /*
            Lua Bindings
        */
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehSplash, asNode>("vehSplash")
                .addFactory([]() { return new vehSplash(); })
                .addFunction("Init", &Init)
                .addFunction("Activate", &Activate)
                .addFunction("Deactivate", &Deactivate)
                .endClass();
        }
    };


    // Lua initialization

}