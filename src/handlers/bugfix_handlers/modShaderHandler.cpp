#include "modShaderHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgMm1StyleRefl("MM1StyleReflections", false);

/*
    modShaderHandler
*/

float oldFogStart, oldFogEnd;
gfxMaterial modShaderHandler::sm_StaticMaterials[modShaderHandler::MaxStaticMaterials];
int modShaderHandler::sm_StaticMaterialCount = 0;

void modShaderHandler::BeginEnvMap(gfxTexture* a1, const Matrix34* a2)
{
    // Disable fog so it's not blended with reflections
    oldFogStart = gfxRenderState::SetFogStart(9999.f);
    oldFogEnd = gfxRenderState::SetFogEnd(10000.f);

    hook::StaticThunk<0x4A41B0>::Call<void>(a1, a2); //call original
}

MM2::gfxMaterial* modShaderHandler::AddStaticMaterial(MM2::gfxMaterial const& reference)
{
    // find our material in the list
    for (int i = 0; i < sm_StaticMaterialCount; i++)
    {
        auto& compareTo = sm_StaticMaterials[i];
        if (compareTo.Ambient == reference.Ambient &&
            compareTo.Diffuse == reference.Diffuse &&
            compareTo.Specular == reference.Specular &&
            compareTo.Emissive == reference.Emissive &&
            compareTo.Shininess == reference.Shininess)
        {
            return &sm_StaticMaterials[i];
        }
    }

    // if it's not found, add a new one
    if (sm_StaticMaterialCount >= modShaderHandler::MaxStaticMaterials)
    {
        Quitf("Out of static materials!");
    }

    sm_StaticMaterials[sm_StaticMaterialCount] = reference;
    sm_StaticMaterialCount++;
    return &sm_StaticMaterials[sm_StaticMaterialCount - 1];
}

void modShaderHandler::KillAll()
{
    sm_StaticMaterialCount = 0;
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

    InstallCallback("modShader::AddStaticMaterial", "Expand the number of static materials.",
        &AddStaticMaterial, {
            cb::call(0x4A3E71),
            cb::call(0x598206),
        }
    );

    InstallCallback("modShader::KillAll", "Expand the number of static materials.",
        &KillAll, {
            cb::call(0x402064),
            cb::call(0x465439),
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