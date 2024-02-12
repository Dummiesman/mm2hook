#pragma once
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class vehEngine;

    // External declarations
    extern class vehCarSim;
    extern class vehTransmission;

    // Class definitions
    class vehEngine : public asNode {
    private:
        float MaxHorsePower;
        float IdleRPM;
        float OptRPM;
        float MaxRPM;
        float GCL;
        float HPScale;
        float AIThrottle; // used only by AI as some kind of saved "target throttle" value
        float AngInertia;
        float MaxTorque;
        float OptTorque;
        float IdleTorque;
        float TorqueGap;
        float unknown676;
        float sq5add1;
        float sq5min1;
        float GearChangedAtRPM;
        float GCLTimer;
        BOOL WaitingOnGCL;
        float ThrottleInput;
        float CurrentTorque;
        float CurrentRPM;
        float unknown712;
        float ThrottleTorque;
        Matrix34 *EngineVisualMatrixRef;
        Matrix34 *EngineVisualMatrixPtr;
        vehCarSim *VehCarSimPtr;
        vehTransmission *VehTransmissionPtr;
    public:

        AGE_API void ComputeConstants()                     { hook::Thunk<0x4D8DC0>::Call<void>(this); }
        AGE_API float CalcTorqueAtFullThrottle(float av)    { return hook::Thunk<0x4D8E20>::Call<float>(this, av); }
        AGE_API float CalcTorqueAtZeroThrottle(float av)    { return hook::Thunk<0x4D8EA3>::Call<float>(this, av); }
        AGE_API float CalcTorque(float throttle)            { return hook::Thunk<0x4D8ED0>::Call<float>(this, throttle); }
        AGE_API float CalcHPAtFullThrottle(float av)        { return hook::Thunk<0x4D8F10>::Call<float>(this, av); }

        /*
            asNode virtuals
        */

        AGE_API void Update() override                      { hook::Thunk<0x4D8F30>::Call<void>(this); }
        AGE_API void Reset() override                       { hook::Thunk<0x4D8CE0>::Call<void>(this); }
        AGE_API void FileIO(datParser &parser)  override
                                                            { hook::Thunk<0x4D9240>::Call<void>(this); }
        AGE_API char* GetClassName() override               { return hook::Thunk<0x4D9310>::Call<char*>(this); }

        //lua helpers
        bool GetWaitingOnGCL() const
        {
            return this->WaitingOnGCL == TRUE;
        }

        float GetThrottleInput() const { return this->ThrottleInput; }
        void SetThrottleInput(float input) { this->ThrottleInput = input; }

        float GetCurrentRPM() const { return this->CurrentRPM; }
        void SetCurrentRPM(float rpm) { this->CurrentRPM = rpm; }

        float GetCurrentTorque() const { return this->CurrentTorque; }
        void SetCurrentTorque(float torque) { this->CurrentTorque = torque; }

        float GetMaxHorsePower() const { return this->MaxHorsePower;}
        void SetMaxHorsePower(float power)  { this->MaxHorsePower = power; }

        void SetThrottleTorque(float torque) 
        {
            this->ThrottleTorque = torque;
        }

        float GetIdleRPM() const { return this->IdleRPM; }
        void SetIdleRPM(float rpm) { this->IdleRPM = rpm; }

        float GetOptRPM() const {return this->OptRPM; }
        void SetOptRPM(float rpm) { this->OptRPM = rpm; }

        float GetMaxRPM() const { return this->MaxRPM; }
        void SetMaxRPM(float rpm) { this->MaxRPM = rpm; }

        float GetHPScale() const { return this->HPScale; }
        void SetHPScale(float scale)
        {
            this->HPScale = scale;
            ComputeConstants();
        }

        float GetGCL() const { return this->GCL; }
        void SetGCL(float gcl) { this->GCL = gcl; }

        float GetAngInertia() const { return this->AngInertia; }
        void SetAngInertia(float inertia) { this->AngInertia = inertia; }

        Matrix34* GetVisualMatrixPtr()
        {
            return this->EngineVisualMatrixPtr;
        }

        vehCarSim* GetCarSim()
        {
            return this->VehCarSimPtr;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehEngine, asNode>("vehEngine")
                //properties
                .addProperty("MaxHorsePower", &GetMaxHorsePower, &SetMaxHorsePower)
                .addProperty("IdleRPM", &GetIdleRPM, &SetIdleRPM)
                .addProperty("OptRPM", &GetOptRPM, &SetOptRPM)
                .addProperty("MaxRPM", &GetMaxRPM, &SetMaxRPM)
                .addProperty("HPScale", &GetHPScale, &SetHPScale)
                .addProperty("GCL", &GetGCL, &SetGCL)
                .addProperty("AngInertia", &GetAngInertia, &SetAngInertia)
                .addPropertyReadOnly("WaitingOnGCL", &GetWaitingOnGCL)
                .addProperty("Throttle", &GetThrottleInput, &SetThrottleInput)
                .addProperty("RPM", &GetCurrentRPM, &SetCurrentRPM)

                //functions
                .addFunction("ComputeConstants", &ComputeConstants)
                .addFunction("CalcTorqueAtFullThrottle", &CalcTorqueAtFullThrottle)
                .addFunction("CalcTorqueAtZeroThrottle", &CalcTorqueAtZeroThrottle)
                .addFunction("CalcTorque", &CalcTorque)
                .addFunction("CalcHPAtFullThrottle", &CalcHPAtFullThrottle)

                .endClass();
        }
    };
    ASSERT_SIZEOF(vehEngine, 0x84);

    // Lua initialization

}