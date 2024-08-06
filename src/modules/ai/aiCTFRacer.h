#pragma once
#include "aiVehiclePhysics.h"

namespace MM2
{
    // Forward declarations
    class aiCTFRacer;

    // External declarations
    extern class Base;

    // Class definitions

    class aiCTFRacer : public Base 
    {
    private:
        Vector3 FlagPosition;
        Vector3 LastFlagPosition;
        Vector3 DeliverPosition;
        aiVehiclePhysics VehiclePhysics;
        byte dword_97bc[2];
        __int16 ID;
        __int16 CTFState;
        __int16 LastCTFState;
        int FlagRoom;
        int DeliverRoom;
        int LastPreStateRoom;
        __int16 RouteIntersections[100];
        __int16 NumRouteIntersections;
        uint TimerTicks; // mm2hook addition to sync logic with MC
    private:
        void PreAquireFlag();
        void AquireFlag();
        void PreDeliverFlag();
        void DeliverFlag();
    public:
        aiCTFRacer(void)                                    DONOTCALL;
        aiCTFRacer(const aiCTFRacer &&)                     DONOTCALL;

        const aiVehiclePhysics* GetVehiclePhysics() const;
        vehCar* GetCar() const;
        int GetId() const;
        short GetState() const;
        bool CanRepairDamage() const;
        void SetCanRepairDamage(bool value);

        Vector3 GetFlagPosition() const;
        void SetFlagPosition(Vector3 pos);

        Vector3 GetDeliverPosition() const;
        void SetDeliverPosition(Vector3 pos);

        void EnterAcquireMode();
        void EnterDeliverMode();

        AGE_API void Init(int id, const char* basename);
        AGE_API void Reset();
        AGE_API void Update();

        void DrawRouteThroughTraffic();

        static void BindLua(LuaState L);
    };

    //ASSERT_SIZEOF(aiCTFRacer, 0x9878);
    ASSERT_SIZEOF(aiCTFRacer, 0x9878 + 0x4); // added Timer

    // Lua initialization

}