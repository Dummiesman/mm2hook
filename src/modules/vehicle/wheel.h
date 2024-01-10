#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>
#include <modules\phys\phinertia.h>
#include <modules\level\material.h>
#include <modules\model\getpivot.h>

namespace MM2
{
    // Forward declarations
    class vehWheel;

    // External declarations
    extern class vehCarSim;

    // Class definitions
    class vehWheel : public asNode {
    private:
        vehCarSim *m_CarSimPtr;
        phInertialCS *m_InertialCSPtr;
        int WheelFlags;
        Matrix34 WheelMatrix;
        float TireDispLimitLat;
        float TireDispLimitLong;
        float TireDampCoefLat;
        float TireDampCoefLong;
        float TireDragCoefLat;
        float TireDragCoefLong;
        float SteeringLimit;
        float CamberLimit;
        float WobbleLimit;
        float BrakeCoef;
        float HandbrakeCoef;
        float SteeringOffset;
        float SuspensionLimit;
        float SuspensionExtent;
        float SuspensionFactor;
        float SuspensionDampCoef;
        byte _buffer0[0xD8];
        BOOL m_IsGrounded;
        byte _buffer1[0x34];
        Vector3 LastHitPosition;
        Vector3 Center;
        float Radius;
        float Width;
        float NormalLoad;
        float BumpDisplacement;
        float MaterialDrag;
        float MaterialFriction;
        float MaterialDepth;
        float MaterialHeight;
        float MaterialWidth;
        byte _buffer2[0x4];
        float AccumulatedRotation;
        float InputBrakeAmount;
        float BrakeCoefLoaded;
        float HandbrakeCoefLoaded;
        float SteerAmount;
        float TargetSuspensionTravel;
        float CurrentSuspensionForce;
        byte _buffer3[0x4];
        float SuspensionCompressionRate;
        float SuspensionForceTwo_;
        float SuspensionMaxForce;
        float SuspensionRestingPosition;
        float SuspensionDampForce;
        float WobbleAmount;
        float CamberAmount;
        float LastSlippage;
        __int16 MajorlySlipping;
        __int8 LastGroundedStatus;
        bool BottomedOut;
        float CurrentTireDispLat;
        float CurrentTireDispLong;
        float DispLatRate;
        float DispLongRate;
        float DispLongRateScaled;
        float RotationRate;
        float LatSlipPercent;
        float LongSlipPercent;
        float DispLimitLongLoaded;
        float DampCoefLongLoaded;
        float OptimumSlipPercent;
        float StaticFric;
        float SlidingFric;
        float DispLimitLatLoaded;
        float DampCoefLatLoaded;
        byte _buffer5[0x4];
        lvlMaterial *CurrentPhysicsMaterial;
    private:
        std::tuple<float, float> luaComputeFriction(float slip) const;
        static hook::Type<float> WeatherFriction;
    public:
        AGE_API vehWheel();

        AGE_API void Init(vehCarSim* carSimPtr, const char* vehicleBasename, const char* wheelName, Vector3 centerOfGravity, phInertialCS* inertialCs, int a6, int a7);

        AGE_API void CopyVars(vehWheel* copyFrom);

        AGE_API void ComputeConstants();
        AGE_API void AddNormalLoad(float loadKG);
        AGE_API void SetNormalLoad(float loadKG);
        AGE_API void SetInputs(float steer, float brake, float handbrake);

        AGE_API int GetSurfaceSound();
        AGE_API float GetVisualDispVert();
        AGE_API float GetVisualDispLat();
        AGE_API float GetVisualDispLong();

        AGE_API float ComputeFriction(float slip, float* vF) const;

        /*                                                    
            asNode virtuals                                  
        */                                                    

        AGE_API void Reset() override;
        AGE_API void Update() override;
        AGE_API void FileIO(datParser& parser) override;
        AGE_API char* GetClassName() override;

        /*
            vehWheel
        */

        Matrix34 GetMatrix() const;
        bool IsGrounded() const;
        lvlMaterial* GetCurrentPhysicsMaterial();
        float GetRadius() const;
        void SetRadius(float radius);
        float GetWidth() const;
        void SetWidth(float width);
        Vector3 GetCenter() const;
        void SetCenter(Vector3 const& center);
        float GetLatSlipPercent() const;
        float GetLongSlipPercent() const;
        float GetRotationRate() const;

        static float GetWeatherFriction();
        static void SetWeatherFriction(float friction);
                                                              
        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(vehWheel, 0x26C);
}