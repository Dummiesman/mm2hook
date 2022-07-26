#pragma once
#include "aiVehiclePhysics.h"

namespace MM2
{
    // Forward declarations
    class aiRouteRacer;

    // External declarations
    

    // Class definitions

    class aiRouteRacer {
    private:
        byte _buffer[0x9794];
    protected:
        static hook::Field<0x00, aiVehiclePhysics> _vehiclePhysics;
        static hook::Field<0x9780, unsigned short> _id;
    public:
        aiRouteRacer(void)                                  DONOTCALL;
        aiRouteRacer(const aiRouteRacer &&)                 DONOTCALL;

        int GetId() const
        {
            return _id.get(this);
        }

        aiVehiclePhysics* GetVehiclePhysics() const 
        {
            return _vehiclePhysics.ptr(this);
        }

        vehCar* GetCar() const
        {
            return _vehiclePhysics.ptr(this)->GetCar();
        }

        unsigned short GetState() const
        {
            return _vehiclePhysics.ptr(this)->GetState();
        }

        AGE_API int Finished()                              { return hook::Thunk<0x53D6E0>::Call<int>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiRouteRacer>("aiRouteRacer")
                .addFunction("Finished", &Finished)

                .addPropertyReadOnly("ID", &GetId)
                .addPropertyReadOnly("Car", &GetCar)
                .addPropertyReadOnly("State", &GetState)
            .endClass();
        }
    };

    ASSERT_SIZEOF(aiRouteRacer, 0x9794);
}