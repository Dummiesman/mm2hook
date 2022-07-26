#pragma once
#include "aiVehiclePhysics.h"

namespace MM2
{
    // Forward declarations
    class mcHookman;

    // External declarations


    // Class definitions

    class mcHookman {
    private:
        byte _buffer[0x9870];
    protected:
        static hook::Field<0x00, aiVehiclePhysics> _vehiclePhysics;
        static hook::Field<0x9794, short> _id;
    public:
        mcHookman(void)                                     DONOTCALL;
        mcHookman(const mcHookman &&)                       DONOTCALL;

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

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mcHookman>("mcHookman")
                .addPropertyReadOnly("ID", &GetId)

                .addPropertyReadOnly("Car", &GetCar)
                .addPropertyReadOnly("State", &GetState)

                .endClass();
        }
    };

    ASSERT_SIZEOF(mcHookman, 0x9870);
}