#pragma once
#include "aiVehicle.h"
#include <mm2_audio.h>

namespace MM2
{
    // Forward declarations
    class aiVehicleSpline;

    // External declarations
    extern class AudImpact;
    extern class aiVehicleInstance;
    extern class aiRailSet;

    // Class definitions

    class aiVehicleSpline : public aiVehicle {
    private:
        byte _buffer[0x186];
    protected:
        static hook::Field<0x10, aiRailSet> _railSet;
        static hook::Field<0xD4, aiVehicleInstance*> _vehicleInstance;
        static hook::Field<0xF4, float> _curSpeed;
    public:
        aiVehicleSpline(void)                               DONOTCALL;
        aiVehicleSpline(const aiVehicleSpline &&)           DONOTCALL;

        void UpdateObstacleMap(void)                        { hook::Thunk<0x568410>::Call<void>(this); }

        void Position(Vector3 &a1) override                 FORWARD_THUNK;
        float Speed(void) override                          FORWARD_THUNK;
        int CurrentRoadIdx(aiPath **a1, const bool *a2, int *a3) override
                                                            FORWARD_THUNK;
        int CurrentRdVert(void) override                    FORWARD_THUNK;

        void Update(void) override                          FORWARD_THUNK;
        void Reset(void) override                           FORWARD_THUNK;
        int Type(void) override                             FORWARD_THUNK;
        Matrix34 & GetMatrix(void) override                 FORWARD_THUNK;
        float FrontBumperDistance(void) override            FORWARD_THUNK;
        float BackBumperDistance(void) override             FORWARD_THUNK;
        float LSideDistance(void) override                  FORWARD_THUNK;
        float RSideDistance(void) override                  FORWARD_THUNK;
        int CurrentLane(void) override                      FORWARD_THUNK;
        int CurrentRoadId(void) override                    FORWARD_THUNK;
        void DrawId(void) override                          FORWARD_THUNK;

        virtual void Impact(int a1)                         FORWARD_THUNK;
        virtual AudImpact * GetAudImpactPtr(void)           FORWARD_THUNK;
        virtual void PlayHorn(float a1, float a2)           { hook::Thunk<0x551CA0>::Call<void>(this, a1, a2); }
        virtual void StopVoice(void)                        { hook::Thunk<0x551CB0>::Call<void>(this); }

        //fields
        inline float getCurSpeed()
        {
            return _curSpeed.get(this);
        }

        inline aiRailSet* getRailSet()
        {
            return _railSet.ptr(this);
        }

        aiVehicleInstance * getVehicleInstance() const {
            return _vehicleInstance.get(this);
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<aiVehicleSpline, aiVehicle>("aiVehicleSpline")
                .addPropertyReadOnly("VehicleInstance", &getVehicleInstance)
                .endClass();
        }
    };

    ASSERT_SIZEOF(aiVehicleSpline, 0x18C);
}