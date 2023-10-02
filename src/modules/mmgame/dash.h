#pragma once
#include <modules\node.h>
#include "gauge.h"

namespace MM2
{
    // Forward declarations
    class mmDashView;

    // External declarations
    extern class mmPlayer;

    // Class definitions
    class mmDashView : public asLinearCS {
    private:
        mmPlayer* m_PlayerPtr;
        int dword_84;
        int dword_88;
        int dword_8c;
        RadialGauge RPMGauge;
        RadialGauge SpeedGauge;
        RadialGauge DamageGauge;
        int dword_3cc;
        int dword_3d0;
        int dword_3d4;
        int dword_3d8;
        int dword_3dc;
        int dword_3e0;
        int dword_3e4;
        int dword_3e8;
        int dword_3ec;
        int dword_3f0;
        float dword_3f4;
        int dword_3f8;
        float dword_3fc;
        float WheelFact;
        int dword_404;
        asLinearCS LinearCS_A;
        asLinearCS LinearCS_B;
        asLinearCS LinearCS_C;
        int dword_588;
        int dword_58c;
        int dword_590;
        Vector3 DashPos;
        Vector3 RoofPos;
        int dword_5ac;
        int dword_5b0;
        int dword_5b4;
        int dword_5b8;
        int dword_5bc;
        int dword_5c0;
        int dword_5c4;
        int dword_5c8;
        int dword_5cc;
        float MaxSpeed;
        float MaxRPM;
        int MinSpeed;
        int dword_5dc;
        int DamageNeedleModStatic;
        int DashModStatic;
        int DashExtraModStatic;
        int GearIndicatorModStatic;
        int RoofModStatic;
        int SpeedNeedleModStatic;
        int TachNeedleModStatic;
        int WheelModStatic;
        int ShaderSet;
        bool m_IsLoaded;
        byte byte_605;
        byte byte_606;
        byte byte_607;
        Vector3 GearIndicatorPivot;
        Vector3 DamageNeedlePivot;
        Vector3 SpeedNeedlePivot;
        Vector3 TachNeedlePivot;
        Vector3 DmgOffset;
        Vector3 SpeedOffset;
        Vector3 TachOffset;
        Vector3 WheelPos;
        Vector3 DmgPivotOffset;
        Vector3 SpeedPivotOffset;
        Vector3 TachPivotOffset;
        Vector3 WheelPivotOffset;
        Vector3 GearPivotOffset;
        int dword_6a4;
        int dword_6a8;
        int dword_6ac;
        int dword_6b0;
        int dword_6b4;
        int dword_6b8;
        int dword_6bc;
        int dword_6c0;
        int dword_6c4;
        int dword_6c8;
        int dword_6cc;
        int dword_6d0;
        BOOL PivotDebug;
        int dword_6d8;
    public:
        bool IsLoaded(void) const {
            return this->m_IsLoaded;
        }

        mmPlayer * GetPlayer(void) const {
            return this->m_PlayerPtr;
        };

        RadialGauge * GetRPMGauge() {
            return &this->RPMGauge;
        }

        RadialGauge* GetSpeedGauge() {
            return &this->SpeedGauge;
        }

        RadialGauge* GetDamageGauge() {
            return &this->DamageGauge;
        }

        AGE_API void Activate()                     { hook::Thunk<0x430E80>::Call<void>(this); }
        AGE_API void Deactivate()                   { hook::Thunk<0x430EA0>::Call<void>(this); }

        /*
            asNode virtuals
        */

        AGE_API void Reset() override               { hook::Thunk<0x430D90>::Call<void>(this); }
        AGE_API void Update() override              { hook::Thunk<0x430ED0>::Call<void>(this); }
        AGE_API void Cull() override                { hook::Thunk<0x430FB0>::Call<void>(this); }
        AGE_API void FileIO(datParser &parser) override
                                                    { hook::Thunk<0x4315D0>::Call<void>(this, &parser); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmDashView, asNode>("mmDashView")
                .addFunction("Activate", &Activate)
                .addFunction("Deactivate", &Deactivate)
                .addPropertyReadOnly("IsLoaded", &IsLoaded)
                .addPropertyReadOnly("RPMGauge", &GetRPMGauge)
                .addPropertyReadOnly("DamageGauge", &GetDamageGauge)
                .addPropertyReadOnly("SpeedGauge", &GetSpeedGauge)
                .addVariable("DashPos", &mmDashView::DashPos)
                .addVariable("RoofPos", &mmDashView::RoofPos)
                .addVariable("WheelPos", &mmDashView::WheelPos)
                .addVariable("DmgOffset", &mmDashView::DmgOffset)
                .addVariable("SpeedOffset", &mmDashView::SpeedOffset)
                .addVariable("TachOffset", &mmDashView::TachOffset)
                .addVariable("DmgPivotOffset", &mmDashView::DmgPivotOffset)
                .addVariable("SpeedPivotOffset", &mmDashView::SpeedPivotOffset)
                .addVariable("TachPivotOffset", &mmDashView::TachPivotOffset)
                .addVariable("WheelPivotOffset", &mmDashView::WheelPivotOffset)
                .addVariable("GearPivotOffset", &mmDashView::GearPivotOffset)
                .addVariable("WheelFact", &mmDashView::WheelFact)
                .addVariable("MaxSpeed", &mmDashView::MaxSpeed)
                .addVariable("MaxRPM", &mmDashView::MaxRPM)
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmDashView, 0x6DC);
}