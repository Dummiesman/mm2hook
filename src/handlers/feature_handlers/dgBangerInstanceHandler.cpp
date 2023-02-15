#include <events\dispatcher.h>
#include "dgBangerInstanceHandler.h"

using namespace MM2;

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

void dgBangerInstanceHandler::Install()
{
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

    InstallCallback("aiTrafficLightInstance::DrawGlow", "Make traffic light banger lights double sided.",
        &DrawGlow, {
            cb::call(0x53CCFD),
        }
    );

    GameEventDispatcher::RegisterStateEndCallback(Reset);
}

