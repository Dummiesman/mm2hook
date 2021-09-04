#include "modShaderHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgMm1StyleRefl("MM1StyleReflections", false);

/*
    modShaderHandler
*/

float oldFogStart, oldFogEnd;

void modShaderHandler::BeginEnvMap(gfxTexture* a1, const Matrix34* a2)
{
    // Disable fog so it's not blended with reflections
    oldFogStart = gfxRenderState::SetFogStart(9999.f);
    oldFogEnd = gfxRenderState::SetFogEnd(10000.f);

    hook::StaticThunk<0x4A41B0>::Call<void>(a1, a2); //call original
}

void modShaderHandler::EndEnvMap()
{
    // Restore last fog settings
    gfxRenderState::SetFogStart(oldFogStart);
    gfxRenderState::SetFogEnd(oldFogEnd);

    hook::StaticThunk<0x4A4420>::Call<void>(); //call original
}

void modShaderHandler::Install()
{
    InstallCallback("modShader::BeginEnvMap", "Turns off fog while drawing reflections.",
        &BeginEnvMap, {
            cb::call(0x4CE1F5),
            cb::call(0x5341DD),
            cb::call(0x552252),
        }
    );

    InstallCallback("modShader::EndEnvMap", "Turns off fog while drawing reflections.",
        &EndEnvMap, {
            cb::call(0x4CE228),
            cb::call(0x534202),
            cb::call(0x55226B),
        }
    );

    if (cfgMm1StyleRefl.Get()) {
        // changes the way reflections are rendered, similar to MM1
        // TODO: move to modShaderHandler feature handler instead!
        InstallPatch({ 0x03 }, {
            (0x4A4243 + 0x03),
            });
    }
}