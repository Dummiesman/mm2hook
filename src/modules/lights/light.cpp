#include "light.h"
#include <modules\rgl.h>

using namespace MM2;

declfield(ltLight::GlowScale)(0x5DDE9C);
declfield(ltLight::GlowIntensity)(0x5DDEA0);
bool ltLight::PreventGeometryClipping = true;


ltLight::ltLight() { hook::Thunk<0x59AB80>::Call<void>(this); }
ltLight::~ltLight() { hook::Thunk<0x59ABC0>::Call<void>(this); }

void ltLight::ShutdownLights() { hook::StaticThunk<0x59ABD0>::Call<void>(); }
void ltLight::DrawGlowBegin() { hook::StaticThunk<0x59AE30>::Call<void>(); }
void ltLight::DrawGlowEnd() { hook::StaticThunk<0x59AEF0>::Call<void>(); }
void ltLight::SetUpGfxLightBegin(Vector3 const* a1)
{
    hook::StaticThunk<0x59B390>::Call<void>(a1);
}
void ltLight::SetUpGfxLightEnd() { hook::StaticThunk<0x59B460>::Call<void>(); }
int ltLight::GetNumPointLights() { return hook::StaticThunk<0x59B3E0>::Call<int>(); }
ltLight* ltLight::GetPointLight(int a1) { return hook::StaticThunk<0x59B3F0>::Call<ltLight*>(a1); }
ltLight* ltLight::GetClosestLight() { return hook::StaticThunk<0x59B410>::Call<ltLight*>(); }

void ltLight::Default() { hook::Thunk<0x59ABF0>::Call<void>(this); }
void ltLight::Random() { hook::Thunk<0x59AC40>::Call<void>(this); }
void ltLight::Draw(float scale) { hook::Thunk<0x59ACB0>::Call<void>(this, scale); }
void ltLight::DrawGlow(Vector3 const & cameraPosition) 
{
    if (this->Type != 2)
    {
        float intensity = this->ComputeIntensity(cameraPosition, 0.0f);
        auto positionDifference = (cameraPosition - this->Position);
        Vector3 drawPosition = Vector3(this->Position);

        Vector3 color = Vector3(this->Color.X, this->Color.Y, this->Color.Z) * ltLight::GlowIntensity.get();
        float size =  sqrtf(intensity * positionDifference.Mag2()) * ltLight::GlowScale.get();

        //tglDrawParticle(drawPosition, size, Vector4(color.X, color.Y, color.Z, 1.0f));
        tglDrawParticleClipAdjusted(drawPosition, size, Vector4(color.X, color.Y, color.Z, 1.0f), 0.05f);
    }
}

void ltLight::DrawHighlight(Vector3* a1, Matrix34* a2)
{
    hook::Thunk<0x59AFB0>::Call<void>(this, a1, a2);
}
void ltLight::SetUpGfxLight() { hook::Thunk<0x59B5B0>::Call<void>(this); }
bool ltLight::SetGfxLight(gfxLight* a1, Vector3* a2)
{
    return hook::Thunk<0x59B740>::Call<bool>(this, a1, a2);
}
void ltLight::Illuminate(Vector3* outColor, Vector3* a2, Vector3* a3)
{
    hook::Thunk<0x59B990>::Call<void>(this, outColor, a2, a3);
}
float ltLight::ComputeIntensity(Vector3 const & a1, float a2)
{
    return hook::Thunk<0x59BA50>::Call<float>(this, &a1, a2);
}
float ltLight::ComputeDistance(Vector3* a1) { return hook::Thunk<0x59BB70>::Call<float>(this, a1); }

void ltLight::FileIO(datParser* a1) { hook::Thunk<0x59BCA0>::Call<void>(this, a1); }

void ltLight::BindLua(LuaState L) {
    LuaBinding(L).beginClass<ltLight>("ltLight")
        //ctor
        .addConstructor(LUA_ARGS())
        //variables
        .addVariable("Type", &ltLight::Type)
        .addVariable("Position", &ltLight::Position)
        .addVariable("Direction", &ltLight::Direction)
        .addVariable("Color", &ltLight::Color)
        .addVariable("Intensity", &ltLight::Intensity)
        .addVariable("SpotExponent", &ltLight::SpotExponent)
        .addVariable("ProjectionSize", &ltLight::ProjectionSize)
        .addVariable("EnableProjection", &ltLight::EnableProjection)
        .addVariable("ShadowMode", &ltLight::ShadowMode)

        //statics
        .addStaticVariable("PGC", &ltLight::PreventGeometryClipping)

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