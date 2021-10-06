#pragma once
#include <modules\ai.h>

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
        hook::Field<0x04, aiVehiclePhysics> _physics;
        hook::Field<0x9778, unsigned short> _id;
        hook::Field<0x977E, unsigned short> _apprehendState;
        hook::Field<0x977A, unsigned short> _policeState;
        byte _buffer[0x986C];
    public:
        aiPoliceOfficer(void)                               DONOTCALL;
        aiPoliceOfficer(const aiPoliceOfficer &&)           DONOTCALL;

        inline aiVehiclePhysics * getVehiclePhysics() 
        {
            return _physics.ptr(this);
        }

        inline int getId()
        {
            return _id.get(this);
        }

        inline int getState()
        {
            return getVehiclePhysics()->getState();
        }

        inline int getApprehendState()
        {
            return _apprehendState.get(this);
        }

        /// <summary>
        /// The state from aiPoliceForce::State
        /// </summary>        
        inline int getPoliceState()
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
                .addPropertyReadOnly("State", &getState)
                .addPropertyReadOnly("PoliceState", &getPoliceState)
                .addPropertyReadOnly("ApprehendState", &getApprehendState)
                .addPropertyReadOnly("ID", &getId)
                .endClass();
        }
    };

    ASSERT_SIZEOF(aiPoliceOfficer, 0x9870);

    // Lua initialization

}