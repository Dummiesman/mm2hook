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
        Vector3 InertiaBoxCopy; //Copied on Init, used for aiVehiclePlayer distance calculations
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
        float GetBoundFriction() const {
            return this->BoundFriction;
        }

        void SetBoundFriction(float friction) {
            this->BoundFriction = friction;
            this->RestoreImpactParams(); //apply
        }

        float GetBoundElasticity() const {
            return this->BoundElasticity;
        }

        void SetBoundElasticity(float elasticity) {
            this->BoundElasticity = elasticity;
            this->RestoreImpactParams(); //apply
        }

        Vector3 GetInertiaBox() const {
            return this->InertiaBox;
        }

        void SetInertiaBox(Vector3 const & inertiaBox) {
            this->InertiaBox = inertiaBox;
            this->InertiaBoxCopy = inertiaBox;
        }

        void SetAndApplyInertiaBox(Vector3 const & inertiaBox) {
            this->SetInertiaBox(inertiaBox);
            this->GetICS()->InitBoxMass(this->GetMass(), inertiaBox.X, inertiaBox.Y, inertiaBox.Z);
        }

        phInertialCS * GetICS() {
            return &this->InertialCS;
        }

        Matrix34 * GetWorldMatrix(void) {
            return &this->WorldMatrix;
        }

        float GetMass() const {
            return this->Mass;
        }

        void SetMass(float mass) {
            this->Mass = mass;
        }

        void SetAndApplyMass(float mass) {
            this->SetMass(mass);
            
            auto box = this->GetInertiaBox();
            this->GetICS()->InitBoxMass(mass, box.X, box.Y, box.Z);
        }

        int GetDrivetrainType() const {
            return this->DrivetrainType;
        }

        void SetDrivetrainType(int type) {
            if (type < 0 || type > 2) //0 = RWD, 1 = FWD, 2 = 4WD
                return;
            this->DrivetrainType = type;
        }

        float GetThrottle() const {
            return this->Engine.GetThrottleInput();
        }

        void SetThrottle(float throttle) {
            this->Engine.SetThrottleInput(throttle);
        }

        float GetSteering() const {
            return this->SteeringInput;
        }

        void SetSteering(float steering) {
            this->SteeringInput = steering;
        }

        float GetBrake() const {
            return this->BrakeInput;
        }

        void SetBrake(float brake) {
            this->BrakeInput = brake;
        }

        float GetHandbrake() const {
            return this->HandBrakeInput;
        }

        void SetHandbrake(float handBrake) {
            this->HandBrakeInput = handBrake;
        }

        float GetSpeed() const {
            return this->Speed;
        }

        float GetSpeedMPH() const {
            return this->SpeedInMph;
        };

        Vector3 GetResetPos() const {
            return this->ResetPosition - this->CenterOfGravity;
        }

        float GetResetRotation() const {
            return this->ResetRotation;
        }

        void SetResetRotation(float rotation) {
            this->ResetRotation = rotation;
        }

        lvlInstance * GetInstance() {
            return this->LvlInstancePtr;
        }

        vehTransmission * GetTransmission() {
            return &this->Transmission;
        }

        vehEngine * GetEngine() {
            return &this->Engine;
        }

        vehDrivetrain * GetDrivetrain() {
            return &this->PrimaryDrivetrain;
        }

        vehDrivetrain * GetLeftFreetrain() {
            return &this->FreetrainLeft;
        }

        vehDrivetrain * GetRightFreetrain() {
            return &this->FreetrainRight;
        }

        vehWheel * GetWheel(int num) {
            switch (num) {
            case 0:
                return &WheelFrontLeft;
            case 1:
                return &WheelFrontRight;
            case 2:
                return &WheelBackLeft;
            case 3:
                return &WheelBackRight;
            }
            return nullptr;
        }

        vehAero * GetAero() {
            return &this->Aero;
        }

        vehAxle * GetFrontAxle() {
            return &this->AxleFront;
        }

        vehAxle * GetRearAxle() {
            return &this->AxleRear;
        }

        AGE_API vehCarSim()                                 { hook::Thunk<0x4CB660>::Call<void>(this); }
        AGE_API ~vehCarSim()                                { hook::Thunk<0x4CB8E0>::Call<void>(this); }

        AGE_API void Init(const char* basename, phColliderJointed* colliderPtr, vehCarModel* carModelPtr)
        {
            //Call original 
            hook::Thunk<0x4CBB80>::Call<void>(this, basename, colliderPtr, carModelPtr);

            //We've expanded this class. Now for *our new stuff*
            Matrix34 diffMatrix;
            
            if (GetPivot(diffMatrix, basename, "whl4")) {
                BackBackLeftWheelPosDiff = diffMatrix.GetRow(3) - GetWheel(2)->GetCenter();
            }

            if (GetPivot(diffMatrix, basename, "whl5")) {
                BackBackRightWheelPosDiff = diffMatrix.GetRow(3) - GetWheel(3)->GetCenter();
            }
        }

        AGE_API int BottomedOut()                           { return hook::Thunk<0x4CBB40>::Call<int>(this); }
        AGE_API int OnGround()                              { return hook::Thunk<0x4CBB00>::Call<int>(this); }
        AGE_API void ReconfigureDrivetrain()                { hook::Thunk<0x4CC0B0>::Call<void>(this); }
        AGE_API void SetHackedImpactParams()                { hook::Thunk<0x4CC080>::Call<void>(this); }
        AGE_API void RestoreImpactParams()                  { hook::Thunk<0x4CC050>::Call<void>(this); }
        AGE_API void SetResetPos(Vector3 const & a1)        { hook::Thunk<0x4CC830>::Call<void>(this, &a1); }
        
        /*
            asNode virtuals
        */

        AGE_API void Update() override                      { hook::Thunk<0x4CC8E0>::Call<void>(this); }
        AGE_API void Reset() override                       { hook::Thunk<0x4CBA70>::Call<void>(this); }
        AGE_API void FileIO(datParser &parser)  override
                                                            { hook::Thunk<0x4CCC70>::Call<void>(this, &parser); }
        AGE_API char* GetClassName() override               { return hook::Thunk<0x4CCF10>::Call<char*>(this); }
        AGE_API char const* GetDirName() override           { return hook::Thunk<0x4CBAF0>::Call<char const*>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehCarSim, asNode>("vehCarSim")
                .addPropertyReadOnly("Drivetrain", &GetDrivetrain)
                .addPropertyReadOnly("LeftFreetrain", &GetLeftFreetrain)
                .addPropertyReadOnly("RightFreetrain", &GetRightFreetrain)

                .addPropertyReadOnly("Aero", &GetAero)
                .addPropertyReadOnly("FrontAxle", &GetFrontAxle)
                .addPropertyReadOnly("RearAxle", &GetRearAxle)
                .addPropertyReadOnly("Transmission", &GetTransmission)
                .addPropertyReadOnly("Engine", &GetEngine)
                .addPropertyReadOnly("Speed", &GetSpeedMPH)

                .addPropertyReadOnly("WorldMatrix", &GetWorldMatrix)

                .addVariable("CenterOfGravity", &vehCarSim::CenterOfGravity)
                .addVariable("SSSValue", &vehCarSim::SSSValue)
                .addVariable("SSSThreshold", &vehCarSim::SSSThreshold)
                .addVariable("CarFrictionHandling", &vehCarSim::CarFrictionHandling)

                .addProperty("Mass", &GetMass, &SetAndApplyMass)
                .addProperty("InertiaBox", &GetInertiaBox, &SetAndApplyInertiaBox)
                .addProperty("BoundFriction", &GetBoundFriction, &SetBoundFriction)
                .addProperty("BoundElasticity", &GetBoundElasticity, &SetBoundElasticity)
                .addProperty("DrivetrainType", &GetDrivetrainType, &SetDrivetrainType)
                
                .addProperty("ResetRotation", &GetResetRotation, &SetResetRotation)
                .addProperty("ResetPosition", &GetResetPos, &SetResetPos)

                .addProperty("Steering", &GetSteering, &SetSteering)
                .addProperty("Brake", &GetBrake, &SetBrake)
                .addProperty("Handbrake", &GetHandbrake, &SetHandbrake)
                .addProperty("Throttle", &GetThrottle, &SetThrottle)

                .addFunction("GetWheel", &GetWheel)
                
                .addFunction("Init", &Init)
                .addFunction("BottomedOut", &BottomedOut)
                .addFunction("OnGround", &OnGround)
                .addFunction("ReconfigureDrivetrain", &ReconfigureDrivetrain)
                .addFunction("SetHackedImpactParams", &SetHackedImpactParams)
                .addFunction("RestoreImpactParams", &RestoreImpactParams)
                .addFunction("SetResetPos", &SetResetPos)
                .addFunction("GetICS", &GetICS)
            .endClass();
        }
    };

    ASSERT_SIZEOF(vehCarSim, 0x1560 + 0xC + 0xC + 0xC + 0xC); //+4 extra fields
}