#pragma once
#include <modules\vector.h>
#include <modules\parse.h>

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
        static bool PreventGeometryClipping;
        static hook::Type<float> GlowIntensity;
        static hook::Type<float> GlowScale;
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
    public:
        ANGEL_ALLOCATOR

        AGE_API ltLight();
        AGE_API ~ltLight();

        //static funcs
        static AGE_API void ShutdownLights();
        static AGE_API void DrawGlowBegin();
        static AGE_API void DrawGlowEnd();
        static AGE_API void SetUpGfxLightBegin(Vector3 const* a1);
        static AGE_API void SetUpGfxLightEnd();
        static AGE_API int GetNumPointLights();
        static AGE_API ltLight* GetPointLight(int a1);
        static AGE_API ltLight* GetClosestLight();


        //member funcs
        AGE_API void Default();
        AGE_API void Random();
        AGE_API void Draw(float scale);
        AGE_API void DrawGlow(Vector3 const& cameraPosition);
        AGE_API void DrawHighlight(Vector3* a1, Matrix34* a2);
        AGE_API void SetUpGfxLight();
        AGE_API bool SetGfxLight(gfxLight* a1, Vector3* a2);
        AGE_API void Illuminate(Vector3* outColor, Vector3* a2, Vector3* a3);
        AGE_API float ComputeIntensity(Vector3 const& a1, float a2);
        AGE_API float ComputeDistance(Vector3* a1);

        //TODO
        /*AGE_API void SetUpProjection(ltProjection* a1)  { hook::Thunk<0x59BBB0 >::Call<void>(this, a1); }*/

        AGE_API void FileIO(datParser* a1);

        static void BindLua(LuaState L);
    };
    ASSERT_SIZEOF(ltLight, 0x4C);

    // Lua initialization

}