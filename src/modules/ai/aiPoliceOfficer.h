#pragma once
#include <mm2_common.h>
#include "aiVehiclePhysics.h"

namespace MM2
{
    // Forward declarations
    class aiPoliceOfficer;

    // External declarations
    extern class aiVehiclePhysics;

    // Class definitions

    class aiPoliceOfficer {
    private:
        byte _buffer[0x9870];
    protected:
        static hook::Field<0x4, aiVehiclePhysics> _vehiclePhysics;
        static hook::Field<0x9774, vehCar*> _followedCar;
        static hook::Field<0x9778, unsigned short> _id;
        static hook::Field<0x977E, unsigned short> _apprehendState;
        static hook::Field<0x977A, unsigned short> _policeState;
    public:
        aiPoliceOfficer(void)                               DONOTCALL;
        aiPoliceOfficer(const aiPoliceOfficer &&)           DONOTCALL;

        int GetId() const
        {
            return _id.get(this);
        }

        aiVehiclePhysics* GetVehiclePhysics() const 
        {
            return _vehiclePhysics.ptr(this);
        }

        int GetApprehendState() const
        {
            return _apprehendState.get(this);
        }

        vehCar* GetFollowedCar() const
        {
            return _followedCar.get(this);
        }

        vehCar* GetCar() const
        {
            return _vehiclePhysics.ptr(this)->GetCar();
        }

        unsigned short GetState() const
        {
            return _vehiclePhysics.ptr(this)->GetState();
        }

        /// <summary>
        /// The state from aiPoliceForce::State
        /// </summary>        
        int GetPoliceState() const
        {
            return _policeState.get(this);
        }

        AGE_API void StartSiren()                           { hook::Thunk<0x53DBF0>::Call<void>(this); }
        AGE_API void StopSiren()                            { hook::Thunk<0x53DC40>::Call<void>(this); }

        //usually private
        AGE_API void DetectPerpetrator()                    { hook::Thunk<0x53DFD0>::Call<void>(this); }
        AGE_API void PerpEscapes()                          { hook::Thunk<0x53F170>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiPoliceOfficer>("aiPoliceOfficer")
                .addPropertyReadOnly("FollowedCar", &GetFollowedCar)
                .addPropertyReadOnly("PoliceState", &GetPoliceState)
                .addPropertyReadOnly("ApprehendState", &GetApprehendState)
                .addPropertyReadOnly("ID", &GetId)

                .addPropertyReadOnly("Car", &GetCar)
                .addPropertyReadOnly("State", &GetState)

                .addFunction("StartSiren", &StartSiren)
                .addFunction("StopSiren", &StopSiren)

                .endClass();
        }
    };

    ASSERT_SIZEOF(aiPoliceOfficer, 0x9870);
}