#pragma once
#include "aiVehicle.h"
#include "aiPath.h"


namespace MM2
{
    // Forward declarations
    class aiVehiclePhysics;
    struct aiRouteNode;

    // External declarations
    extern class vehCar;
    extern class aiVehicle;

    // Class definitions
    struct  aiRouteNode
    {
        int unk_0x00;
        Vector3 Position;
        float Angle;
        float Distance;
        short unk_18;
        short RoadVertexIndex;
        short unk_1C;
        short unk_1E;
        short unk_20;
        short unk_22;
    };


    class aiVehiclePhysics : public aiVehicle {
    private:
        byte _buffer[0x9760];
    private:
        void initLua(int id, const char* basename, bool circuitMode, int audioType)
        {
            this->Init(id, basename, (circuitMode) ? 1 : 0, audioType);
        }
    protected:
        static hook::Field<0x10, vehCar> _vehCar;
        static hook::Field<0x27C, unsigned short> _state;
        static hook::Field<0x9682, unsigned short> _currentLap;
        static hook::Field<0x9684, unsigned short> _lapCount;
    public:
        aiVehiclePhysics(void)                              DONOTCALL;
        aiVehiclePhysics(const aiVehiclePhysics &&)         DONOTCALL;

        vehCar * GetCar() const
        {
            return _vehCar.ptr(this);
        }

        unsigned short GetState() const
        {
            return _state.get(this);
        }

        int GetCurrentLap() const 
        {
            return _currentLap.get(this);
        }

        int GetLapCount() const
        {
            return _lapCount.get(this);
        }

        void Init(int id, const char* basename, short circuitMode, int audioType)
                                                            { hook::Thunk<0x5593E0>::Call<void>(this, id, basename, circuitMode, audioType); }

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
        void ReplayDebug(void) override                     FORWARD_THUNK;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<aiVehiclePhysics, aiVehicle>("aiVehiclePhysics")
                .addPropertyReadOnly("Car", &GetCar)
                .addPropertyReadOnly("State", &GetState)
                .addPropertyReadOnly("CurrentLap", &GetCurrentLap)
                .addPropertyReadOnly("NumLaps", &GetLapCount)
                .addFunction("Init", &initLua)
                .endClass();
        }
    };

    ASSERT_SIZEOF(aiVehiclePhysics, 0x9770);

    // Lua initialization

}