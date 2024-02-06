#pragma once
#include <mm2_model.h>

#include "transmission.h"
#include "engine.h"
#include "drivetrain.h"
#include "wheel.h"
#include "aero.h"
#include "axle.h"
#include "suspension.h"

namespace MM2
{
    // Forward declarations
    class vehCarSim;

    // External declarations
    extern class datParser;
    extern class lvlInstance;
    extern class phInertialCS;
    extern class phCollider;
    extern class vehCarModel;

    // Class definitions

    class vehCarSim : public asNode {
    protected:
        phInertialCS InertialCS;
        phCollider *ColliderPtr;
        lvlInstance *LvlInstancePtr;
        Matrix34 WorldMatrix;
        Vector3 CenterOfGravity;
        Vector3 ResetPosition;
        Vector3 Size; // * without mm2hook this defaults to the same value as InertiaBox, but the name is correct
        Vector3 InertiaBox;
        int unknown564;
        float BoundFriction;
        float BoundElasticity;
        int BoundPtr;
        float Mass;
        float Speed;
        float SpeedInMph;
        float ResetRotation;
        int DrivetrainType;  //0 = RWD, 1 = FWD, 2 = 4WD
        int ConfiguredDrivetrainType;  //0 = RWD, 1 = FWD, 2 = 4WD
        vehEngine Engine;
        vehTransmission Transmission;
        vehDrivetrain PrimaryDrivetrain;
        vehDrivetrain FreetrainLeft;
        vehDrivetrain FreetrainRight;
        vehWheel WheelFrontLeft;
        vehWheel WheelFrontRight;
        vehWheel WheelBackLeft;
        vehWheel WheelBackRight;
    public:
        vehAxle AxleFront;
        vehAxle AxleRear;
        vehSuspension ShaftSuspensions[2]; //SHAFT2-3
        vehSuspension ArmSuspensions[4]; //ARM0-4
        vehSuspension ShockSuspensions[4]; //SHOCK0-4
    protected:
        vehAero Aero;
        float CarFrictionHandling;
        int unknown5440;
        float unknown5444;
        int unknown5448;
        float BrakeInput;
        float HandBrakeInput;
        float SteeringInput;
        float SSSValue;
        float SSSThreshold;
    public:
        //EXTRA FIELDS. The hook expands on this class, this is only possible because it's only used like a pointer in the original MM code
        //These are the position differences from (WHL5-WHL3) and (WHL4-WHL2) / (TWHL5-TWHL3) and (TWHL4-TWHL2) for trailers
        Vector3 BackBackLeftWheelPosDiff;
        Vector3 BackBackRightWheelPosDiff;
        Vector3 TrailerBackBackLeftWheelPosDiff;
        Vector3 TrailerBackBackRightWheelPosDiff;
    public:
        float GetBoundFriction() const;
        void SetBoundFriction(float friction);
        float GetBoundElasticity() const;
        void SetBoundElasticity(float elasticity);
        Vector3 GetInertiaBox() const;
        void SetInertiaBox(Vector3 const& inertiaBox);
        void SetAndApplyInertiaBox(Vector3 const& inertiaBox);
        Vector3 GetCenterOfGravity() const;
        Vector3 GetSize() const;
        void SetSize(Vector3 const& size);
        phInertialCS* GetICS();
        Matrix34* GetWorldMatrix(void);
        float GetMass() const;
        void SetMass(float mass);
        void SetAndApplyMass(float mass);
        int GetDrivetrainType() const;
        void SetDrivetrainType(int type);
        float GetThrottle() const;
        void SetThrottle(float throttle);
        float GetSteering() const;
        void SetSteering(float steering);
        float GetBrake() const;
        void SetBrake(float brake);
        float GetHandbrake() const;
        void SetHandbrake(float handBrake);
        float GetSpeed() const;
        float GetSpeedMPH() const;
        Vector3 GetResetPos() const;
        float GetResetRotation() const;
        void SetResetRotation(float rotation);
        lvlInstance* GetInstance();
        vehTransmission* GetTransmission();
        vehEngine* GetEngine();
        vehDrivetrain* GetDrivetrain();
        vehDrivetrain* GetLeftFreetrain();
        vehDrivetrain* GetRightFreetrain();
        vehWheel* GetWheel(int num);
        vehAero* GetAero();
        vehAxle* GetFrontAxle();
        vehAxle* GetRearAxle();

        AGE_API vehCarSim();
        AGE_API ~vehCarSim();

        AGE_API void Init(const char* basename, phColliderJointed* colliderPtr, vehCarModel* carModelPtr);

        AGE_API int BottomedOut();
        AGE_API int OnGround();
        AGE_API void ReconfigureDrivetrain();
        AGE_API void SetHackedImpactParams();
        AGE_API void RestoreImpactParams();
        AGE_API void SetResetPos(Vector3 const& a1);
        
        /*
            asNode virtuals
        */

        AGE_API void Update() override;
        AGE_API void Reset() override;
        AGE_API void FileIO(datParser& parser)  override;
        AGE_API char* GetClassName() override;
        AGE_API char const* GetDirName() override;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(vehCarSim, 0x1560 + 0xC + 0xC + 0xC + 0xC); //+4 extra fields
}