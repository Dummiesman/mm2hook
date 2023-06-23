#pragma once
#include "aiVehiclePhysics.h"
#include "aiPath.h"


namespace MM2
{
    // Forward declarations
    class aiVehiclePhysicsWrapper;

    // External declarations

    class aiVehiclePhysicsWrapper : public Base  {
    private:
        aiVehiclePhysics vehiclePhysics;
        short intersectionsBuf[128];
    public:
        ANGEL_ALLOCATOR
        
        vehCar* GetCar() const;
        short GetState() const;
        void SetState(aiVehiclePhysicsState state);
        int GetCurrentLap() const;
        int GetLapCount() const;

        void Init(int id, const char* basename, bool circuitMode, int audioType);
        void Reset();
        void RegisterRoute(LuaRef optsTable);
        void DriveRoute();
        void Mirror(vehCar* car);
        void Stop();

        void DrawRouteThroughTraffic();

        static void BindLua(LuaState L);
    };

    // Lua initialization

}