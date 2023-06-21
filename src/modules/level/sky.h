#pragma once
#include <modules\node\cullable.h>

namespace MM2
{
    // Forward declarations
    class lvlSky;

    // External declarations
    extern class modStatic;
    extern class modShader;

    // Class definitions
    class lvlSky : public asCullable
    {
    private:
        modStatic* Model;
        modShader** Shaders;
        float HatYOffset;
        float YMultiplier;
        float RotationRate;
        float CurrentRotation;
        int TimeWeatherType;
        int ShaderCount;
        bool DrawEnabled;
        ColorARGB FogColors[16];
        short FogNearClip[16];
        short FogFarClip[16];
    public:
        int GetShaderSet() const
        {
            return TimeWeatherType;
        }

        Vector4 GetFogColor(int index) const 
        {
            if (index < 0 || index >= 16)
                return Vector4(0.0f, 0.0f, 0.0f, 0.0f);

            auto color = FogColors[index];
            return Vector4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
        }

        int GetFogNearClip(int index) {
            return (index < 0 || index >= 16) ? 0 : this->FogNearClip[index];
        }

        int GetFogFarClip(int index) {
            return (index < 0 || index >= 16) ? 0 : this->FogFarClip[index];
        }

        float GetRotation() const {
            return this->CurrentRotation;
        }

        void setFogNearClip(int index, short clip) {
            if (index < 0 || index >= 16)
                return;
            this->FogNearClip[index] = clip;
        }

        void setFogFarClip(int index, short clip) {
            if (index < 0 || index >= 16)
                return;
            this->FogFarClip[index] = clip;
        }

        void setFogColor(int index, byte r, byte g, byte b, byte a) {
            if (index < 0 || index >= 16)
                return;

            auto myColor = &this->FogColors[index];
            myColor->a = a;
            myColor->r = r;
            myColor->g = g;
            myColor->b = b;
        }

        void SetRotation(float rotation)
        {
            this->CurrentRotation = rotation;
        }
    public:

        /*
            lvlSky Virtuals
        */
        AGE_API virtual void Reset()                        { hook::Thunk<0x464C90>::Call<void>(this); }
        AGE_API virtual void Update()                       { hook::Thunk<0x464F90>::Call<void>(this); }
        AGE_API virtual void Draw()                         { hook::Thunk<0x464FB0>::Call<void>(this); }
        AGE_API virtual void DrawWithNoZState()             { hook::Thunk<0x4650B0>::Call<void>(this); }
        AGE_API virtual void DrawHat()                      { hook::Thunk<0x465140>::Call<void>(this); }
        

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<lvlSky, asCullable>("lvlSky")
                .addVariable("TimeWeatherType", &lvlSky::TimeWeatherType)
                .addVariable("DrawEnabled", &lvlSky::DrawEnabled)
                .addVariable("CurrentRotation", &lvlSky::CurrentRotation)
                .addVariable("RotationSpeed", &lvlSky::RotationRate)
                .addVariable("RotationRate", &lvlSky::RotationRate)
                .addVariable("YMultiplier", &lvlSky::YMultiplier)
                .addVariable("HatYOffset", &lvlSky::HatYOffset)
                .addFunction("GetFogColor", &GetFogColor)
                .addFunction("GetFogNearClip", &GetFogNearClip)
                .addFunction("GetFogFarClip", &GetFogFarClip)
                .addFunction("SetFogColor", &setFogColor)
                .addFunction("SetFogNearClip", &setFogNearClip)
                .addFunction("SetFogFarClip", &setFogFarClip)
                .endClass();
        }
    };

    ASSERT_SIZEOF(lvlSky, 0xA8);

    // Lua initialization

}