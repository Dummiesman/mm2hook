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
        byte _buffer[0x17A];
    protected:
        static hook::Field<0x10, aiRailSet> _railSet;
        static hook::Field<0xD4, aiVehicleInstance*> _vehicleInstance;
        static hook::Field<0xD8, Matrix34*> _matrix;
        static hook::Field<0xF4, float> _curSpeed;
        static hook::Field<0xFC, float> _wheelRotation;
    public:
        aiVehicleSpline()                               DONOTCALL;
        aiVehicleSpline(const aiVehicleSpline &&)           DONOTCALL;

        void UpdateObstacleMap();
        void Position(Vector3& a1) override;
        float Speed() override;
        int CurrentRoadIdx(aiPath** a1, const bool* a2, int* a3) override;
        int CurrentRdVert() override;

        void Update() override;
        void Reset() override;
        int Type() override;
        Matrix34& GetMatrix() const override;
        float FrontBumperDistance() override;
        float BackBumperDistance() override;
        float LSideDistance() override;
        float RSideDistance() override;
        int CurrentLane() override;
        int CurrentRoadId() override;
        void DrawId() const override;

        virtual void Impact(int a1);
        virtual AudImpact* GetAudImpactPtr();
        virtual void PlayHorn(float a1, float a2);
        virtual void StopVoice();

        //fields
        float GetSpeed() const;
        void SetSpeed(float speed);
        float GetWheelRotation() const;
        void SetWheelRotation(float rotation);
        aiRailSet* GetRailSet();
        aiVehicleInstance* GetInst() const;
        void SetMatrix(Matrix34 const& mtx);
        void SolveYPositionAndOrientation();

        //lua
        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(aiVehicleSpline, 0x18C);
}