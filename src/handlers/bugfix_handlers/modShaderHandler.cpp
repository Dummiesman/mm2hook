#include "modShaderHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgMm1StyleRefl("MM1StyleReflections", false);

/*
    modShaderHandler
*/
void modShaderHandler::BeginEnvMap(gfxTexture* a1, const Matrix34& a2)
{
    modShader::BeginEnvMap(a1, a2);
}

MM2::gfxMaterial* modShaderHandler::AddStaticMaterial(MM2::gfxMaterial const& reference)
{
    return modShader::AddStaticMaterial(reference);
}

void modShaderHandler::KillAll()
{
    modShader::KillAll();
}

void modShaderHandler::EndEnvMap()
{
    modShader::EndEnvMap();
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