#include <events\dispatcher.h>
#include "dgBangerInstanceHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgPropShadows("3DShadows", false);

/*
    dgBangerInstanceHandler
*/
hook::Type<gfxTexture*> glowTexture = 0x62767C;
gfxTexture* redGlowTexture;
bool glowLoaded = false;

void dgBangerInstanceHandler::Reset() {
    redGlowTexture = NULL;
    glowLoaded = false;
}

void dgBangerInstanceHandler::tglDrawParticle(const Vector3& position, float size, const Vector4& color)
{
    tglDrawParticleClipAdjusted(position, size, color);
}

void dgBangerInstanceHandler::DrawGlow()
{
    //first time texture load
    if (!glowLoaded) {
        redGlowTexture = gfxGetTexture("s_red_glow", true);
        glowLoaded = true;
    }

    //prepare glow texture
    dgBangerData* data = hook::Thunk<0x441AB0>::Call<dgBangerData *>(this);
    gfxTexture* lastTexture = (gfxTexture*)glowTexture;
    bool swappedTexture = false;

    if (!strcmp(data->GetName(), "sp_light_red_f") && lastTexture != NULL) {
        swappedTexture = true;
        glowTexture = redGlowTexture;
    }

    //draw glows
    ltLight::DrawGlowBegin();
    hook::Thunk<0x441840>::Call<void>(this); // call original
    ltLight::DrawGlowEnd();

    //reset glow texture
    if (swappedTexture) {
        glowTexture = lastTexture;
    }
}

void dgBangerInstanceHandler::DrawShadow()
{
    auto banger = reinterpret_cast<dgBangerInstance*>(this);
    auto timeWeather = cityLevel::GetCurrentLighting();

    if (MMSTATE->TimeOfDay == 3 || lvlLevel::GetSingleton()->GetRoomInfo(banger->GetRoomId())->Flags & static_cast<int>(RoomFlags::Subterranean))
        return;

    bool prevLighting = gfxRenderState::SetLighting(true);

    auto shaders = banger->GetShader(banger->GetVariant());
    modStatic* model = banger->GetGeomBase(0)->GetHighestLOD();

    if (model != nullptr)
    {
        Matrix34 shadowMatrix, dummyMatrix;
        Matrix34 bangerMatrix = banger->GetMatrix(dummyMatrix);
        if (lvlInstance::ComputeShadowProjectionMatrix(shadowMatrix, banger->GetRoomId(), timeWeather->KeyPitch, timeWeather->KeyHeading, bangerMatrix))
        {
            gfxRenderState::SetWorldMatrix(shadowMatrix);
            model->DrawShadowed(shaders, ComputeShadowIntensity(timeWeather->KeyColor));
        }
    }

    gfxRenderState::SetLighting(prevLighting);
}

bool dgBangerInstanceHandler::dgBangerInstance_BeginGeom(const char* a1, const char* a2, int a3)
{
    //We hook this to set flag 64 (shadow)
    auto inst = reinterpret_cast<lvlInstance*>(this);
    inst->SetFlag(64);

    //Call original
    return inst->BeginGeom(a1, a2, a3);
}

void dgBangerInstanceHandler::dgBangerManager_Init(int poolSize)
{
    //We hook this to set flag 64 (shadow)
    auto mgr = reinterpret_cast<dgBangerManager*>(this);
    mgr->Init(poolSize);

    for (int i = 0; i < poolSize; i++)
    {
        auto banger = mgr->GetBanger();
        banger->SetFlag(64);
    }
}

void dgBangerInstanceHandler::Install()
{
    InstallCallback("aiTrafficLightInstance::DrawGlow", "Make traffic light banger lights double sided.",
        &DrawGlow, {
            cb::call(0x53CCFD),
        }
    );

    InstallCallback("dgBangerInstance::DrawGlow", "Use custom tglDrawParticle.",
        &tglDrawParticle, {
            cb::call(0x441966),
        }
    );

    // makes banger glows double sided
    InstallVTableHook("dgBangerInstance::DrawGlow",
        &DrawGlow, {
            0x5B14CC,
            0x5B1544,
            0x5B15F0,
            0x5B570C,
            0x5B57D0,
            0x5B5FC4,
            0x5B610C,
            0x5B61B8
        }
    );

    GameEventDispatcher::RegisterStateEndCallback(Reset);

    if (cfgPropShadows.Get())
    {
        InstallVTableHook("dgBangerInstance::DrawShadow",
            &DrawShadow, {
                0x5B14C4,
                0x5B153C,
                0x5B15E8,
                0x5B1658,
                0x5B54DC,
                0x5B5688,
                0x5B5704,
                0x5B57C8,
                0x5B5FBC,
                0x5B6104,
                0x5B61B0
            }
        );

        InstallCallback("dgBangerInstance::Init", "Hook BeginGeom to set instance shadowing flag.",
            &dgBangerInstance_BeginGeom, {
                cb::call(0x53C7DE), // aiTrafficLightInstance::Init
                cb::call(0x441C86), // dgUnhitBangerInstance::Init
            }
        );

        //InstallCallback("dgBangerManager::Init", "Hook Init to set instance shadowing flag.",
        //    &dgBangerManager_Init, {
        //        cb::call(0x4129F4), // mmGame::Init
        //    }
        //);

        // increases the maximum distance from 5 to 10 meters to enable shadow rendering for very tall props 
        InstallPatch({ 0xD8, 0x25, 0x28, 0x4, 0x5B, 0x0 }, {
            0x464501
            });
    }
}

