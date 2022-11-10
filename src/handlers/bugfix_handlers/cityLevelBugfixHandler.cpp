#include "cityLevelBugfixHandler.h"

using namespace MM2;

/*
    cityLevelBugfixHandler
*/

hook::Type<asParticles*> sm_RainParticles(0x62770C);
hook::Type<bool> sm_EnablePVS(0x62B070);

Stream* cityLevelBugfixHandler::OpenPvsStream(const char* folder, const char* file, const char* extension, bool a4, bool a5) {
    //open stream
    auto stream = datAssetManager::Open(folder, file, extension, a4, a5);

    //stream will be NULL if the PVS doesn't exist
    if (!stream) {
        sm_EnablePVS = false;
    }

    //return original stream
    return stream;
}

void cityLevelBugfixHandler::UpdateRainParticles() {
    asParticles* rainParticles = (asParticles*)sm_RainParticles;

    // set position if appropriate
    if (!mmMirrorHandler::IsDrawing) {
        Vector4 dotWith = Vector4(0.0, 10.0, -10.0, 1.0);

        Vector4 newParticlePosition = Vector4(0, 0, 0, 0);
        newParticlePosition.Dot(dotWith, gfxRenderState::GetCameraMatrix());

        auto rainRule = rainParticles->GetBirthRule();
        rainRule->SetPosition(Vector3(newParticlePosition.X, newParticlePosition.Y, newParticlePosition.Z));
    }

    // render particles
    rainParticles->Cull();
}

void cityLevelBugfixHandler::Install() {
    InstallCallback("cityLevel::Load", "Disables PVS when it doesn't exist.",
        &OpenPvsStream, {
            cb::call(0x4440E8), // cityLevel::Load
        }
    );

    InstallCallback("lvlLevel::Draw", "Allows for control over when to update rain particle position.",
        &UpdateRainParticles, {
            cb::call(0x4462B7),
        }
    );
    mem::nop(0x4462BA + 0x02, 76); // nop out the rest of the rain update, since we're replacing it
}