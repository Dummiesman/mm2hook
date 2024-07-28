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
        Ram = 0x3,
        PushLeft = 0x4,
        PushRight = 0x5,
        Block = 0x6,
        BlockWait = 0x7,
    };


    class aiPoliceOfficer {
    public:
        static bool s_EnableRubberBanding;
    private:
        short unk00; // Unused by default seemingly
        aiVehiclePhysics m_VehiclePhysics;
        vehCar* m_FollowCar;
        short m_ID;
        short m_PoliceState;
        short m_LastPoliceState;
        short m_ApprehendState;
        short m_OpponentChaseDenyList[8];
        short m_WHATISTHIS; // Perhaps a 9th opponent slot?
        float m_FollowCarDistance;
        short m_BehaviorCount;
        short m_AllowedBehaviors[4];
        short m_PerpComponentID; // !!Purposefully mismatches the actual mapping. Angel accidentally used Type instead of ID and vice versa
        short m_PerpComponentType; //  We swap them here so when functions are reimplemented it corrects the issue
        short m_IntersectionIds[100];
        short m_NumIntersectionIds;
    public:
        float OpponentChaseChance;
        float OpponentDetectionRange;
        float ChaseRange;
        bool ChasePlayers;
        bool ChaseOpponents;
    private:
        AGE_API void Push();
        AGE_API void Block();
        AGE_API void Ram();

        AGE_API void ApprehendPerpetrator();
        AGE_API void FollowPerpetrator();
        AGE_API void DetectPerpetrator();
        AGE_API void PerpEscapes(bool playExplosion);

        bool IsPerpBreakingTheLaw(vehCar* perpCar);
    public:
        ANGEL_ALLOCATOR

        aiPoliceOfficer(void);
        ~aiPoliceOfficer();

        void CancelPursuit();
        bool ChaseVehicle(vehCar* chaseMe);
        int GetId() const;
        const aiVehiclePhysics* GetVehiclePhysics() const;
        int GetApprehendState() const;
        vehCar* GetFollowedCar() const;
        vehCar* GetCar() const;
        short GetState() const;
        void SetState(aiVehiclePhysicsState state);
        int GetCurrentLap() const;
        int GetLapCount() const;
        void DrawRouteThroughTraffic() const;
        void DrawId() const;
        int GetPoliceState() const;
        void SetPoliceState(aiPoliceState state);

        void Init(int id);
        void Init(int id, const char* basename, int flags);
        
        AGE_API void Reset();
        AGE_API void Update();
        AGE_API bool InPersuit() const;
        AGE_API void StartSiren();
        AGE_API void StopSiren();
        AGE_API BOOL Collision(vehCar* perpCar);
        AGE_API BOOL HitMe(vehCar* perpCar);
        AGE_API bool IsPerpACop(vehCar* perpCar);
        AGE_API bool WrongWay(vehCar* perpCar);
        AGE_API bool OffRoad(vehCar* perpCar);
        AGE_API bool Fov(vehCar* perpCar);
        AGE_API bool Speeding(vehCar* perpCar);

        static void BindLua(LuaState L);
    };
}