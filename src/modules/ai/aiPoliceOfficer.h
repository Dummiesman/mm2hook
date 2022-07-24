#pragma once
#include "aiVehiclePhysics.h"

namespace MM2
{
    // Forward declarations
    class aiPoliceOfficer;

    // External declarations
    extern class aiVehiclePhysics;

    // Class definitions

    class aiPoliceOfficer : public aiVehiclePhysics {
    private:
        hook::Field<0x9774, vehCar*> _followedCar;
        hook::Field<0x9778, unsigned short> _id;
        hook::Field<0x977E, unsigned short> _apprehendState;
        hook::Field<0x977A, unsigned short> _policeState;
        byte _buffer[0x100-4];
    public:
        aiPoliceOfficer(void)                               DONOTCALL;
        aiPoliceOfficer(const aiPoliceOfficer &&)           DONOTCALL;

        int GetId() const
        {
            return _id.get(this);
        }

        int GetApprehendState() const
        {
            return _apprehendState.get(this);
        }

        vehCar* GetFollowedCar() const
        {
            return _followedCar.get(this);
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
            LuaBinding(L).beginExtendClass<aiPoliceOfficer, aiVehiclePhysics>("aiPoliceOfficer")
                .addPropertyReadOnly("FollowedCar", &GetFollowedCar)
                .addPropertyReadOnly("PoliceState", &GetPoliceState)
                .addPropertyReadOnly("ApprehendState", &GetApprehendState)
                .addPropertyReadOnly("ID", &GetId)

                .addFunction("StartSiren", &StartSiren)
                .addFunction("StopSiren", &StopSiren)

                .endClass();
        }
    };

    ASSERT_SIZEOF(aiPoliceOfficer, 0x9870);

    // Lua initialization

}