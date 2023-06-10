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
    private:
        void registerRouteLua(std::vector<short> intersectionIds, Vector3 const & endPosition, Vector3 const & endOrientation,short numLaps, float targetSpeed) {
            //RegisterRoute(intersectionIds.data(), intersectionIds.size(), endPosition, endOrientation, numLaps, targetSpeed);
        }
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

        int GetCurrentLap() const
        {
            return _vehiclePhysics.ptr(this)->GetCurrentLap();
        }

        int GetLapCount() const
        {
            return _vehiclePhysics.ptr(this)->GetLapCount();
        }

        AGE_API void Init(int id, const char* raceDir)          { hook::Thunk<0x53D060>::Call<void>(this, id, raceDir);}
        AGE_API int Finished()                                  { return hook::Thunk<0x53D6E0>::Call<int>(this); }
        AGE_API void RegisterRoute(short* intersectionIDs, short numIntersections, Vector3 const & endPosition, Vector3 const & endOrientation,
                                   short numLaps = 0, float targetSpeed = 9999.0f, float a8 = 0.0f, bool unkFlag = false, bool avoidTraffic = true, bool avoidProps = true, bool avoidPlayers = true,
                                   bool avoidOpponents = true, bool weirdPathfinding = false, float a15 = 1.0f, float a16 = 2.0f, float a17 = 0.7f, float a18 = 75.0f)
                                                                { hook::Thunk<0x5598A0>::Call<void>(this, intersectionIDs, numIntersections, &endPosition, &endOrientation, 
                                                                                                    numLaps, targetSpeed, a8, unkFlag, avoidTraffic, avoidProps, avoidPlayers,
                                                                                                    avoidOpponents, weirdPathfinding, a15, a16, a17, a18); }

        void DrawRouteThroughTraffic() 
        {
            this->GetVehiclePhysics()->DrawRouteThroughTraffic();
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiRouteRacer>("aiRouteRacer")
                .addPropertyReadOnly("ID", &GetId)
                .addPropertyReadOnly("Car", &GetCar)
                .addPropertyReadOnly("State", &GetState)
                .addPropertyReadOnly("CurrentLap", &GetCurrentLap)
                .addPropertyReadOnly("NumLaps", &GetLapCount)

                .addFunction("Init", &Init)
                .addFunction("DrawRouteThroughTraffic", &DrawRouteThroughTraffic)
                .addFunction("Finished", &Finished)
                .addFunction("RegisterRoute", &registerRouteLua)
            .endClass();
        }
    };

    ASSERT_SIZEOF(aiRouteRacer, 0x9794);
}