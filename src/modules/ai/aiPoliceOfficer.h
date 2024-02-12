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
    enum aiPoliceState : __int16
    {
        Idle = 0x0,
        Apprehend = 0x1,
        FollowPerp = 0x2,
        Invalid = 0x5,
        Incapacitated = 0xC,
    };

    enum aiPoliceApprehendState : __int16
    {
        PushLeft = 0x4,
        PushRight = 0x5,
        Block = 0x6,
        BlockWait = 0x7,
    };


    class aiPoliceOfficer {
    private:
        byte _buffer[0x9870];
    protected:
        static hook::Field<0x4, aiVehiclePhysics> _vehiclePhysics;
        static hook::Field<0x9774, vehCar*> _followedCar;
        static hook::Field<0x9778, unsigned short> _id;
        static hook::Field<0x977E, aiPoliceApprehendState> _apprehendState;
        static hook::Field<0x977A, aiPoliceState> _policeState;
        static hook::Field<0x9794, float> _followedCarDistance;
        static hook::Field<0x9798, short> _behaviorCount;
        static hook::Field<0x979A, short> _behaviorList;
        static hook::Field<0x97A2, short> _perpMapCompIndex;
        static hook::Field<0x97A4, short> _perpMapCompType;
    private:
        AGE_API void FollowPerpetrator();
        AGE_API void DetectPerpetrator();
        AGE_API void PerpEscapes();
    public:
        aiPoliceOfficer(void);
        aiPoliceOfficer(const aiPoliceOfficer&&);

        void ChaseVehicle(vehCar* chaseMe);
        int GetId() const;
        aiVehiclePhysics* GetVehiclePhysics() const;
        int GetApprehendState() const;
        vehCar* GetFollowedCar() const;
        vehCar* GetCar() const;
        short GetState() const;
        int GetCurrentLap() const;
        int GetLapCount() const;
        void DrawRouteThroughTraffic() const;
        void DrawId() const;
        int GetPoliceState() const;

        void ChooseRandomAppBehavior();
        
        AGE_API void Reset();
        AGE_API bool InPersuit() const;
        AGE_API void StartSiren();
        AGE_API void StopSiren();

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(aiPoliceOfficer, 0x9870);
}