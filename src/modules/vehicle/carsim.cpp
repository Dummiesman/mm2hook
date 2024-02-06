#include "carsim.h"
#include "carmodel.h"

using namespace MM2;

float vehCarSim::GetBoundFriction() const {
    return this->BoundFriction;
}

void vehCarSim::SetBoundFriction(float friction) {
    this->BoundFriction = friction;
    this->RestoreImpactParams(); //apply
}

float vehCarSim::GetBoundElasticity() const {
    return this->BoundElasticity;
}

void vehCarSim::SetBoundElasticity(float elasticity) {
    this->BoundElasticity = elasticity;
    this->RestoreImpactParams(); //apply
}

Vector3 vehCarSim::GetInertiaBox() const {
    return this->InertiaBox;
}

void vehCarSim::SetInertiaBox(Vector3 const & inertiaBox) {
    this->InertiaBox = inertiaBox;
}

void vehCarSim::SetAndApplyInertiaBox(Vector3 const & inertiaBox) {
    this->SetInertiaBox(inertiaBox);
    this->GetICS()->InitBoxMass(this->GetMass(), inertiaBox.X, inertiaBox.Y, inertiaBox.Z);
}

Vector3 vehCarSim::GetCenterOfGravity() const
{
    return this->CenterOfGravity;
}

Vector3 vehCarSim::GetSize() const
{
    return this->Size;
}
        
void vehCarSim::SetSize(Vector3 const& size) {
    this->Size = size;
}

phInertialCS * vehCarSim::GetICS() {
    return &this->InertialCS;
}

Matrix34 * vehCarSim::GetWorldMatrix(void) {
    return &this->WorldMatrix;
}

float vehCarSim::GetMass() const {
    return this->Mass;
}

void vehCarSim::SetMass(float mass) {
    this->Mass = mass;
}

void vehCarSim::SetAndApplyMass(float mass) {
    this->SetMass(mass);
            
    auto box = this->GetInertiaBox();
    this->GetICS()->InitBoxMass(mass, box.X, box.Y, box.Z);
}

int vehCarSim::GetDrivetrainType() const {
    return this->DrivetrainType;
}

void vehCarSim::SetDrivetrainType(int type) {
    if (type < 0 || type > 2) //0 = RWD, 1 = FWD, 2 = 4WD
        return;
    this->DrivetrainType = type;
}

float vehCarSim::GetThrottle() const {
    return this->Engine.GetThrottleInput();
}

void vehCarSim::SetThrottle(float throttle) {
    this->Engine.SetThrottleInput(throttle);
}

float vehCarSim::GetSteering() const {
    return this->SteeringInput;
}

void vehCarSim::SetSteering(float steering) {
    this->SteeringInput = steering;
}

float vehCarSim::GetBrake() const {
    return this->BrakeInput;
}

void vehCarSim::SetBrake(float brake) {
    this->BrakeInput = brake;
}

float vehCarSim::GetHandbrake() const {
    return this->HandBrakeInput;
}

void vehCarSim::SetHandbrake(float handBrake) {
    this->HandBrakeInput = handBrake;
}

float vehCarSim::GetSpeed() const {
    return this->Speed;
}

float vehCarSim::GetSpeedMPH() const {
    return this->SpeedInMph;
};

Vector3 vehCarSim::GetResetPos() const {
    return this->ResetPosition - this->CenterOfGravity;
}

float vehCarSim::GetResetRotation() const {
    return this->ResetRotation;
}

void vehCarSim::SetResetRotation(float rotation) {
    this->ResetRotation = rotation;
}

lvlInstance * vehCarSim::GetInstance() {
    return this->LvlInstancePtr;
}

vehTransmission * vehCarSim::GetTransmission() {
    return &this->Transmission;
}

vehEngine * vehCarSim::GetEngine() {
    return &this->Engine;
}

vehDrivetrain * vehCarSim::GetDrivetrain() {
    return &this->PrimaryDrivetrain;
}

vehDrivetrain * vehCarSim::GetLeftFreetrain() {
    return &this->FreetrainLeft;
}

vehDrivetrain * vehCarSim::GetRightFreetrain() {
    return &this->FreetrainRight;
}

vehWheel * vehCarSim::GetWheel(int num) {
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

vehAero * vehCarSim::GetAero() {
    return &this->Aero;
}

vehAxle * vehCarSim::GetFrontAxle() {
    return &this->AxleFront;
}

vehAxle * vehCarSim::GetRearAxle() {
    return &this->AxleRear;
}

AGE_API vehCarSim::vehCarSim()                                 { hook::Thunk<0x4CB660>::Call<void>(this); }
AGE_API vehCarSim::~vehCarSim()                                { hook::Thunk<0x4CB8E0>::Call<void>(this); }

AGE_API void vehCarSim::Init(const char* basename, phColliderJointed* colliderPtr, vehCarModel* carModelPtr)
{
    // Call original 
    hook::Thunk<0x4CBB80>::Call<void>(this, basename, colliderPtr, carModelPtr);

    // Setup size from carmodel if it has a bound
    auto bound = carModelPtr->GetBound(0);
    if (bound)
    {
        SetSize(bound->GetSize());
    }

    // We've expanded this class. Now for *our new stuff*
    Matrix34 diffMatrix;
            
    if (GetPivot(diffMatrix, basename, "whl4")) {
        BackBackLeftWheelPosDiff = diffMatrix.GetRow(3) - GetWheel(2)->GetCenter();
    }

    if (GetPivot(diffMatrix, basename, "whl5")) {
        BackBackRightWheelPosDiff = diffMatrix.GetRow(3) - GetWheel(3)->GetCenter();
    }
}

AGE_API int vehCarSim::BottomedOut()                           { return hook::Thunk<0x4CBB40>::Call<int>(this); }
AGE_API int vehCarSim::OnGround()                              { return hook::Thunk<0x4CBB00>::Call<int>(this); }
AGE_API void vehCarSim::ReconfigureDrivetrain()                { hook::Thunk<0x4CC0B0>::Call<void>(this); }
AGE_API void vehCarSim::SetHackedImpactParams()                { hook::Thunk<0x4CC080>::Call<void>(this); }
AGE_API void vehCarSim::RestoreImpactParams()                  { hook::Thunk<0x4CC050>::Call<void>(this); }
AGE_API void vehCarSim::SetResetPos(Vector3 const & a1)        { hook::Thunk<0x4CC830>::Call<void>(this, &a1); }
        
/*
    asNode virtuals
*/

AGE_API void vehCarSim::Update()                      { hook::Thunk<0x4CC8E0>::Call<void>(this); }
AGE_API void vehCarSim::Reset()                       { hook::Thunk<0x4CBA70>::Call<void>(this); }
AGE_API void vehCarSim::FileIO(datParser &parser)     { hook::Thunk<0x4CCC70>::Call<void>(this, &parser); }
AGE_API char* vehCarSim::GetClassName()               { return hook::Thunk<0x4CCF10>::Call<char*>(this); }
AGE_API char const* vehCarSim::GetDirName()           { return hook::Thunk<0x4CBAF0>::Call<char const*>(this); }

void vehCarSim::BindLua(LuaState L) {
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
        .addPropertyReadOnly("Size", &GetSize)
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