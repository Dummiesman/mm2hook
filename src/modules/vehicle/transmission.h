#pragma once
#include <modules\vehicle.h>

namespace MM2
{
    // Forward declarations
    class vehTransmission;

    // External declarations
    extern class vehCarSim;

    // Class definitions

    class vehTransmission : public asNode {
    private:
        vehCarSim *VehCarSimPtr;
        int GearChanged;
        BOOL IsAutomatic;
        int CurrentGear;
        float GearChangeTimer;
        float GearChangeTime;
        float ManualRatios[8];
        int ManualNumGears;
        int AutoNumGears;
        float AutoRatios[8];
        float UpshiftRpms[8];
        float DownshiftMinRpms[8];
        float DownshiftMaxRpms[8];
        float ReverseSpeed;
        float FirstGearSpeed;
        float LastGearSpeed;
        float UpshiftBias;
        float DownshiftBiasMin;
        float DownshiftBiasMax;
        float GearBias;
    public:
        AGE_API void Upshift()                              { hook::Thunk<0x4CF570>::Call<void>(this); }
        AGE_API void Downshift()                            { hook::Thunk<0x4CF5B0>::Call<void>(this); }
        AGE_API void SetReverse()                           { hook::Thunk<0x4CF6C0>::Call<void>(this); }
        AGE_API void SetNeutral()                           { hook::Thunk<0x4CF6D0>::Call<void>(this); }
        AGE_API void SetForward()                           { hook::Thunk<0x4CF6E0>::Call<void>(this); }
        AGE_API void SetCurrentGear(int a1)                 { hook::Thunk<0x4CF700>::Call<void>(this, a1); }
        AGE_API void Automatic(BOOL a1)                     { hook::Thunk<0x4CF6B0>::Call<void>(this, a1); }
        AGE_API float GearRatioFromMPH(float a1)            { return hook::Thunk<0x4CF530>::Call<float>(this, a1); }
        AGE_API void ComputeConstants()                     { hook::Thunk<0x4CF220>::Call<void>(this); }

        /*
            asNode virtuals
        */

        AGE_API void Update() override                      { hook::Thunk<0x4CF600>::Call<void>(this); }
        AGE_API void Reset() override                       { hook::Thunk<0x4CF200>::Call<void>(this); }
        AGE_API void FileIO(datParser &parser)  override
                                                            { hook::Thunk<0x4CF740>::Call<void>(this); }
        AGE_API char* GetClassName() override               { return hook::Thunk<0x4CF880>::Call<char*>(this); }

        
        int GetGear() const {
            return this->CurrentGear;
        };

        bool IsAuto() const {
            return this->IsAutomatic == TRUE;
        };

        float GetAutoGearRatio(int gear) const {
            return AutoRatios[CLAMPINT(gear, 0, 7)];
        }

        float GetManualGearRatio(int gear) const {
            return ManualRatios[CLAMPINT(gear, 0, 7)];
        }

        float GetUpshiftRpm(int gear) const {
            return UpshiftRpms[CLAMPINT(gear, 0, 7)];
        }

        float GetDownshiftMinRpm(int gear) const {
            return DownshiftMinRpms[CLAMPINT(gear, 0, 7)];
        }

        float GetDownshiftMaxRpm(int gear) const {
            return DownshiftMaxRpms[CLAMPINT(gear, 0, 7)];
        }

        void SetDownshiftMinRpm(int gear, float rpm)
        {
            if (gear < 0 || gear > 7)
                return;
            DownshiftMinRpms[gear] = rpm;
        }

        void SetDownshiftMaxRpm(int gear, float rpm)
        {
            if (gear < 0 || gear > 7)
                return;
            DownshiftMaxRpms[gear] = rpm;
        }

        void SetUpshiftRpm(int gear, float rpm)
        {
            if (gear < 0 || gear > 7)
                return;
            UpshiftRpms[gear] = rpm;
        }

        void SetAutoGearRatio(int gear, float ratio)
        {
            if (gear < 0 || gear > 7)
                return;
            AutoRatios[gear] = ratio;
        }

        void SetManualGearRatio(int gear, float ratio)
        {
            if (gear < 0 || gear > 7)
                return;
            ManualRatios[gear] = ratio;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehTransmission, asNode>("vehTransmission")
                //properties / lua functions
                .addFunction("SetAutoGearRatio", &SetAutoGearRatio)
                .addFunction("GetAutoGearRatio", &GetAutoGearRatio)
                .addFunction("SetManualGearRatio", &SetManualGearRatio)
                .addFunction("GetManualGearRatio", &GetManualGearRatio)
                .addFunction("GetUpshiftRpm", &GetUpshiftRpm)
                .addFunction("SetUpshiftRpm", &SetUpshiftRpm)
                .addFunction("GetDownshiftMinRpm", &GetDownshiftMinRpm)
                .addFunction("GetDownshiftMaxRpm", &GetDownshiftMaxRpm)
                .addFunction("SetDownshiftMinRpm", &SetDownshiftMinRpm)
                .addFunction("SetDownshiftMaxRpm", &SetDownshiftMaxRpm)
                .addVariable("GearChangeTime", &vehTransmission::GearChangeTime)
                .addVariable("AutoNumGears", &vehTransmission::AutoNumGears)
                .addVariable("ManualNumGears", &vehTransmission::ManualNumGears)
                .addVariable("Low", &vehTransmission::FirstGearSpeed)
                .addVariable("High", &vehTransmission::LastGearSpeed)
                .addVariable("Reverse", &vehTransmission::ReverseSpeed)
                .addVariable("GearBias", &vehTransmission::GearBias)
                .addVariable("DownshiftBiasMax", &vehTransmission::DownshiftBiasMax)
                .addVariable("DownshiftBiasMin", &vehTransmission::DownshiftBiasMin)
                .addVariable("UpshiftBias", &vehTransmission::UpshiftBias)
                .addPropertyReadOnly("IsAutomatic", &IsAuto)
                .addProperty("Gear", &GetGear, &SetCurrentGear)

                //functions
                .addFunction("Automatic", &Automatic, LUA_ARGS(bool))
                .addFunction("Downshift", &Downshift)
                .addFunction("Upshift", &Upshift)
                .addFunction("SetReverse", &SetReverse)
                .addFunction("SetNeutral", &SetNeutral)
                .addFunction("SetForward", &SetForward)
                .addFunction("SetCurrentGear", &SetCurrentGear)
                .addFunction("GearRatioFromMPH", &GearRatioFromMPH)
                .addFunction("ComputeConstants", &ComputeConstants)
            .endClass();
        }
    };

    ASSERT_SIZEOF(vehTransmission, 0xF4);

    // Lua initialization

}