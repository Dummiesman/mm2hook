#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class RadialGauge;

    // External declarations
    extern class mmExternalView;
    extern class mmPlayer;
    extern class vehCarSim;
    
    // Class definitions
    class mmSpeedIndicator
    {
    private:
        gfxBitmap* numberBitmaps[10];
        int XPos;
        int YPos;
        mmExternalView* View;
        vehCarSim* CarSim;
    public:
        AGE_API void Init(mmExternalView* view, vehCarSim* car)     { hook::Thunk<0x43F280>::Call<void>(this, view, car); }
        AGE_API void Draw()                                         { hook::Thunk<0x43F330>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmSpeedIndicator>("mmSpeedIndicator")
                .addFunction("Init", &Init)
                .addFunction("Draw", &Draw)
            .endClass();
        }
    };


    class mmGearIndicator
    {
    private:
        gfxBitmap* gearImages[12];
        int XPos;
        int yPos;
        mmExternalView* View;
        mmPlayer* Player;
    public:
        AGE_API void Init(mmExternalView* view, mmPlayer* player)   { hook::Thunk<0x43F0C0>::Call<void>(this, view, player); }
        AGE_API void Draw()                                         { hook::Thunk<0x43F1F0>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmGearIndicator>("mmGearIndicator")
                .addFunction("Init", &Init)
                .addFunction("Draw", &Draw)
            .endClass();
        }
    };


    class mmLinearGauge
    {
    private:
        float* ValuePtr;
        float* MaxValuePtr;
        int XPos;
        int YPos;
        gfxBitmap* GaugeBitmap;
        gfxBitmap* OverlayBitmap;
        int FillDirection;
        mmExternalView* View;
    public:
        AGE_API void InitOverlay(const char* bitmap)            { hook::Thunk<0x43EED0>::Call<void>(this, bitmap); }
        AGE_API void Init(const char* bitmap, float* valuePtr, float* maxValuePtr, int fillDirection, mmExternalView* view)
                                                                { hook::Thunk<0x43EE70>::Call<void>(this, bitmap, valuePtr, maxValuePtr, fillDirection, view); }
        AGE_API void Draw()                                     { hook::Thunk<0x43EEF0>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmLinearGauge>("mmLinearGauge")
                .addFunction("InitOverlay", &InitOverlay)
                .addFunction("Init", &Init)
                .addFunction("Draw", &Draw)
            .endClass();
        }
    };

    class mmSlidingGauge : public mmLinearGauge
    {
    private:
        int ViewportSize;
    public:
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmSlidingGauge, mmLinearGauge>("mmSlidingGauge")
            .endClass();
        }
    };


    class RadialGauge : public asNode
    {
    public:
        asLinearCS LinearCS;
        float* ValuePtr;
        float* MaxValuePtr;
        float RotMin;
        float RotMax;
        float* MinValuePtr;
        Vector3 dword_ac;
        int ShaderSet;
        int ModStatic;
        Vector3 Pivot;
        Vector3 Offset;
        Vector3 PivotOffset;
        Matrix34 dword_e4;
    public:
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<RadialGauge, asNode>("RadialGauge")
                .addVariable("RotMin", &RadialGauge::RotMin)
                .addVariable("RotMax", &RadialGauge::RotMax)
                .addVariable("Pivot", &RadialGauge::Pivot)
                .addVariable("Offset", &RadialGauge::Offset)
                .addVariable("PivotOffset", &RadialGauge::PivotOffset)
                .endClass();
        }
    };
}