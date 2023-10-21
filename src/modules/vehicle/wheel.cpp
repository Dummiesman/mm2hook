#pragma once
#include "wheel.h"

namespace MM2
{
    AGE_API vehWheel::vehWheel() { hook::Thunk<0x4D2190>::Call<void>(this); }

    declfield(vehWheel::WeatherFriction)(0x5CF6B8);

    std::tuple<float, float> vehWheel::luaComputeFriction(float slip) const {
        float visualFriction = 0.f;
        float functionalFriction = this->ComputeFriction(slip, &visualFriction);
        return std::make_tuple(functionalFriction, visualFriction);
    }

    AGE_API void vehWheel::Init(vehCarSim* carSimPtr, const char* vehicleBasename, const char* wheelName, Vector3 centerOfGravity, phInertialCS* inertialCs, int a6, int a7)
    {
        Matrix34 outMatrix;

        this->m_CarSimPtr = carSimPtr;
        this->WheelFlags |= a7;
        *getPtr<int>(this, 0x94) = a6;
        this->m_InertialCSPtr = inertialCs;

        if (GetPivot(outMatrix, vehicleBasename, wheelName)) {
            this->Center.X = outMatrix.m30;
            this->Center.Y = outMatrix.m31;
            this->Center.Z = outMatrix.m32;

            float halfHeight = (outMatrix.m11 - outMatrix.m01) * 0.5f;
            this->Radius = fabs(halfHeight);

            this->Width = outMatrix.m10 - outMatrix.m00;
        }

        this->ComputeConstants();
    }

    AGE_API void vehWheel::CopyVars(vehWheel *copyFrom)              { hook::Thunk<0x4D4110>::Call<void>(this, copyFrom); }

    AGE_API void vehWheel::ComputeConstants()                        { hook::Thunk<0x4D23F0>::Call<void>(this); }
    AGE_API void vehWheel::AddNormalLoad(float a1)                   { hook::Thunk<0x4D2490>::Call<void>(this, a1); }
    AGE_API void vehWheel::SetNormalLoad(float a1)                   { hook::Thunk<0x4D24C0>::Call<void>(this, a1); }
    AGE_API void vehWheel::SetInputs(float a1, float a2, float a3)
                                                            { hook::Thunk<0x4D3F80>::Call<void>(this, a1, a2, a3); }
    AGE_API int vehWheel::GetSurfaceSound()                          { return hook::Thunk<0x4D3F60>::Call<int>(this); }
    AGE_API float vehWheel::GetVisualDispVert()                      { return hook::Thunk<0x4D4030>::Call<float>(this); }
    AGE_API float vehWheel::GetVisualDispLat()                       { return hook::Thunk<0x4D4090>::Call<float>(this); }
    AGE_API float vehWheel::GetVisualDispLong()                      { return hook::Thunk<0x4D40D0>::Call<float>(this); }

    AGE_API float vehWheel::ComputeFriction(float slip, float *vF) const
                                                                     { return hook::Thunk<0x4D25D0>::Call<float>(this, slip, vF); }

    /*
        asNode virtuals
    */

    AGE_API void vehWheel::Reset()                          { hook::Thunk<0x4D22E0>::Call<void>(this); }
    AGE_API void vehWheel::Update()                         { hook::Thunk<0x4D34E0>::Call<void>(this); }
    AGE_API void vehWheel::FileIO(datParser &parser)        { hook::Thunk<0x4D41C0>::Call<void>(this); }
    AGE_API char * vehWheel::GetClassName()                 { return hook::Thunk<0x4D43C0>::Call<char *>(this); }


    /*
        vehWheel
    */

    Matrix34 vehWheel::GetMatrix() const {
        return this->WheelMatrix;
    }

    bool vehWheel::IsGrounded() const {
        return this->m_IsGrounded == TRUE;
    }

    inline lvlMaterial * vehWheel::GetCurrentPhysicsMaterial() {
        return this->CurrentPhysicsMaterial;
    }

    inline float vehWheel::GetRadius() const { return this->Radius; }
    void vehWheel::SetRadius(float radius) { this->Radius = radius; }
    
    float vehWheel::GetWidth() const { return this->Width; }
    void vehWheel::SetWidth(float width) { this->Width = width; }

    Vector3 vehWheel::GetCenter() const { return this->Center; }
    void vehWheel::SetCenter(Vector3 const & center) { this->Center = center; }
    
    float vehWheel::GetLatSlipPercent() const {
        return this->LatSlipPercent;
    }

    float vehWheel::GetLongSlipPercent() const {
        return this->LongSlipPercent;
    }

    float vehWheel::GetRotationRate() const {
        return this->RotationRate;
    }

    float vehWheel::GetWeatherFriction() { return WeatherFriction.get(); }
    void vehWheel::SetWeatherFriction(float friction) { WeatherFriction.set(friction); }

    void vehWheel::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<vehWheel, asNode>("vehWheel")
            //properties
            .addStaticProperty("WeatherFriction", &GetWeatherFriction, &SetWeatherFriction)

            .addVariable("SuspensionExtent", &vehWheel::SuspensionExtent)
            .addVariable("SuspensionLimit", &vehWheel::SuspensionLimit)
            .addVariable("SuspensionFactor", &vehWheel::SuspensionFactor)
            .addVariable("SuspensionDampCoef", &vehWheel::SuspensionDampCoef)
            .addVariable("SteeringLimit", &vehWheel::SteeringLimit)
            .addVariable("SteeringOffset", &vehWheel::SteeringOffset)
            .addVariable("BrakeCoef", &vehWheel::BrakeCoef)
            .addVariable("HandbrakeCoef", &vehWheel::HandbrakeCoef)
            .addVariable("CamberLimit", &vehWheel::CamberLimit)
            .addVariable("WobbleLimit", &vehWheel::WobbleLimit)
            .addVariable("TireDispLimitLong", &vehWheel::TireDispLimitLong)
            .addVariable("TireDampCoefLong", &vehWheel::TireDampCoefLong)
            .addVariable("TireDragCoefLong", &vehWheel::TireDragCoefLong)
            .addVariable("TireDispLimitLat", &vehWheel::TireDispLimitLat)
            .addVariable("TireDampCoefLat", &vehWheel::TireDampCoefLat)
            .addVariable("TireDragCoefLat", &vehWheel::TireDragCoefLat)
            .addVariable("OptimumSlipPercent", &vehWheel::OptimumSlipPercent)
            .addVariable("StaticFric", &vehWheel::StaticFric)
            .addVariable("SlidingFric", &vehWheel::SlidingFric)

            .addProperty("Radius", &GetRadius, &SetRadius)
            .addProperty("Width", &GetWidth, &SetWidth)
                
            .addPropertyReadOnly("IsGrounded", &IsGrounded)
            .addPropertyReadOnly("CurrentPhysicsMaterial", &GetCurrentPhysicsMaterial)
            .addPropertyReadOnly("LatSlipPercent", &GetLatSlipPercent)
            .addPropertyReadOnly("LongSlipPercent", &GetLongSlipPercent)
            .addPropertyReadOnly("RotationRate", &GetRotationRate)

            //functions
            .addFunction("CopyVars", &CopyVars)
            .addFunction("ComputeConstants", &ComputeConstants)
            .addFunction("AddNormalLoad", &AddNormalLoad)
            .addFunction("SetNormalLoad", &SetNormalLoad)
            .addFunction("SetInputs", &SetInputs)
            .addFunction("GetSurfaceSound", &GetSurfaceSound)
            .addFunction("GetVisualDispVert", &GetVisualDispVert)
            .addFunction("GetVisualDispLat", &GetVisualDispLat)
            .addFunction("GetVisualDispLong", &GetVisualDispLong)
            .addFunction("ComputeFriction", &luaComputeFriction)
        .endClass();
    }
}