#pragma once

namespace MM2
{
    // Forward declarations
    class ltLight;

    // External declarations
    extern class gfxTexture;
    extern struct gfxLight;

    // Class definitions
    class ltLight {
    public:
        int Type;
        Vector3 Position;
        Vector3 Direction;
        Vector3 Color;
        float Intensity;
        int unk_2C;
        float SpotExponent;
        gfxTexture* HighlightTexture;
        int unk_38;
        float ProjectionSize;
        bool EnableProjection;
        byte unk_41;
        byte unk_42;
        byte unk_43;
        int ShadowMode;
        int unk_48;

        ANGEL_ALLOCATOR 

        AGE_API ltLight() { hook::Thunk<0x59AB80>::Call<void>(this); }
        AGE_API ~ltLight() { hook::Thunk<0x59ABC0>::Call<void>(this); }

        //static funcs
        static AGE_API void ShutdownLights() { hook::StaticThunk<0x59ABD0>::Call<void>(); }
        static AGE_API void DrawGlowBegin() { hook::StaticThunk<0x59AE30>::Call<void>(); }
        static AGE_API void DrawGlowEnd() { hook::StaticThunk<0x59AEF0>::Call<void>(); }
        static AGE_API void SetUpGfxLightBegin(Vector3 const* a1)
        {
            hook::StaticThunk<0x59B390>::Call<void>(a1);
        }
        static AGE_API void SetUpGfxLightEnd() { hook::StaticThunk<0x59B460>::Call<void>(); }
        static AGE_API int GetNumPointLights() { return hook::StaticThunk<0x59B3E0>::Call<int>(); }
        static AGE_API ltLight* GetPointLight(int a1) { return hook::StaticThunk<0x59B3F0>::Call<ltLight*>(a1); }
        static AGE_API ltLight* GetClosestLight() { return hook::StaticThunk<0x59B410>::Call<ltLight*>(); }


        //member funcs
        AGE_API void Default() { hook::Thunk<0x59ABF0>::Call<void>(this); }
        AGE_API void Random() { hook::Thunk<0x59AC40>::Call<void>(this); }
        AGE_API void Draw(float scale) { hook::Thunk<0x59ACB0>::Call<void>(this, scale); }
        AGE_API void DrawGlow(Vector3* position) { hook::Thunk<0x59AD90>::Call<void>(this, position); }
        AGE_API void DrawHighlight(Vector3* a1, Matrix34* a2)
        {
            hook::Thunk<0x59AFB0>::Call<void>(this, a1, a2);
        }
        AGE_API void SetUpGfxLight() { hook::Thunk<0x59B5B0>::Call<void>(this); }
        AGE_API bool SetGfxLight(gfxLight* a1, Vector3* a2)
        {
            return hook::Thunk<0x59B740>::Call<bool>(this, a1, a2);
        }
        AGE_API void Illuminate(Vector3* outColor, Vector3* a2, Vector3* a3)
        {
            hook::Thunk<0x59B990>::Call<void>(this, outColor, a2, a3);
        }
        AGE_API float ComputeIntensity(Vector3* a1, float a2)
        {
            return hook::Thunk<0x59BA50>::Call<float>(this, a1, a2);
        }
        AGE_API float ComputeDistance(Vector3* a1) { return hook::Thunk<0x59BB70>::Call<float>(this, a1); }

        //TODO
        /*AGE_API void SetUpProjection(ltProjection* a1)  { hook::Thunk<0x59BBB0 >::Call<void>(this, a1); }*/

        AGE_API void FileIO(datParser* a1) { hook::Thunk<0x59BCA0>::Call<void>(this, a1); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<ltLight>("ltLight")
                //ctor
                .addConstructor(LUA_ARGS())
                //variables
                .addVariableRef("Type", &ltLight::Type)
                .addVariableRef("Position", &ltLight::Position)
                .addVariableRef("Direction", &ltLight::Direction)
                .addVariableRef("Color", &ltLight::Color)
                .addVariableRef("Intensity", &ltLight::Intensity)
                .addVariableRef("SpotExponent", &ltLight::SpotExponent)
                .addVariableRef("ProjectionSize", &ltLight::ProjectionSize)
                .addVariableRef("EnableProjection", &ltLight::EnableProjection)
                .addVariableRef("ShadowMode", &ltLight::ShadowMode)

                //statics
                .addStaticFunction("ShutdownLights", &ShutdownLights)
                .addStaticFunction("DrawGlowBegin", &DrawGlowBegin)
                .addStaticFunction("DrawGlowEnd", &DrawGlowEnd)
                .addStaticFunction("SetUpGfxLightBegin", &SetUpGfxLightBegin)
                .addStaticFunction("SetUpGfxLightEnd", &SetUpGfxLightEnd)
                .addStaticFunction("GetNumPointLights", &GetNumPointLights)
                .addStaticFunction("GetClosestLight", &GetClosestLight)
                .addStaticFunction("GetPointLight", &GetPointLight)

                //members
                .addFunction("Default", &Default)
                .addFunction("Random", &Random)
                .addFunction("Draw", &Draw)
                .addFunction("DrawGlow", &DrawGlow)
                .addFunction("DrawHighlight", &DrawHighlight)
                .addFunction("ComputeDistance", &ComputeDistance)
                .addFunction("ComputeIntensity", &ComputeIntensity)
                .addFunction("Illuminate", &Illuminate)
                .addFunction("SetUpGfxLight", &SetUpGfxLight)
                .addFunction("SetGfxLight", &SetGfxLight)
                .addFunction("FileIO", &FileIO)

                .endClass();
        }
    };
    ASSERT_SIZEOF(ltLight, 0x4C);

    // Lua initialization

}